#pragma once

#include "Vec2.h"
#include "MathUtilsMinimal.h"

namespace Dv
{
namespace Spr
{
namespace Math
{

	struct SSpriteTransform
	{
		/**
		* Position of the sprite Origin point (see the Origin).
		*/
		SVec2 Position;

		/**
		* Angle of degrees of rotation in the counter-clockwise order.
		*/
		float AngleDegs;

		/**
		* Origin of rotation and center of the sprite where the position points to.
		* Relative to the bottom-left point of the sprites.
		* By default is zero vector, which corresponds to the bottom-left corner itself;
		* Will be (Width/2;Height/2) for the center poitner of sprite> for example!!
		*/
		Math::SVec2 Origin;

		SSpriteTransform() : Position(0.0F, 0.0F), Origin{0.0F, 0.0F}, AngleDegs(0.0F) {}
		SSpriteTransform(const SVec2& InPosition, const Math::SVec2& InOrigin = { 0.0F, 0.0F }, float InAngleDegs = 0.0F) :
			Position{InPosition}, Origin{ InOrigin }, AngleDegs{ InAngleDegs } {}
};
bool operator==(const SSpriteTransform& InLeft, const SSpriteTransform& InRight);
bool operator!=(const SSpriteTransform& InLeft, const SSpriteTransform& InRight);
bool AreTransformsNearlyEqual(const SSpriteTransform& InLeft, const SSpriteTransform& InRight, float InVectorTolerance = EPS, float InRotationTolerance = EPS);


} // Dv::Spr::Math
} // Dv::Spr
} // Dv