#include "../IFrameCheckContextUtils.h"
#include "../IFrameCheckContext.h"
#include "../Utils/TestUtils.h"
#include "../Utils/MathUtils.h"
#include "../IMPL/Utils/RenTextureUtils.h"

namespace Test
{
	bool RT_TexelMatches(IFrameCheckContext* pContext, UINT InRow, UINT InX, const void* pInRefTexel, DXGI_FORMAT InRefFormat)
	{		
		BOOST_ASSERT(pContext);
		return IMPL::TexelMatches(pInRefTexel, InRefFormat, pContext->GetDevCon(), pContext->GetRT(), InRow, InX);		
	}

	IntVec RT_TexelCoordByFactor(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InFactor)
	{
		return InCanvas->GetRTRect().PointAt(InFactor.X, InFactor.Y);
	}
	
	IntVec RT_TexelCoordByCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InCanvasPoint)
	{
		ScreenCoordSystemDesc CoordSystem = InCanvas->GetCoordSystem();
		return RT_TexelCoordByFactor(pContext, InCanvas, CoordSystem.PointFract(InCanvasPoint));
	}

	TexelColor RT_ColorByCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InCanvasPoint)
	{
		IntVec const TexCoord = RT_TexelCoordByCanvasPoint(pContext, InCanvas, InCanvasPoint);
		return IMPL::GetTexelColorAt(pContext->GetDevCon(), pContext->GetRT(), TexCoord, /*MipLevel*/ 0);
	}

	bool RT_ColorMatchesCanvas_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const SprVec2& InCanvasPoint, const TexelColor& InColor, bool bMatchAlpha)
	{
		return AreTexelsMatch(InColor, RT_ColorByCanvasPoint(pContext, InCanvas, InCanvasPoint), bMatchAlpha);
	}

	bool RT_TexelBlendedProperly(IFrameCheckContext* pContext, CanvasHandle InCanvas, const SprVec2& InCanvasPoint, const TexelColor& InSourceColor, const TexelColor& InDestColor, EBlendOp InOp)
	{
		TexelColor ColorToCheck = RT_ColorByCanvasPoint(pContext, InCanvas, InCanvasPoint);
		return CheckAlphaBlendResult(ColorToCheck, InDestColor, InSourceColor, InOp);
	}

	bool RT_IsCanvasClear_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InCanvasPoint, bool bMatchAlpha)
	{
		return RT_ColorMatchesCanvas_AtCanvasPoint(pContext, InCanvas, InCanvasPoint, pContext->GetRTConfig().ClearColor, bMatchAlpha);
	}

	bool RT_TextureMatches_CanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const SprVec2& InCanvasPoint, const IMPL::TextureElement& InTextureElement, bool bMatchAlpha)
	{
		const IntVec Coord {0,0};
		const TexelColor TexColor = IMPL::GetTexelColorAt(pContext->GetDevCon(), InTextureElement.GetTexture(), Coord, /*Subresource=*/0);
		return  RT_ColorMatchesCanvas_AtCanvasPoint(pContext, InCanvas, InCanvasPoint, TexColor, bMatchAlpha);
	}

	bool RT_FilledWithColor(IFrameCheckContext* pContext, const TexelColor& InColor, bool bMatchAlpha)
	{
		return IMPL::TextureFilledWithColor(pContext->GetDevCon(), pContext->GetRT(), /*MipLevel*/ 0, InColor, bMatchAlpha);
	}

	bool RT_Cleared(IFrameCheckContext* pContext, bool bMatchAlpha)
	{
		return RT_FilledWithColor(pContext, pContext->GetRTConfig().ClearColor, bMatchAlpha);
	}

	bool RT_SpriteVisibleAsColor(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const TexelColor& InColor, bool bMatchAlpha)
	{
		SprVec2 const CanvasPoint = InSprite->GetCenter();
		return RT_ColorMatchesCanvas_AtCanvasPoint(pContext, InCanvas, CanvasPoint, InColor, bMatchAlpha);
	}

	bool RT_SpriteBlendedProperly(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const TexelColor& InSourceColor, const TexelColor& InDestColor, EBlendOp InOp)
	{
		SprVec2 const CanvasPoint = InSprite->GetCenter();
		return RT_TexelBlendedProperly(pContext, InCanvas, CanvasPoint, InSourceColor, InDestColor, InOp);
	}

	bool RT_SpriteHidden(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, bool bMatchAlpha)
	{
		return RT_SpriteHidden_AtCanvasPoint(pContext, InCanvas, InSprite, InSprite->GetLBPosition(), bMatchAlpha);
	}

	bool RT_SpriteHidden_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const SprVec2& InSpriteLBPosition_AsCanvasPoint, bool bMatchAlpha)
	{		
		SprVec2 SpriteCenter = MySprMath::GetCenterOfRotatedRect(InSpriteLBPosition_AsCanvasPoint, InSprite->GetSize(), InSprite->GetOrigin(), InSprite->GetRotationAngle());
		return RT_IsCanvasClear_AtCanvasPoint(pContext, InCanvas, SpriteCenter, bMatchAlpha);
	}
} // Test