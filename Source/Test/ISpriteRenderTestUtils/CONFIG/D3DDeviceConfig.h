#pragma once

#include <d3d11.h>
#include "../Utils/TexelColor.h"

namespace Test
{
	struct TesterConfig_D3DDevice_RenderTarget
	{
		const FLOAT* GetClearColorFloat() const { return ClearColorFloat; }

		FLOAT ClearColorFloat[4] = { 0.0F, 0.0F, 0.0F, 1.0F };
		TexelColor ClearColor;

		TesterConfig_D3DDevice_RenderTarget() :
			ClearColor{TexelColor::GetBlack(DXGI_FORMAT_R8G8B8A8_UNORM)} {}
	};

	struct TesterConfig_D3DDevice_DepthStencil
	{
		DXGI_FORMAT Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		FLOAT ClearZ = 1.0F;
		UINT8 ClearStencil = 0;

		/**
		* WARNING!!! Clear flags must correspond the DepthStencilFormat
		*/
		UINT GetClearFlags() const
		{
			return D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
		}
	};

	struct TesterConfig_D3DDevice_CopyBuffers
	{
		bool bEnableRT = true;
		bool bEnableDepthStencil = true;
	};

	struct TesterConfig_D3DDevice_MSAA
	{
		UINT NumSamples = 1;
		UINT NumQualityLevels = 0;	
	};

	struct TesterConfig_D3DDevice
	{
		TesterConfig_D3DDevice_RenderTarget RenderTarget;
		TesterConfig_D3DDevice_DepthStencil DepthStencil;
		TesterConfig_D3DDevice_CopyBuffers CopyBuffers;
		TesterConfig_D3DDevice_MSAA MSAA;

		UINT GetDeviceFlags() const { return 0; }
	};
} // Test
