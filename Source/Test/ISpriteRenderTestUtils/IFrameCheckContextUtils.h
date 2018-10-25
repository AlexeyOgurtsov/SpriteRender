#pragma once

/**
* Utilities for working with frame resources (like checking pixels of RT etc.).
*
* All main frame checker functions are to be placed here.
* DO May depend on SpriteHelpers!
*/

#include <d3d11.h>
#include "Utils/TexelColor.h"
#include "Utils/MathUtils.h"
#include "SprRenHelper/SprRenHelper_Canvas.h"
#include "SprRenHelper/SprRenHelper_Sprite.h"
#include "IMPL/Utils/TextureElement.h"

namespace Test
{
	class IFrameCheckContext;

	/**
	* Returns render target texel coordinates for the given canvas rect, using the given factors.
	*/
	IntVec RT_TexelCoordByFactor(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InFactor);

	/**
	* Returns render target texel coordinates for the given canvas rect, 
	* using point in the COORDINATE SYSTEM OF CANVAS.
	*/
	IntVec RT_TexelCoordByCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InCanvasPoint);

	/**
	* Returns render target color by canvas point.
	*/
	TexelColor RT_ColorByCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InCanvasPoint);

	/**
	* Checks that RT color matches canvas at the given point.
	*/
	bool RT_ColorMatchesCanvas_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const SprVec2& InCanvasPoint, const TexelColor& InColor, bool bMatchAlpha);

	/**
	* Is canvas at the given point is set to the clear color
	*/
	bool RT_IsCanvasClear_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InCanvasPoint, bool bMatchAlpha);

	/**
	* Returns true, if render target matches texture color.
	*/
	bool RT_TextureMatches_CanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const SprVec2& InCanvasPoint, const IMPL::TextureElement& InTextureElement, bool bMatchAlpha);

	/**
	* Returns true if render target is filled entirely with the given color.
	*/
	bool RT_FilledWithColor(IFrameCheckContext* pContext, const TexelColor& InColor, bool bMatchAlpha);

	/**
	* Returns true if render target is filled entirely with the given color.
	*/
	bool RT_Cleared(IFrameCheckContext* pContext, bool bMatchAlpha);

	/**
	* Returns true if sprite is visible as the given color.
	*/
	bool RT_SpriteVisibleAsColor(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const TexelColor& InColor, bool bMatchAlpha);

	/**
	* Returns true if sprite is not visible.
	*/
	bool RT_SpriteHidden(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, bool bMatchAlpha);

	/**
	* Returns true if sprite is not visible.
	*/
	bool RT_SpriteHidden_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const SprVec2& InSpritePosition_AsCanvasPoint, bool bMatchAlpha);

	/**
	* Returns true if render target texel with the given coordinates matches another one.
	*/
	bool RT_TexelMatches(IFrameCheckContext* pContext, UINT InRow, UINT InX, const void* pInRefTexel, DXGI_FORMAT InRefFormat);
} // Test