#pragma once

namespace Dv
{
namespace Spr
{

using SpriteId = int;

constexpr SpriteId NULL_SPRITE_ID = -1;

constexpr bool IsValidSpriteId(SpriteId Id) { return Id > NULL_SPRITE_ID; }

} // Dv::Spr
} // Dv