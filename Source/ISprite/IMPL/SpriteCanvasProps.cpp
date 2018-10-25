#include "../SpriteCanvasProps.h"
#include "../Math/MathUtils.h"

namespace Dv
{
namespace Spr
{ 
	Math::IntVec SCanvasRect::PointAt(float InAlphaX, float InAlphaY) const
	{
		return Math::IntVec { Math::ClampLerp(Left, (GetRight() - 1), InAlphaX),  Math::ClampLerp(GetBottom()-1, Top, InAlphaY) };
	}
} // Dv::Spr
} // Dv