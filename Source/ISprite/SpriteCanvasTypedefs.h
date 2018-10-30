#pragma once

#include <limits>

namespace Dv
{
namespace Spr
{

using SpriteCanvasId = int;

constexpr SpriteCanvasId NULL_SPRITE_CANVAS_ID = -1;
constexpr SpriteCanvasId MAX_SPRITE_CANVAS_ID = (std::numeric_limits<SpriteCanvasId>::max)();

constexpr bool IsValidCanvasId(SpriteCanvasId Id) { return (Id > NULL_SPRITE_CANVAS_ID) && (Id <= MAX_SPRITE_CANVAS_ID); }

} // Dv::Spr
} // Dv