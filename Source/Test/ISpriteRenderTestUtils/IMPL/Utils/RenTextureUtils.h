#pragma once

#include <d3d11.h>
#include <cassert>
#include <cstdlib>
#include "Exception.h"
#include "RenHRUtils.h"
#include "../../Utils/TexelColor.h"

namespace Test::IMPL
{

/**
* Fills content of the texture with zeroes
*/
void ZeroMainMip_ByMap(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pInTexture, D3D11_MAP InMap);

/**
* Copy the single pixel from a single MIP-level of the texture.
*/
void CopyTexelFromMIP(void* pOutTexel, const void* pSourceMip, size_t InTexelSize, UINT InRowPitch, UINT InRow, UINT InX);

/**
* Get pointer to the texel with the given row and X.
*/
const void* GetTexelFromMIPPtr(const void* pSourceMip, size_t InTexelSize, UINT InRowPitch, UINT InRow, UINT InX);

/**
* Reads a texel from the given texture based on the texture desc, mapping it.
*
* @returns: size of the read texel
*/
DXGI_FORMAT ReadTexel(void* pOutTexel, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX);

/**
* IMPLEMENTATION
*/
inline void CopyTexelFromMIP(void* pOutTexel, const void* pSourceMip, size_t InTexelSize, UINT InRowPitch, UINT InRow, UINT InX)
{
	const void* pSourceTexel = GetTexelFromMIPPtr(pSourceMip, InTexelSize, InRowPitch, InRow, InX);
	std::memcpy(pOutTexel, pSourceTexel, InTexelSize);
}

inline const void* GetTexelFromMIPPtr(const void* pSourceMip, size_t InTexelSize, UINT InRowPitch, UINT InRow, UINT InX)
{
	return static_cast<const uint8_t*>(pSourceMip) + InRow * InRowPitch + InX * InTexelSize;
}
bool TexelMatches(const void* pInRefTexel, DXGI_FORMAT InRefTexelFormat, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX);
} // Test::IMPL