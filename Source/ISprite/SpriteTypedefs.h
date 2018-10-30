#pragma once

#include <limits>

namespace Dv
{
namespace Spr
{

using SpriteId = int;

constexpr SpriteId NULL_SPRITE_ID = -1;
constexpr SpriteId MAX_SPRITE_ID = (std::numeric_limits<SpriteId>::max)();

constexpr bool IsValidSpriteId(SpriteId Id) { return (Id > NULL_SPRITE_ID) && (Id <= MAX_SPRITE_ID); }

} // Dv::Spr
} // Dv