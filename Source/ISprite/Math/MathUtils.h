#pragma once

#include "MathUtilsMinimal.h"
#include "MathMinimal.h"
#include "Size.h"
#include "IntVec.h"
#include "SpriteTransform.h"

namespace Dv::Spr::Math
{
	constexpr float M_PI = 3.1415926535897F;

	/**
	* Returns center of rectangle with the given left bottom position and size.
	*/
	SVec2 GetRectCenter(const SVec2& InLeftBottom, const SSize& InSize);

	/**
	* Returns center of rotated rectangle, rotated around origin.
	* Origin is relative to the left bottom position of the rectangle.
	*/
	SVec2 GetCenterOfRotatedRect(const SVec2& InLeftBottom, const SSize& InSize, const SVec2& InOriginOffset, float InAngleDegs);

	/**
	* Rect vertex positions are outputted in the following order
	* 1) Top Left
	* 2) Top Right
	* 3) Bottom Left
	* 4) Bottom Right
	*
	* @param: InOriginOffset: offset vector from left bottom vertex to the origin.
	*/
	void CalculateRectVertices
	(
		SVec2 OutVertices[4],
		const SVec2& InLeftBottom, 
		const SSize& InSize, const SVec2& InOriginOffset, float InAngleDegs
	);

	/**
	* Clamp angle in degs to range between [0; 360)
	*/
	float ClampAngleDegs(float InAngleDegs);

	float DegreesToRads(float InAngleDegs);
	float RadsToDegrees(float InAngleRadians);
} // Dv::Spr