#pragma once

namespace Dv::Spr::Math
{
	template<class T>
	T Lerp(T A, T B, float Factor)
	{
		return A + (B - A) * static_cast<T>(Factor);
	}
} // Dv::Spr