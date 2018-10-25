#include "RenResources.h"
#include "Geometry/SpriteGeometryVertex_InputLayout.h"
#include "QSpriteRender/IMPL/Environment/AmbientContext.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{
namespace D3D
{
	RasterizerStateHandle CreateRasterizerState(ID3D11Device* pDev);
	SamplerStateHandle CreateDefaultSampler(ID3D11Device* pDev);
	DepthStencilStateHandle CreateDepthStencilState(ID3D11Device* pDev);
	BlendStateHandle CreateBlendStateOpaque(ID3D11Device* pDev);
	BlendStateHandle CreateBlendStateAdditive(ID3D11Device* pDev);

	RenResources::RenResources(const SRenResourcesInitializer& InInitializer) :
		_pDev(InInitializer.pDev)
	,	_pDevCon(InInitializer.pDevCon)
	,	_config(InInitializer.Render)
	,	_pAmbientContext(InInitializer.pAmbientContext)
	{
		BOOST_ASSERT(_pDev);
		BOOST_ASSERT(_pDevCon);
		BOOST_ASSERT(_pAmbientContext);
		_pSpriteInputLayout = RegisterInputLayout_SpriteGeometryVertex(_pDev, InInitializer.Render.Shaders);
		_pRasterizerState = CreateRasterizerState(_pDev);
		_pDefaultSampler = CreateDefaultSampler(_pDev);
		_pDepthStencilState = CreateDepthStencilState(_pDev);
		_pBlendStateOpaque = CreateBlendStateOpaque(_pDev);
		_pBlendStateAdditive = CreateBlendStateAdditive(_pDev);
	}

	RasterizerStateHandle CreateRasterizerState(ID3D11Device* pDev)
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AntialiasedLineEnable = FALSE;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = FALSE;
		desc.MultisampleEnable = FALSE;
		desc.SlopeScaledDepthBias = 0.0F;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0F;
		desc.ScissorEnable = FALSE;
		desc.DepthClipEnable = FALSE;
		ID3D11RasterizerState* pTempState = nullptr;
		HRESULT hr = pDev->CreateRasterizerState(&desc, &pTempState);
		if(FAILED(hr))
		{
			throw SpriteRenderHRFailure("CreateRasterizerState: CreateRasterizerState FAILED", hr);
		}
		return RasterizerStateHandle(pTempState);
	}

	SamplerStateHandle CreateDefaultSampler(ID3D11Device* pDev)
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.MinLOD = 0.0F;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		desc.MipLODBias = 0.0F;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.Filter = D3D11_FILTER_ANISOTROPIC;
		ID3D11SamplerState* pTempState = nullptr;
		HRESULT hr = pDev->CreateSamplerState(&desc, &pTempState);
		if(FAILED(hr))
		{
			throw SpriteRenderHRFailure("CreateDefaultSampler: CreateSamplerState FAILED", hr);
		}
		return SamplerStateHandle(pTempState);
	}

	DepthStencilStateHandle CreateDepthStencilState(ID3D11Device* pDev)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		desc.StencilEnable = FALSE;
		desc.StencilReadMask = 0;
		desc.StencilWriteMask = 0;
		ID3D11DepthStencilState* pTempState = nullptr;
		HRESULT hr = pDev->CreateDepthStencilState(&desc, &pTempState);
		if(FAILED(hr))
		{
			throw SpriteRenderHRFailure("CreateDepthStencilState: CreateDepthStencilState FAILED", hr);
		}
		return DepthStencilStateHandle(pTempState);
	}

	BlendStateHandle CreateBlendStateOpaque(ID3D11Device* pDev)
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = FALSE;
		desc.IndependentBlendEnable = FALSE;
		desc.RenderTarget[0].BlendEnable = FALSE;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState* pTempState = nullptr;
		HRESULT hr = pDev->CreateBlendState(&desc, &pTempState);
		if(FAILED(hr))
		{
			throw SpriteRenderHRFailure("CreateBlendStateOpaque: CreateBlendState FAILED", hr);
		}
		return BlendStateHandle(pTempState);
	}

	BlendStateHandle CreateBlendStateAdditive(ID3D11Device* pDev)
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = FALSE;
		desc.IndependentBlendEnable = FALSE;
		desc.RenderTarget[0].BlendEnable = FALSE;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState* pTempState = nullptr;
		HRESULT hr = pDev->CreateBlendState(&desc, &pTempState);
		if(FAILED(hr))
		{
			throw SpriteRenderHRFailure("CreateBlendStateAdditive: CreateBlendState FAILED", hr);
		}
		return BlendStateHandle(pTempState);
	}
} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv