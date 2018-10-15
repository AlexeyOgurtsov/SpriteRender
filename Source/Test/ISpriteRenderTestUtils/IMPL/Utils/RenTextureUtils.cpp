#include "RenTextureUtils.h"
#include "../../Utils/TestUtils.h"

namespace Test::IMPL
{
	const void* GetTexelFromMIPPtr(const void* pSourceMip, size_t InTexelSize, UINT InRowPitch, UINT InRow, UINT InX)
	{
		return static_cast<const uint8_t*>(pSourceMip) + InRow * InRowPitch + InX * InTexelSize;
	}

	void CopyTexelFromMIP(void* pOutTexel, const void* pSourceMip, size_t InTexelSize, UINT InRowPitch, UINT InRow, UINT InX)
	{
		const void* pSourceTexel = GetTexelFromMIPPtr(pSourceMip, InTexelSize, InRowPitch, InRow, InX);
		std::memcpy(pOutTexel, pSourceTexel, InTexelSize);
	}

	UINT GetTexelOffset(DXGI_FORMAT InFormat, UINT InRowPitch, const IntVec& InCoord)
	{
		return InRowPitch * InCoord.Y + InCoord.X * GetFormatTexelSize(InFormat);
	}

	const void* GetTexelPtr(const void* pSourceTex, DXGI_FORMAT InFormat, UINT InRowPitch, const IntVec& InCoord)
	{
		return static_cast<const uint8_t*>(pSourceTex) + GetTexelOffset(InFormat, InRowPitch, InCoord);
	}

	TexelColor GetTexelColorAt(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pTex, const IntVec& InCoord, UINT InSubresource)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		ZeroMemory(&map, sizeof(map));
		HRESULT hr_mapped = pDevCon->Map(pTex, InSubresource, D3D11_MAP_READ, 0, &map);
		if (FAILED(hr_mapped))
		{
			throw HRException(hr_mapped, "GetTexelColorAt: Map failed");
		}

		D3D11_TEXTURE2D_DESC desc;
		pTex->GetDesc(&desc);

		TexelColor Texel = ::Test::IMPL::GetTexelColor(map, desc.Format, InCoord);

		pDevCon->Unmap(pTex, InSubresource);
		return Texel;
	}

	TexelColor GetTexelColor(const D3D11_MAPPED_SUBRESOURCE& InSubresource, DXGI_FORMAT InFormat, const IntVec& InCoord)
	{
		return ::Test::IMPL::GetTexelColor(InSubresource.pData, InFormat, InSubresource.RowPitch, InCoord);
	}

	TexelColor GetTexelColor(const void* pInData, DXGI_FORMAT InFormat, UINT InRowPitch, const IntVec& InCoord)
	{
		BOOST_ASSERT(pInData);
		const void* const pData = GetTexelPtr(pInData, InFormat, InRowPitch, InCoord);
		return TexelColor(InFormat, pInData);
	}

	void ZeroMainMip_ByMap(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pInTexture, D3D11_MAP InMap)
	{
		D3D11_TEXTURE2D_DESC Desc;
		pInTexture->GetDesc(&Desc);
		
		D3D11_MAPPED_SUBRESOURCE MapSub;
		HRESULT hr = pDevCon->Map(pInTexture, 0, InMap, 0, &MapSub);
		BOOST_ASSERT_MSG(SUCCEEDED(hr), "ZeroMainMip_ByMap: Map must succeed");
		ZeroMemory(MapSub.pData, MapSub.RowPitch * Desc.Height);

		pDevCon->Unmap(pInTexture, 0);
	}

	DXGI_FORMAT ReadTexel(void* pOutTexel, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		ZeroMemory(&map, sizeof(map));
		HRESULT hr_mapped = pInDevCon->Map(pTexture, 0, D3D11_MAP_READ, 0, &map);
		if (FAILED(hr_mapped))
		{
			throw HRException(hr_mapped, "ReadTexel: Map failed");
		}

		// Reading texel
		D3D11_TEXTURE2D_DESC desc;
		pTexture->GetDesc(&desc);


		const UINT texelSize = GetFormatTexelSize(desc.Format);
		CopyTexelFromMIP(pOutTexel, map.pData, texelSize, map.RowPitch, InRow, InX);

		pInDevCon->Unmap(pTexture, 0);
		return desc.Format;
	}

	bool TexelMatches(const void* pInRefTexel, DXGI_FORMAT InRefTexelFormat, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX)
	{
		assert(pInRefTexel);

		// Buffer enough for any pixel
		uint8_t texelBuf[1024];

		DXGI_FORMAT TexFormat = ReadTexel(texelBuf, pInDevCon, pTexture, InRow, InX);

		return AreTexelsMatch(pInRefTexel, InRefTexelFormat, texelBuf, TexFormat);
	}
} // Test::IMPL