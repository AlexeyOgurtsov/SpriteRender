#include "RenTextureUtils.h"
#include "../../Utils/TestUtils.h"

namespace Test::IMPL
{
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