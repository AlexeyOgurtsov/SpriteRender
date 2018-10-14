#pragma once

#include "ISprite/Math/IntVec.h"

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

	SSpriteCanvasProps() = default;
	explicit SSpriteCanvasProps(const SCanvasRect& InRTRect) :
		RTRect(InRTRect) {}
};

} // Dv::Spr
} // Dv