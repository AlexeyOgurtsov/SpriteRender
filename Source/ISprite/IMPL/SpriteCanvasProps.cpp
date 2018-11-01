#include "../SpriteCanvasProps.h"
#include "../Math/MathUtils.h"
#include "boost/assert.hpp"

namespace Dv
{
namespace Spr
{ 
	bool SCanvasPickProps::CanBePicked() const
	{
		return Mode != ECanvasPickMode::Disabled;
	}

	Math::IntVec SCanvasRect::PointAt(float InAlphaX, float InAlphaY) const
	{
		return Math::IntVec { Math::ClampLerp(Left, (GetRight() - 1), InAlphaX),  Math::ClampLerp(GetBottom()-1, Top, InAlphaY) };
	}

	float SCanvasRect::GetAspectWidthOverHeight() const
	{
		return static_cast<float>(Width)/Height;
	}

	SCanvasCoordSystem::SCanvasCoordSystem(const Math::SVec2& InCenter, float InHalfHeight) :
		Center{InCenter}
	,	HalfHeight{InHalfHeight} 
	{
		BOOST_ASSERT_MSG(InHalfHeight > 0, "SCanvasCoordSystem: ctor: Half height must be greater than zero!");
	}

	float SCanvasCoordSystem::GetHalfWidth(float InAspectWidthOverHeight) const
	{
		return HalfHeight * InAspectWidthOverHeight;
	}

	float SCanvasCoordSystem::GetLeftX(float InAspectWidthOverHeight) const
	{
		return Center.X - GetHalfWidth(InAspectWidthOverHeight);
	}

	float SCanvasCoordSystem::GetRightX(float InAspectWidthOverHeight) const
	{
		return Center.X + GetHalfWidth(InAspectWidthOverHeight);
	}

	float SCanvasCoordSystem::GetBottomY() const
	{
		return Center.Y - HalfHeight;
	}

	float SCanvasCoordSystem::GetTopY() const
	{
		return Center.Y + HalfHeight;
	}

	const SCanvasPickProps SCanvasPickProps::Disabled { ECanvasPickMode::Disabled };
	const SCanvasPickProps SCanvasPickProps::Enabled { ECanvasPickMode::Enabled };
} // Dv::Spr
} // Dv