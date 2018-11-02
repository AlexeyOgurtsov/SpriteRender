#pragma once

#include "ISprite/Math/IntVec.h"
#include "ISprite/Math/Vec2.h"
#include <iomanip>
#include <string>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{ 

/**
* Should picking feature be used for the sprites of the canvas?
*/
enum class ECanvasPickMode
{
	Disabled = 0,
	Enabled,
	Num
};
constexpr ECanvasPickMode DEFAULT_CANVAS_PICK_MODE = ECanvasPickMode::Disabled;

inline std::string ToString(ECanvasPickMode InMode)
{
	switch (InMode)
	{
	case ECanvasPickMode::Disabled:
		return std::string("Disabled");

	case ECanvasPickMode::Enabled:
		return std::string("Enabled");

	default:
		break;
	}
	BOOST_ASSERT_MSG(false, "ToString(ECanvasPickMode): Unknown constant value or not yet implemented");
	return std::string("{SHOULD_NOT_GET_HERE}");
}

template<class Strm>
Strm& operator<<(Strm& S, ECanvasPickMode InMode)
{
	return S << ToString(InMode);
}

struct SCanvasPickProps
{
	ECanvasPickMode Mode;

	bool CanBePicked() const;

	inline SCanvasPickProps() :
		SCanvasPickProps{ DEFAULT_CANVAS_PICK_MODE } {}
	inline SCanvasPickProps(ECanvasPickMode InMode) :
		Mode{ InMode }
	{
		BOOST_ASSERT_MSG(static_cast<int>(InMode) < static_cast<int>(ECanvasPickMode::Num), "SCanvasPickProps: wrong pick mode");
	}
	static const SCanvasPickProps Disabled;
	static const SCanvasPickProps Enabled;
};

template<class Strm>
Strm& operator<<(Strm& S, const SCanvasPickProps& InProps)
{
	return S << InProps.Mode << std::endl;
}

/**
* Rect that canvas takes on the render target.
*/
struct SCanvasRect
{
	/**
	* Width of the canvas in pixels of the render target.
	*/
	int Width;

	/**
	* Height of the canvas in pixels of the render target.
	*/
	int Height;

	/**
	* Left offset of the canvas in pixels of the render target.
	*/
	int Left;

	/**
	* Top offset of the canvas in pixels of the render target.
	*/
	int Top;

	int GetRight() const { return Left + Width; }
	int GetBottom() const { return Top + Height; }

	float GetAspectWidthOverHeight() const;

	/**
	* Returns point at the canvas by lerping between its corners on each of the axes.
	*
	* WARNING: Y: 0 corresponds to BOTTOM.
	*/
	Math::IntVec PointAt(float InAlphaX, float InAlphaY) const;

	/**
	* Checks, whether the properties of the canvas are correct.
	*/
	bool IsValid() const 
	{
		return Width > 0 && Height > 0;
	}

	SCanvasRect() :
		Width(0)
	,	Height(0)
	,	Left(0)
	,	Top(0) {}

	SCanvasRect(int InWidth, int InHeight) :
		Width(InWidth)
	,	Height(InHeight)
	,	Left(0)
	,	Top(0) {}

	SCanvasRect(int InWidth, int InHeight, int InLeft, int InTop) :
		Width(InWidth)
	,	Height(InHeight)
	,	Left(InLeft)
	,	Top(InTop) {}
};
template<class Strm>
Strm& operator<<(Strm& S, const SCanvasRect& InRect)
{
	S << "{Left;Top}=" << "{" << InRect.Left << ";" << InRect.GetTop()<< "}" 
	<< "; {Width*Height}=" << "{" << InRect.Width << "*" << InRect.Height << "}" << std::endl;
	return S;
}


/**
* Coord system where X and Y axes have the same scale.
*
* Y axis is always upward, X if always left-to-right;
*/
struct SCanvasCoordSystem
{
	Math::SVec2 Center;
	float HalfHeight;

	float GetHalfWidth(float InAspectWidthOverHeight) const;
	float GetLeftX(float InAspectWidthOverHeight) const;
	float GetRightX(float InAspectWidthOverHeight) const;
	float GetBottomY() const;
	float GetTopY() const;

	SCanvasCoordSystem() :
		Center{0.0F, 0.0F}
	,	HalfHeight{1.0F} {}

	SCanvasCoordSystem(const Math::SVec2& InCenter, float InHalfHeight);
};
template<class Strm>
Strm& operator<<(Strm& S, const SCanvasCoordSystem& InSystem)
{
	S << "{Center=" << InSystem.Center << "; HalfHeight=" << InSystem.HalfHeight << "}";
	return S;
}

/**
* All properties of the sprite canvas.
* Initially created to facilitate passing properties to the render.
*/
struct SSpriteCanvasProps
{
	/**
	* Rect of the render target that this canvas occupies.
	*/
	SCanvasRect RTRect;

	/**
	* Coord system.
	*/
	SCanvasCoordSystem CoordSystem;

	/**
	* Picking properties of the canvas.
	*/
	SCanvasPickProps Pick;

	SSpriteCanvasProps() = default;
	explicit SSpriteCanvasProps
	(
		const SCanvasRect& InRTRect, const SCanvasCoordSystem& InCoordSystem,
		const SCanvasPickProps& InPick = SCanvasPickProps{}
	) :
		RTRect{InRTRect}
	,	CoordSystem{InCoordSystem}
	,	Pick(InPick) {}
};

template<class Strm>
Strm& operator<<(Strm& S, const SSpriteCanvasProps& InProps)
{
	S << "RTRect: " << InProps.RTRect << std::endl;
	S << "CoordSystem: " << InProps.CoordSystem << std::endl;
	return S;
}

} // Dv::Spr
} // Dv