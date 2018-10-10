#pragma once

#include "Vec2.h"

namespace Dv
{
namespace Spr
{
namespace Math
{

struct SSpriteTransform
{
	SVec2 Position;

	SSpriteTransform() : Position(0.0F, 0.0F) {}
	explicit SSpriteTransform(const SVec2& InPosition) :
		Position(InPosition) {} 
};
bool operator==(const SSpriteTransform& InLeft, const SSpriteTransform& InRight);
bool operator!=(const SSpriteTransform& InLeft, const SSpriteTransform& InRight);

inline bool operator==(const SSpriteTransform& InLeft, const SSpriteTransform& InRight)
{
	return InLeft.Position == InRight.Position;
}

inline bool operator!=(const SSpriteTransform& InLeft, const SSpriteTransform& InRight)
{
	return ! operator==(InLeft, InRight);
}

} // Dv::Spr::Math
} // Dv::Spr
} // Dv