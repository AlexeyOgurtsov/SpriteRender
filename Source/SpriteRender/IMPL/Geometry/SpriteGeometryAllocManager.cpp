#include "SpriteGeometryAllocManager.h"
#include "SpriteGeometry.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{
	SpriteGeometryAllocManager::SpriteGeometryAllocManager() :
		_capacity(0)
	{
		// Fill with free indices
	}

	SpriteGeometryAllocManager::~SpriteGeometryAllocManager()
	{
		// default implementation
	}

	void SpriteGeometryAllocManager::SetNewCapacity(int InNewCapacity)
	{
		if(InNewCapacity > _capacity)
		{
			for(int extraFreeSlotIndex = _capacity; extraFreeSlotIndex < InNewCapacity; extraFreeSlotIndex++)
			{
				_freeIndices.push_back(extraFreeSlotIndex);
			}
		}
		else
		{
			for(auto it_FreeSlot = _freeIndices.begin(); it_FreeSlot != _freeIndices.end(); ++it_FreeSlot)
			{
				_freeIndices.erase(it_FreeSlot);
			}
		}
		_capacity = InNewCapacity;
	}

	int SpriteGeometryAllocManager::GetSlack() const
	{
		return _freeIndices.size();
	}

	int SpriteGeometryAllocManager::GetCapacity() const
	{
		return _capacity;
	}

	int SpriteGeometryAllocManager::GetNumSprites() const
	{
		return _allocs.size();
	}
	SReservedGeometryAlloc SpriteGeometryAllocManager::Reserve()
	{
		SpriteGeometryIndex newAllocIndex = _freeIndices.front();
		_freeIndices.pop_front();
		auto it_alloc = _allocs.emplace(_allocs.end(), newAllocIndex);
		return SReservedGeometryAlloc{it_alloc};
	}

	void SpriteGeometryAllocManager::Allocate(const SReservedGeometryAlloc& InAlloc, SpriteGeometry* pInSpriteGeometry)
	{
		BOOST_ASSERT(pInSpriteGeometry);
		pInSpriteGeometry->GetItAlloc()->State = ESpriteGeometryAllocState::Allocated;
		pInSpriteGeometry->GetItAlloc()->pSpriteGeometry.reset(pInSpriteGeometry);
		
	}

	void SpriteGeometryAllocManager::MarkForDelete(SpriteGeometry* pInSpriteGeometry)
	{
		BOOST_ASSERT(pInSpriteGeometry);
		BOOST_ASSERT_MSG(ESpriteGeometryAllocState::PendingDelete != pInSpriteGeometry->GetItAlloc()->State, "SpriteGeometryAllocManager::MarkForDelete: not allowed to mark the same sprite geometry deleted twice");
		pInSpriteGeometry->GetItAlloc()->State = ESpriteGeometryAllocState::PendingDelete;
	}

	void SpriteGeometryAllocManager::DeleteAllPending()
	{
		for(auto it_alloc = _allocs.begin(); it_alloc != _allocs.end(); )
		{
			auto next_it_alloc = std::next(it_alloc);

			_freeIndices.emplace_back(it_alloc->Index);
			_allocs.erase(it_alloc);

			it_alloc = next_it_alloc;
		}
	}

	SSpriteGeometryAlloc::~SSpriteGeometryAlloc()
	{
		// default implementation
	}
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv