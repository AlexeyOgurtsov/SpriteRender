#include "UniformBuffer.h"
#include "RenBufferUtils.h"

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
		D3D11_BUFFER_DESC GetDescForCapacity(const UniformBuffer* InBuffer, UINT InNumSlots)
		{
			D3D11_BUFFER_DESC Desc;
			ZeroMemory(&Desc, sizeof(Desc));
			Desc.ByteWidth = InBuffer->GetSlotSize() * InNumSlots;
			Desc.Usage = InBuffer->GetUsage();
			Desc.CPUAccessFlags = InBuffer->GetCpuAccessFlags();
			Desc.BindFlags = InBuffer->GetBindFlags();
			return Desc;
		}
	}

	void CheckUniformBufferInitializer(const SUniformBufferInitializer& InInitializer)
	{
		BOOST_ASSERT_MSG(InInitializer.pDev, "CheckUniformBufferInitializer: device ptr must be NON-null");
		BOOST_ASSERT_MSG(InInitializer.pDevCon, "CheckUniformBufferInitializer: device context ptr must be NON-null");
		BOOST_ASSERT_MSG(InInitializer.SlotSize > 0, "CheckUniformBufferInitializer: Slot size must be > 0");
		BOOST_ASSERT_MSG(InInitializer.CapacityInSlots > 0, "CheckUniformBufferInitializer: Capacity in slots must be > 0");
	}

	UniformBuffer::UniformBuffer(const SUniformBufferInitializer& InInitializer) :
		bStoring{false}
	{
		CheckUniformBufferInitializer(InInitializer);
		pDev = InInitializer.pDev;
		pDevCon = InInitializer.pDevCon;
		SlotSize = InInitializer.SlotSize;
		CapacityInSlots = InInitializer.CapacityInSlots;
		BindFlags = InInitializer.BindFlags;
		Usage = InInitializer.Usage;
		CpuAccessFlags = InInitializer.CpuAccessFlags;
		bAutoResizeable = InInitializer.bAutoResizeable;

		// @TODO: Create Buffer
	}

	void UniformBuffer::ResetCapacity(UINT InCapacityInSlots)
	{
		BOOST_ASSERT_MSG(InCapacityInSlots > 0, "UniformBuffer::ResetCapacity: new capacity must be greater than zero!");
		BOOST_ASSERT_MSG(InCapacityInSlots >= GetNumOccupiedSlots(), "UniformBuffer::ResetCapacity: new capacity must be greater or equal than current count of occupied slots!");

		BOOST_ASSERT_MSG(false, "UniformBuffer::ResetCapacity: NOT yet impl!");
	}

	void UniformBuffer::Clear()
	{
		BOOST_ASSERT_MSG(false, "UniformBuffer::Clear: NOT yet impl!");
	}

	UINT UniformBuffer::GetNumFreeSlots() const
	{
		BOOST_ASSERT_MSG(false, "UniformBuffer::GetNumFreeSlots: NOT YET IMPL"); return 0;
	}

	UINT UniformBuffer::GetNumOccupiedSlots() const
	{
		BOOST_ASSERT_MSG(false, "UniformBuffer::GetNumOccupiedSlots: NOT YET IMPL"); return 0;	
	}

	UINT UniformBuffer::GetNumAllocs() const
	{
		BOOST_ASSERT_MSG(false, "UniformBuffer::GetNumAllocs: NOT YET IMPL"); return 0;	
	}

	void UniformBuffer::StartStore()
	{
		BOOST_ASSERT_MSG( ! IsStoring(), "UniformBuffer::StartStore: already storing");

		bStoring = true;

		BOOST_ASSERT_MSG(false, "UniformBuffer::StartStore: NOT YET IMPL");
	}

	void UniformBuffer::EndStore()
	{
		BOOST_ASSERT_MSG( IsStoring(), "UniformBuffer::EndStore: not storing now");	

		bStoring = false;

		BOOST_ASSERT_MSG(false, "UniformBuffer::EndStore: NOT YET IMPL");
	}

	bool UniformBuffer::IsStoring() const
	{
		return bStoring;
	}

	BufferAlloc UniformBuffer::Alloc(const void* pInSourceData, UINT InSizeInBytes)
	{
		BOOST_ASSERT_MSG(pInSourceData, "UniformBuffer::Alloc: source data ptr must be non-null");
		BOOST_ASSERT_MSG(InSizeInBytes > 0, "UniformBuffer::Alloc: Source data size must be greater than zero!");
		BOOST_ASSERT_MSG((InSizeInBytes % SlotSize == 0), "UniformBuffer::Alloc: Source data size must be a multiple of slot size!");
		
		UINT const SourceData_LengthInSlots = InSizeInBytes / SlotSize;

		BOOST_ASSERT_MSG(false, "UniformBuffer::EndStore: NOT YET IMPL"); return BufferAlloc::Invalid;
	}

	void UniformBuffer::Realloc(BufferAlloc& InAlloc, const void* pInSourceData, UINT InSizeInBytes)
	{
		BOOST_ASSERT_MSG(InAlloc.IsValid(), "UniformBuffer::Realloc: the given alloc must be valid");
		BOOST_ASSERT_MSG(pInSourceData, "UniformBuffer::Realloc: source data pointer must be NON-null");
		BOOST_ASSERT_MSG(InSizeInBytes > 0, "UniformBuffer::Realloc: Source data size must be greater than zero!");
		BOOST_ASSERT_MSG((InSizeInBytes % SlotSize == 0), "UniformBuffer::Realloc: Source data size must be a multiple of slot size!");
		

		BOOST_ASSERT_MSG(false, "UniformBuffer::Realloc: NOT YET IMPL");
	}

	void UniformBuffer::FreeAlloc(BufferAlloc& InAlloc)
	{
		BOOST_ASSERT_MSG(InAlloc.IsValid(), "UniformBuffer::FreeAlloc: the given alloc must be valid");

		BOOST_ASSERT_MSG(false, "UniformBuffer::FreeAlloc: NOT YET IMPL");
	}
} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv