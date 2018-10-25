#pragma once

#include "BufferAlloc.h"
#include <windows.h>
#include <optional>
#include <list>
#include <vector>
#include <string>
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
	class BufferAllocManager
	{
	public:
		/**
		* Default ctor: creates alloc manager with zero capacity.
		*/
		BufferAllocManager();

		UINT GetNumSlots() const;
		UINT GetNumOccupiedSlots() const;
		UINT GetNumFreeSlots() const;
		UINT GetNumAllocs() const;

		/**
		* Tries to reset capacity to the given value.
		* @Returns: false if capacity was not changed to the given value;
		*/
		bool TryResetCapacity(UINT InNumSlots);

		/**
		* If unable to allocate returns std::nullopt.
		*/
		std::optional<BufferAlloc> NewAlloc(UINT InLengthInSlots);
		void FreeAlloc(BufferAlloc& InAlloc);

		/**
		* @Returns: 
		* - false if unable to reallocate; At this case the alloc is kept unchanged; 
		*/
		bool TryReAlloc(BufferAlloc& InAlloc, UINT InLengthInSlots);

		void Clear();

	private:
		using AllocList = std::list<BufferAlloc>;
		using FreeList = std::list<UINT>;
		struct SlotState
		{
			AllocList::iterator GetAllocIt() const { return AllocIt.value(); }
			FreeList::iterator GetFreeIt() const { return FreeIt.value(); }

			bool IsOccupied() const { return AllocIt.has_value(); }
			bool IsFree() const { return FreeIt.has_value(); }

			void MakeFree(FreeList::iterator InIt);
			void MakeOccupied(AllocList::iterator InIt);

		private:
			std::optional<AllocList::iterator> AllocIt;
			std::optional<FreeList::iterator> FreeIt;
		};

		std::optional<BufferSlotIndex> FindFreeSlots(UINT InLengthInSlots);

		/**
		* Updates the slots and free slot indices, but does NOT update the allocs.
		*/
		void MakeSlotsOccupied(BufferSlotIndex InIndex, UINT InLengthInSlots, AllocList::iterator InAllocIt);

		/**
		* Updates the slots and free slot indices, but does NOT update the allocs.
		*/
		void MakeSlotsFree(BufferSlotIndex InIndex, UINT InLengthInSlots);
		bool IsFreeOfLength(BufferSlotIndex InIndex, UINT InLengthInSlots) const;
		bool IsOccupiedSlot(BufferSlotIndex InIndex) const;
		bool IsFreeSlot(BufferSlotIndex InIndex) const;
		template<class T, class ThisType>
		T* GetSlotByIndex(BufferSlotIndex InIndex, ThisType pThis) const 
		{
			BOOST_ASSERT_MSG(InIndex < pThis->Slots.size(), "BufferAllocManager::GetSlotByIndex: index is out of range"); 
			return &(pThis->Slots[InIndex]);
		}
		void DoFreeAlloc(AllocList::iterator InAllocIt);

		std::vector<SlotState> Slots;
		FreeList FreeSlotIndices;
		AllocList Allocs;
	};
} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv