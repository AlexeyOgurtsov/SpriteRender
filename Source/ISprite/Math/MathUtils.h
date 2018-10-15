#pragma once

#include "MathUtilsMinimal.h"
#include "MathMinimal.h"
#include "Size.h"
#include "IntVec.h"
#include "SpriteTransform.h"

namespace Dv::Spr::Math
{
	SVec2 GetCenter(const SVec2& InCenter, const SSize& InSize);
} // Dv::Spr