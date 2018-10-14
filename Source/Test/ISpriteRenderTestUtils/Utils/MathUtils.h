#pragma once

#include "../SprRenHelper/SprRenHelper_MathTypes.h"
#include <cmath>
#include <string>
#include <iomanip>

namespace Test
{
	using Vec2 = SprVec2;
	using IntVec = SprIntVec;

	constexpr float HALF = 0.5F;
	constexpr float QUARTER = 0.25F;
	constexpr float EIGHTH = 0.125F;

	constexpr float CENTER = 0.5F;

	constexpr float LEFT = 0.0F;
	constexpr float RIGHT = 1.0F;
	constexpr float BOTTOM = 0.0F;
	constexpr float TOP = 1.0F;

	constexpr float QUARTER_FROM_LEFT = LEFT + QUARTER;
	constexpr float QUARTER_FROM_BOTTOM = BOTTOM + QUARTER;	

	constexpr float EIGHTH_FROM_LEFT = LEFT + EIGHTH;
	constexpr float EIGHTH_FROM_BOTTOM = BOTTOM + EIGHTH;

	/**
	* Interpolates between A and B with the given Factor.
	*/
	float Lerp(float A, float B, float Factor);

	/**
	* Opposite to Lerp: returns value, so that Lerp(A,B,Factor(A,B)) returns InLerpResult.
	*/
	float Factor(float A, float B, float InLerpResult);

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
		SprSize GetSize() const { return SprSize{GetWidth(), GetHeight()}; }

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

		/**
		* Opposite to PointAt: returns vector, where X and Y are factors in range 0..1.
		*/
		Vec2 PointFract(const Vec2& InPointAtResult) const
		{
			return Vec2{ Factor(LeftX, RightX, InPointAtResult.X), Factor(BottomY, TopY, InPointAtResult.Y) };
		}

		/**
		* Returns part of size of the screen.
		*/
		SprSize GetSizePart(float InWidthFactor, float InHeightFactor)
		{
			return SprSize(GetWidth() * InWidthFactor, GetHeight() * InHeightFactor);
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

	template<class Strm> Strm& operator<<(Strm& S, const ScreenCoordSystemDesc InDesc)
	{
		S << "Center: " << ToString(InDesc.Center) << std::endl;
		S << "(LeftX, RightX)=" << InDesc.LeftX << ";" << InDesc.RightX << std::endl;		
		S << "(BottomY, TopY)=" << InDesc.BottomY << ";" << InDesc.TopY << std::endl;
		return S;
	}
} // Test