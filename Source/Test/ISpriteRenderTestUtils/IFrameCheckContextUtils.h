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
#include "SprRenHelper/SprRenHelper_Types.h"
#include "IMPL/Utils/TextureElement.h"

namespace Test
{
	class IFrameCheckContext;

	/**
	* Returns render target texel coordinates for the given canvas rect, using the given factors.
	*/
	IntVec RT_TexelCoordByFactor(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InFactor);

	/**
	* Returns vector of factors in range [0..1] from the point given in the coordinate system of the canvas.
	*/
	MySprMath::SVec2 GetCanvasFactorCoord(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InCoord);

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
	* Checks that the given canvas point contains valid result of blending source and dest blend colors using the given blending op.
	*/
	bool RT_TexelBlendedProperly(IFrameCheckContext* pContext, CanvasHandle InCanvas, const SprVec2& InCanvasPoint, const TexelColor& InSourceColor, const TexelColor& InDestColor, EBlendOp InOp);

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
	* Returns true if sprite is blended properly as the given color.
	*/
	bool RT_SpriteBlendedProperly(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const TexelColor& InSourceColor, const TexelColor& InDestColor, EBlendOp InOp);	

	/**
	* Returns true if sprite is not visible.
	*/
	bool RT_SpriteHidden(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, bool bMatchAlpha);

	/**
	* Returns true if sprite is not visible.
	*
	* Canvas point is specified in the canvas coord system (NOT fract!).
	*/
	bool RT_SpriteHidden_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const SprVec2& InSpriteLBPosition_AsCanvasPoint, bool bMatchAlpha);

	/**
	* Returns pick result for for the given sprite returned by the pick request.
	*/
	MySprRen::SPickResult PickSprite(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite);

	/**
	* Returns pick result for the given sprite returned by the pick request at the given canvas point.
	*
	* Canvas point is specified in the canvas coord system (NOT fract!).
	*/
	MySprRen::SPickResult PickSprite_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const SprVec2& InSpriteLBPosition_AsCanvasPoint);

	/**
	* Returns true if sprite is picked at the given canvas point.
	*/
	bool SpritePickedAs(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, MySpr::PickObjectId InObjId);

	/**
	* Returns true if sprite is picked at the given canvas point.
	*
	* Canvas point is specified in the canvas coord system (NOT fract!).
	*/
	bool SpritePickedAs_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, MySpr::PickObjectId InObjId, const SprVec2& InSpriteLBPosition_AsCanvasPoint);

	/**
	* Returns true if sprite is not picked.
	*/
	bool SpriteNotPicked(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite);

	/**
	* Returns true if sprite is not picked at the given position.
	*
	* Canvas point is specified in the canvas coord system (NOT fract!).
	*/
	bool SpriteNotPickedAt(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const SprVec2& InSpriteLBPosition_AsCanvasPoint);

	/**
	* Returns true if render target texel with the given coordinates matches another one.
	*/
	bool RT_TexelMatches(IFrameCheckContext* pContext, UINT InRow, UINT InX, const void* pInRefTexel, DXGI_FORMAT InRefFormat);
} // Test