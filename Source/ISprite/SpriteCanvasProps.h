#pragma once

#include "ISprite/Math/IntVec.h"
#include "ISprite/Math/Vec2.h"
#include <iomanip>

namespace Dv
{
namespace Spr
{ 

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

	SSpriteCanvasProps() = default;
	explicit SSpriteCanvasProps(const SCanvasRect& InRTRect, const SCanvasCoordSystem& InCoordSystem) :
		RTRect{InRTRect}
	,	CoordSystem{InCoordSystem} {}
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