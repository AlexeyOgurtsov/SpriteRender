#pragma once

/**
* Utilities for working with frame resources (like checking pixels of RT etc.).
*/

#include <d3d11.h>
#include "Utils/TexelColor.h"

namespace Test
{
	class IFrameCheckContext;

	/**
	* Returns true if render target texel with the given coordinates matches another one.
	*/
	bool RT_TexelMatches(IFrameCheckContext* pContext, UINT InRow, UINT InX, const void* pInRefTexel, DXGI_FORMAT InRefFormat);
} // Test