#pragma once

#include <limits>
#include <string>
#include <cstdint>

namespace Dv
{
namespace Spr
{

using SpriteId = int;

constexpr SpriteId NULL_SPRITE_ID = -1;
constexpr SpriteId MAX_SPRITE_ID = (std::numeric_limits<SpriteId>::max)();

/**
* Returns true if the id can be used to identify the REAL object and NOT a special value.
*/
constexpr bool IsValidSpriteId(SpriteId Id) { return (Id > NULL_SPRITE_ID) && (Id <= MAX_SPRITE_ID); }

inline std::string GetSpriteIdValidityStr(SpriteId Id)
{
	return IsValidSpriteId(Id) ? "Valid" : "Special";
}

/**
* Id of the pickable object
*/
using PickObjectId = uint32_t;
static_assert(false == std::is_signed_v<PickObjectId>, "PickObjectId: must be unsigned type because typically be used as a render target format");

/**
* Object id to be passed to indicate value that is not associated with any object.
*
* We always used zero for this value because typically renders typically fill the pick buffer by zeroing.
*/
constexpr PickObjectId ZERO_PICK_OBJECT_ID = 0;

constexpr PickObjectId MINIMAL_PICK_OBJECT_ID = (ZERO_PICK_OBJECT_ID + 1);
constexpr PickObjectId MAX_PICK_OBJECT_ID = (std::numeric_limits<PickObjectId>::max)();

/**
* Returns true if the id can be used to identify the REAL object and NOT a special value.
*/
constexpr bool IsValidPickObjectId(PickObjectId Id) { return (Id >= MINIMAL_PICK_OBJECT_ID) && (Id <= MAX_PICK_OBJECT_ID); }

inline std::string GetPickObjectIdValidityStr(PickObjectId Id) 
{
	return IsValidPickObjectId(Id) ? "Valid" : "Special";
}

} // Dv::Spr
} // Dv