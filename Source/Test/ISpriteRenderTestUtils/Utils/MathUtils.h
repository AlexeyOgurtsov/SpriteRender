#pragma once

#include "../SprRenHelper/SprRenHelper_MathTypes.h"
#include <cmath>
#include <string>
#include <iomanip>

namespace Test
{
	using Vec2 = SprVec2;

	/**
	* Interpolates between A and B with the given Factor.
	*/
	float Lerp(float A, float B, float Factor);

	/**
	* Screen coord system description in 2D.
	*/
	struct ScreenCoordSystemDesc
	{
		Vec2 Center;
		float LeftX = -1.0F;
		float RightX = 1.0F;
		float TopY = 1.0F;
		float BottomY = -1.0F;

		float GetWidth() const { return fabs(RightX - LeftX); }
		float GetHeight() const { return fabs(TopY - BottomY); }

		/**
		* Returns point at the given system by interpolating X and Y between boundaries.
		* So that:
		* - Point {0, 0} is at LeftX;
		* - Point {1, 0} at RightX;
		* - Point {0, 0} is at BottomY;
		* - Point {0, 1} at TopY;
		*/
		Vec2 PointAt(float AlphaX, float AlphaY) const
		{
			return Vec2{ Lerp(LeftX, RightX, AlphaX), Lerp(BottomY, TopY, AlphaY) };
		}

		ScreenCoordSystemDesc() = default;

		/**
		* Creates down-to-up, left-to-right coord system.
		*/
		ScreenCoordSystemDesc(const Vec2& InCenter, float InHalfWidth, float InHalfHeight) :
			Center{ InCenter }
		,	LeftX{ InCenter.X - InHalfWidth }
		,	RightX{ InCenter.X + InHalfWidth }
		,	BottomY{ InCenter.Y - InHalfHeight }
		,	TopY{ InCenter.Y + InHalfHeight } {}
	};
;

	template<class Strm> Strm& operator<<(Strm& S, const ScreenCoordSystemDesc InDesc)
	{
		S << "Center: " << ToString(InDesc.Center) << std::endl;
		S << "(LeftX, RightX)=" << InDesc.LeftX << ";" << InDesc.RightX << std::endl;		
		S << "(BottomY, TopY)=" << InDesc.BottomY << ";" << InDesc.TopY << std::endl;
		return S;
	}
} // Test