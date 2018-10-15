#include "MathUtils.h"

#include "Size.h"
#include "Vec2.h"

namespace Dv::Spr::Math
{
	SVec2 GetCenter(const SVec2& InCenter, const SSize& InSize)
	{
		return SVec2(InCenter.X + InSize.Width / 2.0F, InCenter.Y + InSize.Height / 2.0F);
	}
} // Dv::Spr::Math