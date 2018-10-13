#include "MathUtils.h"

namespace Test
{
	float Lerp(float A, float B, float Factor)
	{
		return A + (B - A) * Factor;
	}
} // Test