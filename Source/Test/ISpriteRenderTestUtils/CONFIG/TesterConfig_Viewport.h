#pragma once

#include <d3d11.h>

namespace Test
{
	/**
	* Parameters of the viewport and render target: width, height, format
	*/
	struct TesterConfig_Viewport
	{
		UINT Width = 1024;
		UINT Height = 768;
	};
} // Test