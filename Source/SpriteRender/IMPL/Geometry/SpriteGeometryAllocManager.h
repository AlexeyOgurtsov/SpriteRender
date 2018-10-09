#pragma once

#include "SpriteGeometryAllocManagerList.h"
#include "SpriteGeometry.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

/**
* Allocation that is reserved, but not necessarily yet done
*/
struct SReservedGeometryAlloc
{
	SpriteGeometryAllocList::iterator it_Alloc;

	SReservedGeometryAlloc() = default;
	SReservedGeometryAlloc(SpriteGeometryAllocList::iterator InItAlloc) :
		it_Alloc(InItAlloc) {}
};

class SpriteGeometryAllocManager
{
public:
	SpriteGeometryAllocManager();
	~SpriteGeometryAllocManager();

	/**
	* Capacity in sprites.
	*/
	int GetCapacity() const;

	/**
	* Count of stored sprites.
	*/
	int GetNumSprites() const;

	/**
	* Sets a new capacity.
	* WARNING!!! None of the already existing elements should be beyond the new capacity.
	* All occupied elements remain on the same place.
	*/
	void SetNewCapacity(int InNewCapacity);

	/**
	* Gets count of element that can be inserted without an overflow
	*/
	int GetSlack() const;

	/**
	* Reserve allocation for geometry.
	*/
	SReservedGeometryAlloc Reserve();

	/**
	* Allocate reserved for allocation
	*/
	void Allocate(const SReservedGeometryAlloc& InAlloc, SpriteGeometry* pInSpriteGeometry);

	/**
	* Mark the given sprite geometry as waiting for to be deleted
	*/
	void MarkForDelete(SpriteGeometry* pInSpriteGeometry);

	/**
	* Delete all allocations that are pending delete.
	*/
	void DeleteAllPending();

private:
	SpriteGeometryAllocList _allocs;
	SpriteGeometryFreeIndexList _freeIndices;
	int _capacity;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv
