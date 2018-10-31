#include "UniformBuffer.h"
#include "RenBufferUtils.h"
#include "RenHRUtils.h"
#include "../Utils/CommonSystem.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

namespace D3D
{
	namespace
	{
		D3D11_BUFFER_DESC GetDescForCapacity(const UniformBuffer* InBuffer)
		{
			D3D11_BUFFER_DESC Desc;
			ZeroMemory(&Desc, sizeof(Desc));
			Desc.ByteWidth = InBuffer->GetCapacityInBytes();
			Desc.Usage = InBuffer->GetUsage();
			Desc.CPUAccessFlags = InBuffer->GetCpuAccessFlags();
			Desc.BindFlags = InBuffer->GetBindFlags();
			Desc.MiscFlags = 0;
			Desc.StructureByteStride = 0;
			return Desc;
		}

		BufferHandle CreateBuffer(const UniformBuffer* InBuffer) throw(SpriteRenderException)
		{

			D3D11_BUFFER_DESC const Desc = GetDescForCapacity(InBuffer);
			ID3D11Buffer* pBuffer = nullptr;
			HRESULT hr = InBuffer->GetDev()->CreateBuffer(&Desc, nullptr, &pBuffer);
			if (FAILED(hr))
			{
				std::string const Msg = std::string("UniformBuffer: CreateBuffer Failed!!!; HR=") + GetCommonHRESULTDescription(hr);
				T_LOG_TO(InBuffer->GetLog(), Msg);
				throw SpriteRenderException(Msg);
			}
			else
			{
				T_LOG_TO(InBuffer->GetLog(), "UniformBuffer: CreateBuffer succeeed; HR=" << GetCommonHRESULTDescription(hr));
			}
			return BufferHandle{pBuffer};
		}

		struct BeginFlushResult
		{
			const UniformBuffer* pBuffer = nullptr;
			HRESULT HR = E_FAIL;
			D3D11_MAPPED_SUBRESOURCE Map;

			/**
			* True if was mapped, otherwise - was updated using UpdateSubresource.
			*/
			bool bWasMapped = false;

			bool IsSucceeded() const { return SUCCEEDED(HR); }

			ID3D11DeviceContext* GetDevCon() const { return pBuffer->GetDevCon(); }
			ID3D11Buffer* GetBuffer() const { return pBuffer->GetBuffer(); }

			BeginFlushResult(const UniformBuffer* pInBuffer, bool bInWasMapped, HRESULT InHR, const D3D11_MAPPED_SUBRESOURCE& InMap) :
				pBuffer{pInBuffer}
			,	bWasMapped{ bInWasMapped }
			,	HR{ InHR }
			,	Map{ InMap } {}

			/**
			* Only for UpdateSubresource: bInWasMapped must always be false!
			*/
			BeginFlushResult(const UniformBuffer* pInBuffer, bool bInWasMapped) :
				pBuffer{ pInBuffer }
			,	bWasMapped{ bInWasMapped }
			,	HR{ S_OK }
			{
				BOOST_ASSERT_MSG( ! bWasMapped, "BeginFlushResult ctor: bInWasMapped must be always false for this constructor!");
			}
		};

		BeginFlushResult BeginFlush(const UniformBuffer* InBuffer, const void* pInSourceData)
		{
			ID3D11DeviceContext* pDevCon = InBuffer->GetDevCon();
			ID3D11Buffer* pBuffer = InBuffer->GetBuffer();

			if (InBuffer->IsDynamic())
			{
				D3D11_MAPPED_SUBRESOURCE Map;
				D3D11_MAP MapType = D3D11_MAP_WRITE;
				switch (InBuffer->GetUsage())
				{
				case D3D11_USAGE_DYNAMIC:
					MapType = D3D11_MAP_WRITE_DISCARD;
					break;
				case D3D11_USAGE_STAGING:
					MapType = D3D11_MAP_WRITE;
					break;
				default:
					BOOST_ASSERT_MSG(false, "UniformBuffer: BeginFlush: Unknown usage constant");
				}
				HRESULT hr = pDevCon->Map(pBuffer, 0, MapType, 0, &Map);
				if (SUCCEEDED(hr))
				{
					std::memcpy(Map.pData, pInSourceData, InBuffer->GetCapacityInBytes());
				}
				return BeginFlushResult{InBuffer, /*bWasMapped*/true, hr, Map};
			}
			else
			{
				D3D11_BOX Box;
				ZeroMemory(&Box, sizeof(Box));
				Box.bottom = 1;
				Box.back = 1;
				Box.left = 0;
				Box.right = InBuffer->GetCapacityInBytes();
				pDevCon->UpdateSubresource(pBuffer, 0, &Box, pInSourceData, 0, 0);
				// UpdateSubresource never fails, so we should pass the HRESULT:
				return BeginFlushResult{InBuffer, /*bWasMapped*/false};
			}
		}

		void EndFlush(const BeginFlushResult& InResult)
		{
			BOOST_ASSERT(InResult.IsSucceeded());
			ID3D11DeviceContext* pDevCon = InResult.GetDevCon();
			ID3D11Buffer* pBuffer = InResult.GetBuffer();
			if (InResult.bWasMapped)
			{
				pDevCon->Unmap(pBuffer, 0);
			}
			// Nothing is to be done here (data are already copied by the update subresource)
		}
	}

	void CheckUniformBufferInitializer(const SUniformBufferInitializer& InInitializer)
	{
		BOOST_ASSERT_MSG(InInitializer.pLog, "CheckUniformBufferInitializer: log ptr must be NON-null");
		BOOST_ASSERT_MSG(InInitializer.pDev, "CheckUniformBufferInitializer: device ptr must be NON-null");
		BOOST_ASSERT_MSG(InInitializer.pDevCon, "CheckUniformBufferInitializer: device context ptr must be NON-null");
		BOOST_ASSERT_MSG(InInitializer.SlotSize > 0, "CheckUniformBufferInitializer: Slot size must be > 0");
		BOOST_ASSERT_MSG(InInitializer.CapacityInSlots > 0, "CheckUniformBufferInitializer: Capacity in slots must be > 0");
		BOOST_ASSERT_MSG(InInitializer.Usage != D3D11_USAGE_IMMUTABLE, "CheckUniformBufferInitializer: creating immutable uniform buffer is meaningless, so not supported");
		BOOST_ASSERT_MSG( ( InInitializer.BindFlags == 0 ) || ( InInitializer.Usage != D3D11_USAGE_STAGING ), "CheckUniformBufferInitializer: STAGING resource must have NO bind flags!");

		BOOST_ASSERT_MSG(false == InInitializer.bAutoResizeable, "CheckUniformBufferInitializer: bAutoResizable NOT implemented!");
	}

	UniformBuffer::UniformBuffer(const SUniformBufferInitializer& InInitializer) :
		bStoring{false}
	,	bD3DBufferUpToDate{false}
	{
		CheckUniformBufferInitializer(InInitializer);
		pLog = InInitializer.pLog;
		T_LOG("UniformBuffer: ctor...");
		bDebug = InInitializer.bDebug;
		BufferName = InInitializer.Name;
		T_LOG("Name: " << InInitializer.Name);
		pDev = InInitializer.pDev;
		pDevCon = InInitializer.pDevCon;
		SlotSize = InInitializer.SlotSize;
		CapacityInSlots = InInitializer.CapacityInSlots;
		InitialCapacity = InInitializer.CapacityInSlots;
		BindFlags = InInitializer.BindFlags;
		Usage = InInitializer.Usage;
		CpuAccessFlags = InInitializer.CpuAccessFlags;
		bAutoResizeable = InInitializer.bAutoResizeable;
		
		BOOST_ASSERT_MSG(Allocs.TryResetCapacity(CapacityInSlots), "UniformBuffer::ctor: resetting capacity of the allocation manager should never fail at this point because it's empty!");
		Data.resize(GetCapacityInBytes());
		ReCreateD3DBuffer();

		T_LOG("UniformBuffer: ctor DONE");
	}

	void UniformBuffer::ReCreateD3DBuffer()  throw(SpriteRenderException)
	{
		T_LOG("UniformBuffer::ReCreateD3DBuffer; Name=" << GetBufferName() << "...");

		LogBufferState(GetLog(), this);

		pBuffer = CreateBuffer(this);
		bD3DBufferUpToDate = false;
		
		T_LOG("UniformBuffer::ReCreateD3DBuffer DONE");
	}

	std::ofstream& UniformBuffer::GetLog() const
	{
		return *pLog;
	}

	bool UniformBuffer::IsDynamic() const
	{
		return GetUsage() == D3D11_USAGE_DYNAMIC || GetUsage() == D3D11_USAGE_STAGING;
	}

	const void* UniformBuffer::GetData() const
	{
		BOOST_ASSERT(Data.size() == GetCapacityInBytes());
		return Data.data();
	}

	UINT UniformBuffer::GetCapacityInBytes() const
	{
		return GetNumSlots() * GetSlotSize();
	}

	ID3D11Buffer* UniformBuffer::GetBuffer() const
	{
		BOOST_ASSERT(pBuffer.get());
		return pBuffer.get(); 
	}

	void UniformBuffer::ResetCapacity(UINT InNewCapacityInSlots)
	{
		T_LOG("UniformBuffer::ResetCapacity; Name=" << GetBufferName() << "...");
		T_LOG("NewCapacity (in slots) " << InNewCapacityInSlots);

		BOOST_ASSERT_MSG(InNewCapacityInSlots > 0, "UniformBuffer::ResetCapacity: new capacity must be greater than zero!");
		if (InNewCapacityInSlots < GetNumOccupiedSlots())
		{
			T_LOG("UniformBuffer::ResetCapacity: new capacity is less than current number of slots - skipping reset");
		}
		else
		{

			// We know that ReCreateD3DBuffer will update this flag automatically, however we set up it now
			// because later we may refuse to use ReCreateD3DBuffer here and we do NOT want to introduce a bug here.
			bD3DBufferUpToDate = false;

			UINT const OldCapacity = CapacityInSlots;
			if (false == Allocs.TryResetCapacity(InNewCapacityInSlots))
			{
				if (OldCapacity < InNewCapacityInSlots)
				{
					std::string const Msg = std::string("UniformBuffer: Name=") + GetBufferName() + std::string("; AllocManager failed to reset capacity to greater value");
					T_LOG(Msg);
					throw(SpriteRenderException(Msg));
				}
				else
				{
					T_LOG("UniformBuffer: failed to reset capacity to a lesser value");
					// NOTE: We should NOT throw here, as it's NOT a fatal failure.
				}
			}
			else
			{
				CapacityInSlots = InNewCapacityInSlots;
				Data.resize(InNewCapacityInSlots * GetSlotSize());
				ReCreateD3DBuffer();
			}
		}

		T_LOG("UniformBuffer: ResetCapacity DONE");
	}

	void UniformBuffer::Clear(bool bInResetCapacity)
	{
		T_LOG("UniformBuffer::Clear; Name=" << GetBufferName() << "...");
		T_LOG("bInResetCapacity: " << (bInResetCapacity ? "YES" : "no"));

		BOOST_ASSERT_MSG( ! IsStoring(), "UniformBuffer::Clear: should not be clear while storing!");

		bD3DBufferUpToDate = false;

		Allocs.Clear();

		if (bInResetCapacity)
		{
			T_LOG("bInResetCapacity==TRUE: ResettingCapacity to initial:");
			ResetCapacity(InitialCapacity);
		}

		if (bDebug)
		{
			T_LOG("UniformBuffer::Clear: Debug mode used - zeroing memory");
			// It's unnecessary to zero the data, but we do zero for debug purposes
			ZeroMemory(Data.data(), Data.size());
		}

		T_LOG("UniformBuffer: Clear DONE");
	}

	UINT UniformBuffer::GetNumFreeSlots() const
	{
		return Allocs.GetNumFreeSlots();
	}

	UINT UniformBuffer::GetNumOccupiedSlots() const
	{
		return Allocs.GetNumOccupiedSlots();
	}

	UINT UniformBuffer::GetNumAllocs() const
	{
		return Allocs.GetNumAllocs();
	}

	void UniformBuffer::Flush() throw(SpriteRenderException)
	{
		BOOST_ASSERT_MSG(!IsStoring(), "UniformBuffer::Flush: should not be flushing while storing");
		BOOST_ASSERT_MSG(pBuffer, "UniformBuffer::Flush: the buffer must be created at this time");

		// We should not do anything if buffer is up to date
		if (bD3DBufferUpToDate)
		{

			return;
		}

		BOOST_ASSERT_MSG(Data.size() == GetCapacityInBytes(), "UniformBuffer::Flush: size of the source data must be always equal to the capacity");
		BeginFlushResult Result = BeginFlush(this, Data.data());
		if ( ! Result.IsSucceeded() )
		{
			std::string const Msg = std::string("UniformBuffer: BeginFlush: failed; ") + (Result.bWasMapped ? "Mapped" : "UpdateSubresource") + "; HR=" + GetCommonHRESULTDescription(Result.HR);
			T_LOG(Msg);
			throw SpriteRenderException(Msg);
		}
		// Nothing is to be done here - copying is performed inside the BeginFlush/EndFlush
		EndFlush(Result);
	
		bD3DBufferUpToDate = true;
	}

	void UniformBuffer::StartStore()
	{
		BOOST_ASSERT_MSG( ! IsStoring(), "UniformBuffer::StartStore: already storing");

		bStoring = true;

		// NOTE: Buffer should NOT be updated here - @See Flush
	}

	void UniformBuffer::EndStore()
	{
		BOOST_ASSERT_MSG( IsStoring(), "UniformBuffer::EndStore: not storing now");	

		bStoring = false;

		// NOTE: Buffer should NOT be updated here - @See Flush
	}

	bool UniformBuffer::IsStoring() const
	{
		return bStoring;
	}

	BufferAlloc UniformBuffer::Alloc(const void* pInSourceData, UINT InSizeInBytes)
	{
		T_LOG("UniformBuffer::Alloc; Name=" << GetBufferName() << "...");
		T_LOG("SizeInBytes = " << InSizeInBytes);

		BOOST_ASSERT_MSG(pInSourceData, "UniformBuffer::Alloc: source data ptr must be non-null");
		BOOST_ASSERT_MSG(InSizeInBytes > 0, "UniformBuffer::Alloc: Source data size must be greater than zero!");
		BOOST_ASSERT_MSG((InSizeInBytes % SlotSize == 0), "UniformBuffer::Alloc: Source data size must be a multiple of slot size!");
		
		UINT const SourceData_LengthInSlots = InSizeInBytes / SlotSize;

		T_LOG("Source data length in slots = " << SourceData_LengthInSlots);

		std::optional<BufferAlloc> TheAlloc = Allocs.NewAlloc(SourceData_LengthInSlots);
		if (false == TheAlloc.has_value())
		{
			T_LOG("AllocManager::NewAlloc failed");
			BOOST_ASSERT_MSG( ! bAutoResizeable, "UniformBuffer::Alloc: auto-resize is NOT implemented yet!");
			throw SpriteRenderException("UniformBuffer::Alloc: AllocManager::NewAlloc failed");
		}
		BOOST_ASSERT_MSG(TheAlloc && TheAlloc->IsValid(), "UniformBuffer::Alloc: At this point the alloc must be valid");

		const UINT DestOffsetInBytes = TheAlloc->OffsetInSlots * SlotSize;
		std::memcpy(Data.data() + DestOffsetInBytes, pInSourceData, InSizeInBytes);

		bD3DBufferUpToDate = false;

		T_LOG("UniformBuffer: Alloc DONE");
		return TheAlloc.value();
	}

	void UniformBuffer::Realloc(BufferAlloc& InAlloc, const void* pInSourceData, UINT InSizeInBytes)
	{
		BOOST_ASSERT_MSG(!bAutoResizeable, "UniformBuffer::Realloc: auto-resize is NOT implemented yet and this implementation of realloc does NOT work without auto-resize!");

		T_LOG("UniformBuffer::ReAlloc; Name=" << GetBufferName() << "...");
		T_LOG("Alloc=" << ToString(InAlloc));
		T_LOG("SizeInBytes=" << InSizeInBytes);

		BOOST_ASSERT_MSG(InAlloc.IsValid(), "UniformBuffer::Realloc: the given alloc must be valid");
		BOOST_ASSERT_MSG(pInSourceData, "UniformBuffer::Realloc: source data pointer must be NON-null");
		BOOST_ASSERT_MSG(InSizeInBytes > 0, "UniformBuffer::Realloc: Source data size must be greater than zero!");
		BOOST_ASSERT_MSG((InSizeInBytes % SlotSize == 0), "UniformBuffer::Realloc: Source data size must be a multiple of slot size!");
		
		if (false == Allocs.TryReAlloc(InAlloc, InSizeInBytes))
		{
			T_LOG("AllocManager::TryReAlloc failed");
			BOOST_ASSERT_MSG( ! bAutoResizeable, "UniformBuffer::Realloc: auto-resize is NOT implemented yet!" );
			throw SpriteRenderException("UniformBuffer::Realloc: AllocManager::TryReAlloc failed");
		}

		const UINT DestOffsetInBytes = InAlloc.OffsetInSlots * SlotSize;
		std::memcpy(Data.data() + DestOffsetInBytes, pInSourceData, InSizeInBytes);

		bD3DBufferUpToDate = false;

		T_LOG("UniformBuffer: Realloc DONE");
	}

	void UniformBuffer::OverwriteAlloc(const BufferAlloc& InAlloc, const void* pInSourceData, UINT InSizeInBytes, UINT InOffsetInBytes)
	{
		BOOST_ASSERT(InAlloc.IsValid());
		BOOST_ASSERT(pInSourceData);
		BOOST_ASSERT(InSizeInBytes > 0);

		const UINT AllocSizeInBytes = InAlloc.NumSlots * SlotSize;
		BOOST_ASSERT(InOffsetInBytes + InSizeInBytes <= AllocSizeInBytes);


		const UINT DestOffsetInBytes = InAlloc.OffsetInSlots * SlotSize + InOffsetInBytes;
		std::memcpy(Data.data() + DestOffsetInBytes, pInSourceData, InSizeInBytes);

		bD3DBufferUpToDate = false;
	}

	void UniformBuffer::FreeAlloc(BufferAlloc& InAlloc)
	{
		T_LOG("UniformBuffer::FreeAlloc; Name=" << GetBufferName() << "...");
		T_LOG("Alloc=" << ToString(InAlloc));

		BOOST_ASSERT_MSG(InAlloc.IsValid(), "UniformBuffer::FreeAlloc: the given alloc must be valid");

		if (bDebug)
		{
			T_LOG("UniformBuffer::FreeAlloc: Debug mode used - zeroing alloc memory");
			ZeroMemory(Data.data() + InAlloc.OffsetInSlots * SlotSize, InAlloc.NumSlots * SlotSize);
		}

		Allocs.FreeAlloc(InAlloc);
		// WARNING!!! We should NOT set the bD3DBufferUpToDate as false here:
		// Freed slots are treated as uninitialized, and we should not commit uninitialized regions to the d3d buffers.

		T_LOG("UniformBuffer: FreeAlloc DONE");
	}

	void LogBufferState(std::ofstream& InLog, const UniformBuffer* pInBuffer)
	{
		BOOST_ASSERT(pInBuffer);
		T_LOG_TO(InLog, "Logging buffer state...");
		T_LOG_TO(InLog, "Name: " << pInBuffer->GetBufferName());
		T_LOG_TO(InLog, "bDebug: " << (pInBuffer->IsDebug() ? "YES" : "no"));
		T_LOG_TO(InLog, "bAutoResizable:" << (pInBuffer->IsAutoResizeable() ? "YES" : "no"));
		T_LOG_TO(InLog, "SlotSize:" << pInBuffer->GetSlotSize());
		T_LOG_TO(InLog, "NumFreeSlots / NumOccuppiedSlots = " << pInBuffer->GetNumFreeSlots() << "/" << pInBuffer->GetNumOccupiedSlots());
		T_LOG_TO(InLog, "NumAllocs: " << pInBuffer->GetNumAllocs());
		T_LOG_TO(InLog, "CapacityInSlots:" << pInBuffer->GetNumSlots());
		T_LOG_TO(InLog, "CapacityInBytes:" << pInBuffer->GetCapacityInBytes());
		T_LOG_TO(InLog, "BindFlags: " << GetBindFlagsString(pInBuffer->GetBindFlags()));
		T_LOG_TO(InLog, "Usage: " << GetUsageFlags(pInBuffer->GetUsage()));
		T_LOG_TO(InLog, "CPUAccessFlags: " << GetCPUAccessFlagsString(pInBuffer->GetCpuAccessFlags()));
		T_LOG_TO(InLog, "Logging buffer state DONE");
	}
} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv