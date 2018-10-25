#pragma once

#include <d3d11.h>
#include <cassert>
#include <cstdlib>
#include "Exception.h"
#include "RenHRUtils.h"
#include "../../Utils/TexelColor.h"
#include "../../Utils/MathUtils.h"

namespace Test::IMPL
{


/**
* Returns true if the given MIP-level of the fiven texture is filled with the given color.
*
* @NOTE: InSubresource: mip level for non-array (or mip level or array element)
*/
bool TextureFilledWithColor(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pTex, UINT InSubresource, const TexelColor& InRefColor, bool bMatchAlpha);

/**
* Returns texel color at the given point from the given texture.
*
* @NOTE: InSubresource: mip level for non-array (or mip level or array element)
*/
TexelColor GetTexelColorAt(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pTex, const IntVec& InCoord, UINT InSubresource);

/**
* Returns texel color at the given point from the given mapped subresource.
*/
TexelColor GetTexelColor(const D3D11_MAPPED_SUBRESOURCE& InSubresource, DXGI_FORMAT InFormat, const IntVec& InCoord);

/**
* Returns texel color at the given point from the given subresource.
*/
TexelColor GetTexelColor(const void* pInData, DXGI_FORMAT InFormat, UINT InRowPitch, const IntVec& InCoord);

/**
* Fills content of the texture with zeroes
*/
void ZeroMainMip_ByMap(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pInTexture, D3D11_MAP InMap);

bool TexelMatches(const void* pInRefTexel, DXGI_FORMAT InRefTexelFormat, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX);
} // Test::IMPL