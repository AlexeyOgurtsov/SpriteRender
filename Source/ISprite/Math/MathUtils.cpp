#include "MathUtils.h"

#include "Size.h"
#include "Vec2.h"
#include <cmath>
#include <boost/assert.hpp>

namespace Dv::Spr::Math
{
	SVec2 GetRectCenter(const SVec2& InLeftBottom, const SSize& InSize)
	{
		return SVec2(InLeftBottom.X + InSize.Width / 2.0F, InLeftBottom.Y + InSize.Height / 2.0F);
	}

	void CalcRotatedBasis(SVec2 pOutRotatedBasis[2], float InAngleRadians)
	{
		pOutRotatedBasis[0] = SVec2{cosf(InAngleRadians), sinf(InAngleRadians)};
		pOutRotatedBasis[1] = SVec2{ -sinf(InAngleRadians), cosf(InAngleRadians) };
	}

	SVec2 RotatePoint_WithBasis(SVec2 InBasis[2], const SVec2& InPoint)
	{
		return (InBasis[0] * InPoint.X) + (InBasis[1] * InPoint.Y);
	}

	SVec2 RotatePoint_WithBasis(SVec2 InBasis[2], float InX, float InY)
	{
		return (InBasis[0] * InX) + (InBasis[1] * InY);
	}

	SVec2 TransformPoint(SVec2 InBasis[2], const SVec2& InOrigin, float InX, float InY)
	{
		return RotatePoint_WithBasis(InBasis, InX, InY) + InOrigin;
	}

	SVec2 GetCenterOfRotatedRect(const SVec2& InLeftBottom, const SSize& InSize, const SVec2& InOriginOffset, float InAngleDegs)
	{
		float const Angle = ClampAngleDegs(InAngleDegs);
		float const AngleRads = DegreesToRads(Angle);

		// Origin relative to the coordinate system that the rect is described in
		SVec2 const Origin = InLeftBottom + InOriginOffset;

		// Rotated basis in the coordinate system relative to origin.
		SVec2 RotatedBasis[2];
		CalcRotatedBasis(RotatedBasis, InAngleDegs);

		// New left bottom position
		SVec2 const NewLeftBottom = TransformPoint(RotatedBasis, Origin, (- InOriginOffset.X), (- InOriginOffset.Y));

		// Get center coordinate:
		return NewLeftBottom + (RotatedBasis[0] * InSize.Width / 2.0F) + (RotatedBasis[1] * InSize.Height / 2.0F);
	}

	void CalculateRectVertices
	(
		SVec2 OutVertices[4],
		const SVec2& InLeftBottom,
		const SSize& InSize, const SVec2& InOriginOffset, float InAngleDegs
	)
	{
		float const Angle = ClampAngleDegs(InAngleDegs);
		float const AngleRads = DegreesToRads(Angle);

		// Origin relative to the coordinate system that the rect is described in
		SVec2 const Origin = InLeftBottom + InOriginOffset;

		// Rotated basis in the coordinate system relative to origin.
		SVec2 RotatedBasis[2];
		CalcRotatedBasis(RotatedBasis, InAngleDegs);

		float const LeftX = (-InOriginOffset.X);
		float const RightX = (InSize.Width - InOriginOffset.X);
		float const TopY =  (InSize.Height - InOriginOffset.Y);
		float const BottomY = (-InOriginOffset.Y);
		OutVertices[0] = TransformPoint(RotatedBasis, Origin, LeftX, TopY);
		OutVertices[1] = TransformPoint(RotatedBasis, Origin, RightX, TopY);
		OutVertices[2] = TransformPoint(RotatedBasis, Origin, LeftX, BottomY);
		OutVertices[3] = TransformPoint(RotatedBasis, Origin, RightX, BottomY);
	}

	float ClampAngleDegs(float InAngleDegs)
	{
		float Clamped = fmod(InAngleDegs, 360.0F);
		if (Clamped < 0.0F)
		{
			Clamped = 360.0F - Clamped;
		}
		return Clamped;
	}

	float DegreesToRads(float InAngleDegs)
	{
		return (InAngleDegs / 180.0F) * M_PI;
	}

	float RadsToDegrees(float InAngleRadians)
	{
		return (InAngleRadians / M_PI) * 180;
	}
} // Dv::Spr::Math