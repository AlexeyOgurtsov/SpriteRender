#include "BlobBuffer.h"
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
		D3D11_BUFFER_DESC GetDescForCapacity(const BlobBuffer* InBuffer)
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

		BufferHandle CreateBuffer(const BlobBuffer* InBuffer) throw(SpriteRenderException)
		{

			D3D11_BUFFER_DESC const Desc = GetDescForCapacity(InBuffer);
			ID3D11Buffer* pBuffer = nullptr;
			HRESULT hr = InBuffer->GetDev()->CreateBuffer(&Desc, nullptr, &pBuffer);
			if (FAILED(hr))
			{
				std::string const Msg = std::string("BlobBuffer: CreateBuffer Failed!!!; HR=") + GetCommonHRESULTDescription(hr);
				T_LOG_TO(InBuffer->GetLog(), Msg);
				throw SpriteRenderException(Msg);
			}
			else
			{
				T_LOG_TO(InBuffer->GetLog(), "BlobBuffer: CreateBuffer succeeed; HR=" << GetCommonHRESULTDescription(hr));
			}
			return BufferHandle{pBuffer};
		}

		struct BeginFlushResult
		{
			const BlobBuffer* pBuffer = nullptr;
			HRESULT HR = E_FAIL;
			D3D11_MAPPED_SUBRESOURCE Map;

			/**
			* True if was mapped, otherwise - was updated using UpdateSubresource.
			*/
			bool bWasMapped = false;

			bool IsSucceeded() const { return SUCCEEDED(HR); }

			ID3D11DeviceContext* GetDevCon() const { return pBuffer->GetDevCon(); }
			ID3D11Buffer* GetBuffer() const { return pBuffer->GetBuffer(); }

			BeginFlushResult(const BlobBuffer* pInBuffer, bool bInWasMapped, HRESULT InHR, const D3D11_MAPPED_SUBRESOURCE& InMap) :
				pBuffer{pInBuffer}
			,	bWasMapped{ bInWasMapped }
			,	HR{ InHR }
			,	Map{ InMap } {}

			/**
			* Only for UpdateSubresource: bInWasMapped must always be false!
			*/
			BeginFlushResult(const BlobBuffer* pInBuffer, bool bInWasMapped) :
				pBuffer{ pInBuffer }
			,	bWasMapped{ bInWasMapped }
			,	HR{ S_OK }
			{
				BOOST_ASSERT_MSG( ! bWasMapped, "BeginFlushResult ctor: bInWasMapped must be always false for this constructor!");
			}
		};

		BeginFlushResult BeginFlush(const BlobBuffer* InBuffer, const void* pInSourceData)
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
					BOOST_ASSERT_MSG(false, "BlobBuffer: BeginFlush: Unknown usage constant");
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

	void CheckBlobBufferInitializer(const SBlobBufferInitializer& InInitializer)
	{
		BOOST_ASSERT_MSG(InInitializer.pLog, "CheckBlobBufferInitializer: log ptr must be NON-null");
		BOOST_ASSERT_MSG(InInitializer.pDev, "CheckBlobBufferInitializer: device ptr must be NON-null");
		BOOST_ASSERT_MSG(InInitializer.pDevCon, "CheckUniformBufferInitializer: device context ptr must be NON-null");
		BOOST_ASSERT_MSG(InInitializer.CapacityInBytes > 0, "CheckBlobBufferInitializer: Capacity in bytes must be > 0");
		BOOST_ASSERT_MSG(( InInitializer.BindFlags == D3D11_BIND_CONSTANT_BUFFER ) || (InInitializer.CapacityInBytes % 16 == 0), "CheckBlobBufferInitializer: Size of a constant buffer must be a multiple of 16");
		BOOST_ASSERT_MSG(InInitializer.Usage != D3D11_USAGE_IMMUTABLE, "CheckBlobBufferInitializer: creating immutable blob buffer is meaningless, so not supported");
		BOOST_ASSERT_MSG( ( InInitializer.BindFlags == 0 ) || ( InInitializer.Usage != D3D11_USAGE_STAGING ), "CheckBlobBufferInitializer: STAGING resource must have NO bind flags!");
	}

	BlobBuffer::BlobBuffer(const SBlobBufferInitializer& InInitializer) :
		bStoring{ false }
	,	bD3DBufferUpToDate{ false }
	{
		CheckBlobBufferInitializer(InInitializer);
		pLog = InInitializer.pLog;
		T_LOG("BlobBuffer: ctor...");
		bDebug = InInitializer.bDebug;
		BufferName = InInitializer.Name;
		T_LOG("Name: " << InInitializer.Name);
		pDev = InInitializer.pDev;
		pDevCon = InInitializer.pDevCon;
		CapacityInBytes = InInitializer.CapacityInBytes;
		BindFlags = InInitializer.BindFlags;
		Usage = InInitializer.Usage;
		CpuAccessFlags = InInitializer.CpuAccessFlags;

		Data.resize(GetCapacityInBytes());
		ReCreateD3DBuffer();

		T_LOG("BlobBuffer: ctor DONE");
	}

	void BlobBuffer::ReCreateD3DBuffer()  throw(SpriteRenderException)
	{
		T_LOG("BlobBuffer::ReCreateD3DBuffer; Name=" << GetBufferName() << "...");

		LogBufferState(GetLog(), this);

		pBuffer = CreateBuffer(this);
		bD3DBufferUpToDate = false;

		T_LOG("BlobBuffer::ReCreateD3DBuffer DONE");
	}

	std::ofstream& BlobBuffer::GetLog() const
	{
		return *pLog;
	}

	bool BlobBuffer::IsDynamic() const
	{
		return GetUsage() == D3D11_USAGE_DYNAMIC || GetUsage() == D3D11_USAGE_STAGING;
	}

	const void* BlobBuffer::GetData() const
	{
		BOOST_ASSERT(Data.size() == GetCapacityInBytes());
		return Data.data();
	}

	UINT BlobBuffer::GetCapacityInBytes() const
	{
		return CapacityInBytes;
	}

	ID3D11Buffer* BlobBuffer::GetBuffer() const
	{
		BOOST_ASSERT(pBuffer.get());
		return pBuffer.get();
	}


	void BlobBuffer::Flush() throw(SpriteRenderException)
	{
		BOOST_ASSERT_MSG(!IsStoring(), "BlobBuffer::Flush: should not be flushing while storing");
		BOOST_ASSERT_MSG(pBuffer, "BlobBuffer::Flush: the buffer must be created at this time");

		// We should not do anything if buffer is up to date
		if (bD3DBufferUpToDate)
		{

			return;
		}

		BOOST_ASSERT_MSG(Data.size() == GetCapacityInBytes(), "BlobBuffer::Flush: size of the source data must be always equal to the capacity");
		BeginFlushResult Result = BeginFlush(this, Data.data());
		if ( ! Result.IsSucceeded() )
		{
			std::string const Msg = std::string("BlobBuffer: BeginFlush: failed; ") + (Result.bWasMapped ? "Mapped" : "UpdateSubresource") + "; HR=" + GetCommonHRESULTDescription(Result.HR);
			T_LOG(Msg);
			throw SpriteRenderException(Msg);
		}
		// Nothing is to be done here - copying is performed inside the BeginFlush/EndFlush
		EndFlush(Result);
	
		bD3DBufferUpToDate = true;
	}

	void BlobBuffer::StartStore()
	{
		BOOST_ASSERT_MSG( ! IsStoring(), "BlobBuffer::StartStore: already storing");

		bStoring = true;

		// NOTE: Buffer should NOT be updated here - @See Flush
	}

	void BlobBuffer::EndStore()
	{
		BOOST_ASSERT_MSG( IsStoring(), "BlobBuffer::EndStore: not storing now");	

		bStoring = false;

		// NOTE: Buffer should NOT be updated here - @See Flush
	}

	void BlobBuffer::SetCB(ID3D11DeviceContext* pDevCon, UINT InSlot)
	{
		ID3D11Buffer* pBuffer = GetBuffer();
		pDevCon->VSSetConstantBuffers(InSlot, 1, &pBuffer);
		pDevCon->PSSetConstantBuffers(InSlot, 1, &pBuffer);
	}

	bool BlobBuffer::IsStoring() const
	{
		return bStoring;
	}

	void BlobBuffer::CopyBytesToData(UINT InOffset, const void* pInSourceData, UINT InSizeInBytes)
	{
		BOOST_ASSERT(pInSourceData);
		BOOST_ASSERT(InSizeInBytes > 0);
		BOOST_ASSERT(InOffset + InSizeInBytes <= CapacityInBytes);
		std::memcpy(GetDestBuffer(InOffset), pInSourceData, InSizeInBytes);
	}

	uint8_t* BlobBuffer::GetDestBuffer(UINT InOffset)
	{
		BOOST_ASSERT_MSG(IsStoring(), "BlobBuffer::GetDestBuffer: must be storing now");
		BOOST_ASSERT(InOffset < CapacityInBytes);
		return Data.data() + InOffset;
	}

	void BlobBuffer::SetMatrix3x3(UINT InOffset, const float* pInValue)
	{
		for (int i = 0; i < 3; i++)
		{
			CopyBytesToData(InOffset, pInValue + 4 * i, 4 * sizeof(float));
		}
	}

	void BlobBuffer::SetMatrix4x4(UINT InOffset, const float* pInValue)
	{
		CopyBytesToData(InOffset, pInValue, 16 * sizeof(float));
	}

	void BlobBuffer::SetVectorFour(UINT InOffset, const float* pInValue)
	{
		CopyBytesToData(InOffset, pInValue, 4 * sizeof(float));
	}

	void BlobBuffer::SetVectorThree(UINT InOffset, const float* pInValue)
	{
		CopyBytesToData(InOffset, pInValue, 3 * sizeof(float));
	}

	void BlobBuffer::SetVector2d(UINT InOffset, const float* pInValue)
	{
		CopyBytesToData(InOffset, pInValue, 2 * sizeof(float));
	}

	void LogBufferState(std::ofstream& InLog, const BlobBuffer* pInBuffer)
	{
		BOOST_ASSERT(pInBuffer);
		T_LOG_TO(InLog, "Logging buffer state...");
		T_LOG_TO(InLog, "Name: " << pInBuffer->GetBufferName());
		T_LOG_TO(InLog, "bDebug: " << (pInBuffer->IsDebug() ? "YES" : "no"));
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