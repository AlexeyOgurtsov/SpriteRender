#pragma once

#include <limits>
#include <string>

namespace Dv
{
namespace Spr
{

using SpriteCanvasId = int;

constexpr SpriteCanvasId NULL_SPRITE_CANVAS_ID = -1;
constexpr SpriteCanvasId MAX_SPRITE_CANVAS_ID = (std::numeric_limits<SpriteCanvasId>::max)();

constexpr bool IsValidCanvasId(SpriteCanvasId Id) { return (Id > NULL_SPRITE_CANVAS_ID) && (Id <= MAX_SPRITE_CANVAS_ID); }

inline std::string GetSpriteCanvasIdValidityStr(SpriteCanvasId Id)
{
	return IsValidCanvasId(Id) ? "Valid" : "Special";
}

} // Dv::Spr
} // Dv