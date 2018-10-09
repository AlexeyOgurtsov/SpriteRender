#pragma once

#include <d3d11.h>

namespace Test
{
	struct TesterConfig_D3DDevice
	{
		DXGI_FORMAT DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	};
} // Test
