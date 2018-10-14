#include "MathUtils.h"

namespace Test
{
	float Lerp(float A, float B, float Factor)
	{
		return A + (B - A) * Factor;
	}

	float Factor(float A, float B, float InLerpResult)
	{
		return (InLerpResult-A)/(B-A);
	}
} // Test