#include "BufferAllocManager.h"
#include <boost/assert.hpp>

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
	void BufferAlloc::Invalidate()
	{
		OffsetInSlots = 0;
		NumSlots = 0;
	}
	bool BufferAlloc::IsValid() const
	{
		return NumSlots > 0;
	}

	const BufferAlloc BufferAlloc::Invalid{ 0, 0 };

	std::string ToString(const BufferAlloc& InAlloc)
	{
		std::string result;
		result.append("{");

		result.append("Offset=");
		result.append(std::to_string(InAlloc.OffsetInSlots));

		result.append("; NumSlots=");
		result.append(std::to_string(InAlloc.NumSlots));

		if (false == InAlloc.IsValid())
		{
			result.append("; INVALID!");
		}

		result.append("}");
		return result;
	}

	BufferAllocManager::BufferAllocManager()
	{
	}

	UINT BufferAllocManager::GetNumSlots() const
	{
		return Slots.size();
	}

	UINT BufferAllocManager::GetNumOccupiedSlots() const
	{
		return GetNumSlots() - GetNumFreeSlots();
	}

	UINT BufferAllocManager::GetNumFreeSlots() const
	{
		return FreeSlotIndices.size();
	}

	UINT BufferAllocManager::GetNumAllocs() const
	{
		return Allocs.size();
	}

	bool BufferAllocManager::TryResetCapacity(UINT InNewCapacityInSlots)
	{
		int const InitialSlotCount = GetNumSlots();

		if (GetNumOccupiedSlots() > InNewCapacityInSlots)
		{
			return false;
		}
		if (GetNumSlots() <= InNewCapacityInSlots)
		{
			int const CountSlotsToAppend = static_cast<int>(InNewCapacityInSlots - InitialSlotCount);
			BOOST_ASSERT(CountSlotsToAppend >= 0);
			for (int i = 0; i < CountSlotsToAppend; i++)
			{
				BufferSlotIndex NewSlotIndex = InitialSlotCount + i;
				auto FreeIt = FreeSlotIndices.insert(FreeSlotIndices.end(), NewSlotIndex);
				auto SlotIt = Slots.insert(Slots.end(), SlotState());
				SlotIt->MakeFree(FreeIt);
			}
		}
		else
		{
			int const CountSlotsToRemove = static_cast<int>(InitialSlotCount - InNewCapacityInSlots);
			BOOST_ASSERT(CountSlotsToRemove > 0);

			// We only set capacity if all extra elements are freed
			if ( IsFreeOfLength(InNewCapacityInSlots, CountSlotsToRemove) )
			{
				for (int i = 0; i < CountSlotsToRemove; i++)
				{
					BufferSlotIndex ExtraSlotIndex = InNewCapacityInSlots + i;
					auto FreeIt = GetSlotByIndex<SlotState>(ExtraSlotIndex, this)->GetFreeIt();
					FreeSlotIndices.erase(FreeIt);
				}
				Slots.resize(InNewCapacityInSlots);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	void BufferAllocManager::Clear()
	{
		for(AllocList::iterator AllocIt = Allocs.begin(); AllocIt != Allocs.end(); )
		{
			AllocList::iterator NextAllocIt = std::next(AllocIt);
			DoFreeAlloc(AllocIt);
			AllocIt = NextAllocIt;
		}
	}

	void BufferAllocManager::FreeAlloc(BufferAlloc& InAlloc)
	{
		const SlotState* FirstSlot = &Slots[InAlloc.OffsetInSlots];
		BOOST_ASSERT_MSG(FirstSlot->IsOccupied(), "BufferAllocManager::FreeAlloc: The first slot of the given alloc must be occupied");		

		DoFreeAlloc(FirstSlot->GetAllocIt());

		InAlloc.Invalidate();
	}

	std::optional<BufferAlloc> BufferAllocManager::NewAlloc(UINT InLengthInSlots)
	{
		std::optional<BufferSlotIndex> SlotIndex = FindFreeSlots(InLengthInSlots);
		if(false == SlotIndex.has_value())
		{
			return std::nullopt;
		}
		BufferAlloc NewAlloc {SlotIndex.value(), InLengthInSlots};
		auto AllocIt = Allocs.insert(Allocs.end(), NewAlloc);
		MakeSlotsOccupied(SlotIndex.value(), InLengthInSlots, AllocIt);

		return NewAlloc;
	}

	void BufferAllocManager::MakeSlotsOccupied(BufferSlotIndex InIndex, UINT InLengthInSlots, AllocList::iterator InAllocIt)
	{
		for (int i = 0; i < static_cast<int>(InLengthInSlots); i++)
		{
			SlotState* pSlot = GetSlotByIndex<SlotState>(InIndex + i, this);
			FreeSlotIndices.erase(pSlot->GetFreeIt());
			pSlot->MakeOccupied(InAllocIt);
		}
	}

	bool BufferAllocManager::TryReAlloc(BufferAlloc& InAlloc, UINT InLengthInSlots)
	{
		BOOST_ASSERT_MSG(InAlloc.IsValid(), "BufferAllocManager::TryReAlloc: alloc must be valid");
		BOOST_ASSERT_MSG(InLengthInSlots > 0, "BufferAllocManager: Length in slots must be greater than zero");

		if (InLengthInSlots < InAlloc.NumSlots)
		{
			MakeSlotsFree(InAlloc.OffsetInSlots + InLengthInSlots, InAlloc.NumSlots);
		}
		else
		{
			if (!IsFreeOfLength(InAlloc.OffsetInSlots, InLengthInSlots))
			{
				return false;
			}

			auto const AllocIt = GetSlotByIndex<const SlotState>(InAlloc.OffsetInSlots, this)->GetAllocIt();
			MakeSlotsOccupied(InAlloc.OffsetInSlots + InAlloc.NumSlots, InLengthInSlots, AllocIt);
		}

		InAlloc.NumSlots = InLengthInSlots;

		return true;
	}

	bool BufferAllocManager::IsFreeOfLength(BufferSlotIndex InIndex, UINT InLengthInSlots) const
	{
		for(int i = 0; i < static_cast<int>(InLengthInSlots); i++)
		{
			if( IsOccupiedSlot(InIndex + i) )
			{
				return false;
			}
		}
		return true;
	}

	std::optional<BufferSlotIndex> BufferAllocManager::FindFreeSlots(UINT InLengthInSlots)
	{
		for(BufferSlotIndex FreeSlotIndex : FreeSlotIndices)
		{
			if(IsFreeOfLength(FreeSlotIndex, InLengthInSlots))
			{
				return FreeSlotIndex;
			}
		}
		return std::optional<BufferSlotIndex>();
	}

	void BufferAllocManager::DoFreeAlloc(AllocList::iterator InAllocIt)
	{
		MakeSlotsFree(InAllocIt->OffsetInSlots, InAllocIt->NumSlots);
		Allocs.erase(InAllocIt);
	}

	void BufferAllocManager::MakeSlotsFree(BufferSlotIndex InIndex, UINT InLengthInSlots)
	{
		for (UINT i = 0; i < InLengthInSlots; i++)
		{
			BufferSlotIndex SlotIndex = InIndex + i;
			SlotState* pSlot = GetSlotByIndex<SlotState>(SlotIndex, this);
			BOOST_ASSERT_MSG(pSlot->IsOccupied(), "BufferAllocManager::MakeSlotsFree: Current slot must be occupied");

			auto const FreeIt = FreeSlotIndices.insert(FreeSlotIndices.end(), SlotIndex);
			pSlot->MakeFree(FreeIt);
		}
	}

	bool BufferAllocManager::IsOccupiedSlot(BufferSlotIndex InIndex) const
	{
		return GetSlotByIndex<const SlotState>(InIndex, this)->IsOccupied();
	}

	bool BufferAllocManager::IsFreeSlot(BufferSlotIndex InIndex) const
	{
		return GetSlotByIndex<const SlotState>(InIndex, this)->IsFree();
	}


	void BufferAllocManager::SlotState::MakeFree(FreeList::iterator InIt)
	{
		AllocIt.reset();
		FreeIt = InIt;
	}

	void BufferAllocManager::SlotState::MakeOccupied(AllocList::iterator InIt)
	{
		FreeIt.reset();
		AllocIt = InIt;
	}
} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv