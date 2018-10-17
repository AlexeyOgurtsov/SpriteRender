#pragma once

#include "Com.h"
#include "Exception.h"
#include <d3d11.h>

namespace Test::IMPL
{
	class DepthStencil
	{
	public:
		ID3D11Texture2D* GetTex() const { return pTex.get(); }
		ID3D11DepthStencilView* GetView() const { return pView.get(); }

		void Reset
		(	
			ID3D11Device* pDev,
			UINT InWidth, UINT InHeight, 
			DXGI_FORMAT InFormat, D3D11_USAGE InUsage, UINT InCpuAccessFlags
		) throw(HRException);
		void Reset();

	private:
		TCom<ID3D11Texture2D> pTex;
		TCom<ID3D11DepthStencilView> pView;
	};
} // Test::IMPL