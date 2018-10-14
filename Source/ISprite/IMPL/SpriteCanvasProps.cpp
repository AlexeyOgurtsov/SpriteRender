#include "../SpriteCanvasProps.h"
#include "../Math/MathUtils.h"

namespace Dv
{
namespace Spr
{ 
	Math::IntVec SCanvasRect::PointAt(float InAlphaX, float InAlphaY) const
	{
		return Math::IntVec { Math::Lerp(Left, GetRight(), InAlphaX),  Math::Lerp(GetBottom(), Top, InAlphaY) };
	}
} // Dv::Spr
} // Dv