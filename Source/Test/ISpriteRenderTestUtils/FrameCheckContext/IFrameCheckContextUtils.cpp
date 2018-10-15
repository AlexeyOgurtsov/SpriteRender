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
		return InCanvas->GetRect().PointAt(InFactor.X, InFactor.Y);
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

	bool RT_ColorMatchesCanvas_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const SprVec2& InCanvasPoint, const TexelColor& InColor)
	{
		return InColor == RT_ColorByCanvasPoint(pContext, InCanvas, InCanvasPoint);
	}

	bool RT_IsCanvasClear_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const Vec2& InCanvasPoint)
	{
		return RT_ColorMatchesCanvas_AtCanvasPoint(pContext, InCanvas, InCanvasPoint, pContext->GetRTConfig().ClearColor);
	}

	bool RT_TextureMatches_CanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, const SprVec2& InCanvasPoint, const IMPL::TextureElement& InTextureElement)
	{
		const IntVec Coord {0,0};
		const TexelColor TexColor = IMPL::GetTexelColorAt(pContext->GetDevCon(), InTextureElement.GetTexture(), Coord, /*Subresource=*/0);
		return  RT_ColorMatchesCanvas_AtCanvasPoint(pContext, InCanvas, InCanvasPoint, TexColor);
	}

	bool RT_SpriteVisibleAsColor(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const TexelColor& InColor)
	{
		SprVec2 const CanvasPoint = InSprite->GetCenter();
		return RT_IsCanvasClear_AtCanvasPoint(pContext, InCanvas, CanvasPoint);
	}

	bool RT_SpriteHidden(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite)
	{
		return RT_SpriteHidden_AtCanvasPoint(pContext, InCanvas, InSprite, InSprite->GetPosition());
	}

	bool RT_SpriteHidden_AtCanvasPoint(IFrameCheckContext* pContext, CanvasHandle InCanvas, SpriteHandle InSprite, const SprVec2& InSpritePosition_AsCanvasPoint)
	{		
		SprVec2 SpriteCenter = MySprMath::GetCenter(InSpritePosition_AsCanvasPoint, InSprite->GetSize());
		return RT_IsCanvasClear_AtCanvasPoint(pContext, InCanvas, SpriteCenter);
	}
} // Test