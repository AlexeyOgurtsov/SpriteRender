#include "../IFrameCheckContext.h"
#include "../Utils/TestUtils.h"
#include "../ISpriteRenderSubsystemManager.h"

namespace Test
{
	DXGI_FORMAT IFrameCheckContext::GetRTFormat() const
	{
		return GetRTDesc().Format;
	}

	DXGI_FORMAT IFrameCheckContext::GetDepthStencilFormat() const
	{
		return GetDepthStencilDesc().Format;
	}

	const TesterConfig_D3DDevice_RenderTarget& IFrameCheckContext::GetRTConfig() const
	{
		return GetD3DConfig().RenderTarget;
	}

	const TesterConfig_D3DDevice_DepthStencil& IFrameCheckContext::GetDepthStencilConfig() const
	{
		return GetD3DConfig().DepthStencil;
	}

	UINT IFrameCheckContext::GetRTWidth() const
	{
		return GetRTDesc().Width;
	}

	UINT IFrameCheckContext::GetRTHeight() const
	{
		return GetRTDesc().Height;
	}

	const D3D11_TEXTURE2D_DESC& IFrameCheckContext::GetRTDesc() const
	{
		GetRT()->GetDesc(&RTDesc);
		return RTDesc;
	}

	const D3D11_TEXTURE2D_DESC& IFrameCheckContext::GetDepthStencilDesc() const
	{
		GetDepthStencil()->GetDesc(&DepthStencilDesc);
		return DepthStencilDesc;
	}

	MySprRen::SPickResult IFrameCheckContext::PickAtScreen(const MySprMath::SVec2& InPoint)
	{
		return GetSubsys()->PickAtScreen(InPoint);
	}

	MySprRen::SPickResult IFrameCheckContext::PickAtCanvasPoint(MySpr::SpriteCanvasId InId, const MySprMath::SVec2& InPoint)
	{
		return GetSubsys()->PickAtCanvasPoint(InId, InPoint);
	}
} // Test 