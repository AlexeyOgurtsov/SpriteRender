#pragma once

#include <d3d11.h>
#include <cassert>
#include <cstdlib>
#include "Exception.h"
#include "RenHRUtils.h"

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
* Returns true if the given pixel of the given texture matches another pixel.
*/

inline bool TexelMatches(const void* pInRefTexel, size_t InRefTexelSize, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX);

/**
* Reads a texel from the given texture based on the texture desc, mapping it.
*
* @returns: size of the read texel
*/
inline size_t ReadTexel(void* pOutTexel, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX);

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

inline UINT GetFormatTexelSize(DXGI_FORMAT InFormat)
{
	switch(InFormat)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return sizeof(uint32_t) * 4;

	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return sizeof(uint8_t) * 4;

	case DXGI_FORMAT_R8G8B8A8_UINT:
		return sizeof(uint8_t) * 4;

	default:
		break;
	}
	assert( !"Should NOT get here" );
	return 0;
}

inline size_t ReadTexel(void* pOutTexel, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX)
{
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(map));
	HRESULT hr_mapped = pInDevCon->Map(pTexture, 0, D3D11_MAP_READ, 0, &map);
	if(FAILED(hr_mapped))
	{
		throw HRException(hr_mapped, "ReadTexel: Map failed");
	}

	// Reading texel
	D3D11_TEXTURE2D_DESC desc;
	pTexture->GetDesc(&desc);
	

	const UINT texelSize = GetFormatTexelSize(desc.Format);
	const UINT rowPitch = desc.Width * texelSize; // How to get the row pitch from an ID3D11Texture2D resource?
	CopyTexelFromMIP(pOutTexel, map.pData, texelSize, rowPitch, InRow, InX);

	pInDevCon->Unmap(pTexture, 0);
	return texelSize;
}

inline bool TexelMatches(const void* pInRefTexel, size_t InRefTexelSize, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX)
{
	assert(pInRefTexel);
	// Buffer enough for any pixel
	uint8_t texelBuf[1024];
	size_t texelSize = ReadTexel(texelBuf, pInDevCon, pTexture, InRow, InX);
	assert(InRefTexelSize == texelSize);

	int cmpResult = std::memcmp(pInRefTexel, texelBuf, texelSize);
	
	return (0 == cmpResult);
}

} // Test::IMPL