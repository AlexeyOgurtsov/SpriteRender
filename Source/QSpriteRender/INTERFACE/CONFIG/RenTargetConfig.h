#pragma once

#include <d3d11.h>
#include <iomanip>
#include <string>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{

constexpr float DEFAULT_ZFAR = 1.0F;

struct SRenTargetConfig
{
	/**
	* Width and height of the Render target.
	*/
	int RTWidth, RTHeight;
	ID3D11RenderTargetView* pRenderTargetView;
	ID3D11DepthStencilView* pDepthStencilView;
	float ZFar = DEFAULT_ZFAR;

	SRenTargetConfig() :
		RTWidth(0)
	,	RTHeight(0)
	,	pRenderTargetView(nullptr)
	,	pDepthStencilView(nullptr)
	,	ZFar(DEFAULT_ZFAR) {}
	SRenTargetConfig
	(
		int InRTWidth, int InRTHeight,
		ID3D11RenderTargetView* pInRenderTargetView,
		ID3D11DepthStencilView* pInDepthStencilView
	) :
		RTWidth(InRTWidth)
	,	RTHeight(InRTHeight)
	,	pRenderTargetView(pInRenderTargetView)
	,	pDepthStencilView(pInDepthStencilView) {}
};

namespace IMPL::D3D
{
	std::string GetFormatString(DXGI_FORMAT InFormat);
}

template<class Strm>
inline void LogRenderTargetView(Strm& strm, const SRenTargetConfig& InConfig)
{
	ID3D11RenderTargetView* const pView = InConfig.pRenderTargetView;
	D3D11_RENDER_TARGET_VIEW_DESC desc;
	pView->GetDesc(&desc);
	strm << std::setw(32) << std::left << "Format: " << IMPL::D3D::GetFormatString(desc.Format) << std::endl;
}

template<class Strm>
inline void LogDepthStencilView(Strm& strm, const SRenTargetConfig& InConfig)
{
	ID3D11DepthStencilView* const pView = InConfig.pDepthStencilView;
	D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	pView->GetDesc(&desc);
	strm << std::setw(32) << std::left << "Format: " << IMPL::D3D::GetFormatString(desc.Format) << std::endl;
}

template<class Strm>
inline void LogRenderTargetConfig(Strm& strm, const SRenTargetConfig& InConfig)
{
	strm << std::left << std::setw(32) << "Render target size (width * height): " << InConfig.RTWidth << " * " << InConfig.RTHeight << std::endl;
	strm << "Render target view:" << std::endl;
	LogRenderTargetView(strm, InConfig);
	strm << "Depth stencil view:" << std::endl;
	LogDepthStencilView(strm, InConfig);
	strm << std::setw(32) << std::left << "ZFar: " << InConfig.ZFar << std::endl;
}

inline void AssertRenderTargetConfig_RTViewValid(const SRenTargetConfig& InConfig)
{
	ID3D11RenderTargetView* const pView = InConfig.pRenderTargetView;
	BOOST_ASSERT_MSG(pView, "QSpriteRender config is invalid: render target view pointer must be valid");
	D3D11_RENDER_TARGET_VIEW_DESC desc;
	pView->GetDesc(&desc);
	BOOST_ASSERT_MSG(desc.ViewDimension == D3D11_RTV_DIMENSION_TEXTURE2D, "QSpriteRender config is invalid: render target dimension must be Texture2D");
	BOOST_ASSERT_MSG(desc.Format == DXGI_FORMAT_R8G8B8A8_UNORM, "QSpriteRender config is invalid: render target format is invalid");
}

inline void AssertRenderTargetConfig_DepthStencilViewValid(const SRenTargetConfig& InConfig)
{
	ID3D11DepthStencilView* const pView = InConfig.pDepthStencilView;
	BOOST_ASSERT_MSG(pView, "QSpriteRender config is invalid: depth-stencil view pointer must be valid");
	D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	pView->GetDesc(&desc);
	BOOST_ASSERT_MSG(desc.ViewDimension == D3D11_DSV_DIMENSION_TEXTURE2D, "QSpriteRender config is invalid: depth stencil view dimension must be Texture2D");
	BOOST_ASSERT_MSG( ! (D3D11_DSV_READ_ONLY_DEPTH & desc.Flags ), "QSpriteRender config: depth stencil view must be writable");
}

/**
* Checks that config is valid.
* (meaning that the subsystem will work with the given properties)
*/
inline void AssertRenderTargetConfigValid(const SRenTargetConfig& InConfig)
{
	BOOST_ASSERT_MSG(InConfig.RTWidth > 0, "QSpriteRender config is invalid: Render target width must be greater than zero");
	BOOST_ASSERT_MSG(InConfig.RTHeight > 0, "QSpriteRender config is invalid: Render target height must be greater than zero");
	AssertRenderTargetConfig_RTViewValid(InConfig);
	AssertRenderTargetConfig_DepthStencilViewValid(InConfig);
	BOOST_ASSERT_MSG(InConfig.ZFar >= 0 && InConfig.ZFar <= 1.0F, "QSpriteRender config is invalid: ZFar must be in range [0.0F; 1.0F]");
	// WARNING!!! We do NOT check the consistency of the RenderTarget, DepthStencil and viewport,
	// because this objects may vary over time, and it's more efficient to check for their consistency,
	// using the internals of the render subsystem itself.
}

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv