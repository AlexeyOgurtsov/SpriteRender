#pragma once

namespace Dv::Spr::Math
{
	template<class T>
	T Lerp(T A, T B, float Factor)
	{
		return A + static_cast<T>((B - A) * Factor);
	}

	template<class T>
	T ClampLerp(T A, T B, float Factor)
	{
		if (Factor <= 0.0F) { return A; }
		if (Factor >= 1.0F) { return B; }
		return Lerp(A, B, Factor);
	}

	template<class T>
	float LerpFactor(T A, T B, float InLerpResult)
	{
		return (InLerpResult - A) / (B - A);
	}

	template<class T>
	float ClampLerpFactor(T A, T B, float InLerpResult)
	{
		if (InLerpResult <= A) { return 0.0F; }
		if (InLerpResult <= B) { return 1.0F; }
		return (InLerpResult - A) / (B - A);
	}
} // Dv::Spr::Math