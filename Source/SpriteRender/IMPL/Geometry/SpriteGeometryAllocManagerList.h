#pragma once

#include "SpriteGeometryTypedefs.h"

#include <memory>
#include <list>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class SpriteGeometry;

enum class ESpriteGeometryAllocState
{
	Reseved,

	Allocated,

	/**
	* We are about to delete this geometry sometime.
	* However, we must keep it in the buffer yet,
	* and we may not override this geometry with another one.
	* so it must remain in the allocation list.
	*/
	PendingDelete
};

/**
* Represents a geometry, allocated in the buffer.
*/
struct SSpriteGeometryAlloc
{
	std::unique_ptr<SpriteGeometry> pSpriteGeometry;
	
	/**
	* Sprite geometry index
	*/
	SpriteGeometryIndex Index;

	/**
	* State of geometry allocation
	*/
	ESpriteGeometryAllocState State;
	
	~SSpriteGeometryAlloc();
	SSpriteGeometryAlloc(SpriteGeometryIndex InIndex) :
		Index(InIndex)
	,	State(ESpriteGeometryAllocState::Reseved) {}
};

using SpriteGeometryAllocList = std::list<SSpriteGeometryAlloc>;

/**
* List of indices to the slots in the buffer that are free
*/
using SpriteGeometryFreeIndexList = std::list<int>;


} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv
