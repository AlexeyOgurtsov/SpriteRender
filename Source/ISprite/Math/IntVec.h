#pragma once

#include <string>

namespace Dv
{
namespace Spr
{
namespace Math
{
	
	struct IntVec
	{
		int X = 0; 
		int Y = 0;

		IntVec() = default;
		IntVec(int InX, int InY)
		:	X { InX }
		,	Y { InY } {}
	};

	inline bool operator==(const IntVec& A, const IntVec& B)
	{
		return A.X == B.X && A.Y == B.Y;	
	}
	inline bool operator!=(const IntVec& A, const IntVec& B) { return ! (A == B); }
	
	inline std::string ToString(const IntVec& InValue)
	{
		std::string s;
		s.append("{");
		s.append(std::to_string(InValue.X));
		s.append(";");
		s.append(std::to_string(InValue.Y));
		s.append("}");
		return s;
	}

	template<class Strm>
	Strm& operator<<(Strm& S, const IntVec& InValue)
	{
		return S << ToString(InValue);
	}
} // Dv::Spr::Math
} // Dv::Spr
} // Dv