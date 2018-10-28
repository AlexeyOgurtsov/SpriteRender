#pragma once

namespace Dv::Spr::Math
{
	constexpr float EPS = 0.00001F;
	constexpr float ZERO_TOLERANCE = 0.0F;

	template<class T>
	bool AreNearlyEqual(T A, T B, T InEps = static_cast<T>(EPS))
	{
		return -InEps <= (A - B) && (A - B) <= InEps;
	}

	template<class T>
	T Lerp(T A, T B, float Factor)
	{
		return A + static_cast<T>((B - A) * Factor);
	}

	/**
	* Clamp
	*/
	template<class T>
	T Clamp(T A, T B, T C)
	{
		if (C <= A) { return A; }
		if (C >= B) { return B; }
		return C;
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
		if (InLerpResult >= B) { return 1.0F; }
		return (InLerpResult - A) / (B - A);
	}
} // Dv::Spr::Math