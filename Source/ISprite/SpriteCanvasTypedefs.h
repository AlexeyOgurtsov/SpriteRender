#pragma once

namespace Dv
{
namespace Spr
{

using SpriteCanvasId = int;

constexpr SpriteCanvasId NULL_SPRITE_CANVAS_ID = -1;

constexpr bool IsValidCanvasId(SpriteCanvasId Id) { return Id > NULL_SPRITE_CANVAS_ID; }

} // Dv::Spr
} // Dv