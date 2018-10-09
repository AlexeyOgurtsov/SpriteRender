#include "DepthStencil.h"
#include "RenHRUtils.h"

namespace Test::IMPL
{

	namespace
	{
		void ResetTex
		(
			ID3D11Device* pDev,
			TCom<ID3D11Texture2D>& pOutTex,
			UINT InWidth, UINT InHeight,
			DXGI_FORMAT InFormat, D3D11_USAGE InUsage, UINT InCpuAccessFlags
		)
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.ArraySize = 1;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			desc.Width = InWidth;
			desc.Height = InHeight;
			desc.Format = InFormat;
			desc.CPUAccessFlags = InCpuAccessFlags;
			desc.MipLevels = 1;
			desc.Usage = InUsage;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			ID3D11Texture2D* pTempTexture = nullptr;
			HRESULT hr = pDev->CreateTexture2D(&desc, nullptr, &pTempTexture);
			if (FAILED(hr))
			{
				throw HRException(hr, "DepthStencil: CreateTexture2D: FAILED");
			}
			pOutTex.reset(pTempTexture);
		}

		void ResetView(ID3D11Device* pDev, TCom<ID3D11DepthStencilView>& pOutView, ID3D11Texture2D* pTex)
		{
			ID3D11DepthStencilView* pTempDepthStencilView = nullptr;
			HRESULT hr = pDev->CreateDepthStencilView(pTex, nullptr, &pTempDepthStencilView);
			if (FAILED(hr))
			{
				throw HRException(hr, "DepthStencil: CreateDepthStencilView: FAILED");
			}
			pOutView.reset(pTempDepthStencilView);
		}
	} // anonymous

	void DepthStencil::Reset
	(
		ID3D11Device* pDev,
		UINT InWidth, UINT InHeight,
		DXGI_FORMAT InFormat, D3D11_USAGE InUsage, UINT InCpuAccessFlags
	)
	{
		ResetTex(pDev, /*Out*/pTex, InWidth, InHeight, InFormat, InUsage, InCpuAccessFlags);
		ResetView(pDev, /*Out*/pView, pTex.get());
	}
} // Test::IMPL