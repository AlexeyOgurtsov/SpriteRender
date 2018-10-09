#pragma once

namespace Dv
{
namespace Spr
{
namespace Math
{

struct SSize
{
	float Width, Height;

	SSize() :
		Width(0.0F)
	,	Height(0.0F) {}
	SSize(float InWidth, float InHeight) :
		Width(InWidth)
	,	Height(InHeight) {}
};
bool operator!=(const SSize& InLeft, const SSize& InRight);
bool operator==(const SSize& InLeft, const SSize& InRight);

inline bool operator!=(const SSize& InLeft, const SSize& InRight)
{
	return ! operator==(InLeft, InRight);
}

inline bool operator==(const SSize& InLeft, const SSize& InRight)
{
	return InLeft.Width == InRight.Width && InLeft.Height == InRight.Height;
}

} // Dv::Spr::Math
} // Dv::Spr
} // Dv