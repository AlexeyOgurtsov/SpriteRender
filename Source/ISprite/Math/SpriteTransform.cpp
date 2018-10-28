#include "SpriteTransform.h"
#include "MathUtils.h"

namespace Dv
{
namespace Spr
{
namespace Math
{
	bool AreTransformsNearlyEqual(const SSpriteTransform& InLeft, const SSpriteTransform& InRight, float InVectorTolerance, float InRotationTolerance)
	{
		return AreNearlyEqual(InLeft.Position, InRight.Position, InVectorTolerance)
			&& AreNearlyEqual(ClampAngleDegs(InLeft.AngleDegs), ClampAngleDegs(InRight.AngleDegs), InRotationTolerance)
			&& AreNearlyEqual(InLeft.Origin, InRight.Origin, InVectorTolerance);
	}

	bool operator==(const SSpriteTransform& InLeft, const SSpriteTransform& InRight)
	{
		return AreTransformsNearlyEqual(InLeft, InRight, ZERO_TOLERANCE, ZERO_TOLERANCE);
	}

	bool operator!=(const SSpriteTransform& InLeft, const SSpriteTransform& InRight)
	{
		return !operator==(InLeft, InRight);
	}


} // Dv::Spr::Math
} // Dv::Spr
} // Dv