#pragma once

namespace Dv
{
namespace Spr
{
namespace Math
{

struct SVec2
{
	float X, Y;

	SVec2() :
		X(0.0F), Y(0.0F) {}

	SVec2(float InX, float InY) :
		X(InX), Y(InY) {}


	SVec2 operator+(const SVec2& InOther)
	{
		return SVec2{X + InOther.X, Y + InOther.Y};
	}

	SVec2 operator-(const SVec2& InOther)
	{
		return SVec2{X - InOther.X, Y - InOther.Y};
	}

	SVec2 operator*(float InCoeff)
	{
		return SVec2{X * InCoeff, Y * InCoeff};
	}

	SVec2 operator/(float InCoeff)
	{
		return SVec2{X / InCoeff, Y / InCoeff};
	}

	SVec2& operator*=(float InCoeff)
	{
		X *= InCoeff;
		Y *= InCoeff;
		return *this;
	}

	SVec2& operator/=(float InCoeff)
	{
		X /= InCoeff;
		Y /= InCoeff;
		return *this;
	}

	SVec2& operator+=(const SVec2& InOther)
	{
		X += InOther.X;
		Y += InOther.Y;
		return *this;
	}
	SVec2& operator-=(const SVec2& InOther)
	{
		X -= InOther.X;
		Y -= InOther.Y;
		return *this;
	}
};

bool operator==(const SVec2& InLeft, const SVec2& InRight);
bool operator!=(const SVec2& InLeft, const SVec2& InRight);

inline bool operator==(const SVec2& InLeft, const SVec2& InRight)
{
	return InLeft.X == InRight.X && InLeft.Y == InRight.Y;
}

inline bool operator!=(const SVec2& InLeft, const SVec2& InRight)
{
	return !operator==(InLeft, InRight);
}

} // Dv::Spr::Math
} // Dv::Spr
} // Dv