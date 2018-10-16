#pragma once

#include "RenUtilsSystem.h"
#include "RenInputLayoutTypes.h"
#include "QSpriteRender/INTERFACE/CONFIG/SpriteRenderConfig.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

class AmbientContext;

namespace D3D
{

using SamplerStateHandle = std::unique_ptr<ID3D11SamplerState,ComReleaser>;
using DepthStencilStateHandle = std::unique_ptr<ID3D11DepthStencilState,ComReleaser>;
using BlendStateHandle = std::unique_ptr<ID3D11BlendState,ComReleaser>;
using RasterizerStateHandle = std::unique_ptr<ID3D11RasterizerState,ComReleaser>;

struct SRenResourcesInitializer
{
	AmbientContext* pAmbientContext;
	ID3D11Device* pDev;
	ID3D11DeviceContext* pDevCon;
	SConfig Render;

	SRenResourcesInitializer(AmbientContext* pInAmbientContext, ID3D11Device* pInDev, ID3D11DeviceContext* pInDevCon, const SConfig& InRenderConfig) :
		pAmbientContext{ pInAmbientContext }
	,	pDev{ pInDev }
	,	pDevCon{ pInDevCon }
	,	Render{ InRenderConfig } {}
};

/**
* Manages D3D resources.
* Config, device are also resources.
*/
class RenResources
{
public:
	RenResources(const SRenResourcesInitializer& InInitializer);

	ID3D11Device* GetDev() const { return _pDev; }
	ID3D11DeviceContext* GetDevCon() const { return _pDevCon; }
	const SConfig& GetConfig() const { return _config; }

	ID3D11InputLayout* GetSpriteInputLayout() const { return _pSpriteInputLayout.get(); }

	ID3D11RasterizerState* GetRasterizerState() const { return _pRasterizerState.get(); }
	ID3D11SamplerState* GetDefaultSampler() const { return _pDefaultSampler.get(); }
	ID3D11DepthStencilState* GetDepthStencilState() const { return _pDepthStencilState.get(); }
	ID3D11BlendState* GetBlendStateOpaque() const { return _pBlendStateOpaque.get(); }
	ID3D11BlendState* GetBlendStateAdditive() const { return _pBlendStateAdditive.get(); }

	ID3D11RenderTargetView* GetRTView() const { return _config.RenderTarget.pRenderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() const { return _config.RenderTarget.pDepthStencilView; }
	int GetRTWidth() const { return _config.RenderTarget.RTWidth; }
	int GetRTHeight() const { return _config.RenderTarget.RTHeight; }

private:
	ID3D11Device* _pDev;
	ID3D11DeviceContext* _pDevCon;
	SConfig _config;
	InputLayoutHandle _pSpriteInputLayout;
	RasterizerStateHandle _pRasterizerState;
	SamplerStateHandle _pDefaultSampler;
	DepthStencilStateHandle _pDepthStencilState;
	BlendStateHandle _pBlendStateOpaque;
	BlendStateHandle _pBlendStateAdditive;
	AmbientContext* _pAmbientContext;
};


} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv