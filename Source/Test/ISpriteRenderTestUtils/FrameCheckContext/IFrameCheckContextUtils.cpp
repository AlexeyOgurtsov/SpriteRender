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
} // Test