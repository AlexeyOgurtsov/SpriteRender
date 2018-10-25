#include "RenTextureUtils.h"
#include "../../Utils/TestUtils.h"

namespace Test::IMPL
{
	namespace
	{
		struct MapHelper
		{
			MapHelper
			(
				ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pInResource, UINT InSubresource,
				D3D11_MAP InMapType,
				UINT InMapFlags = 0
			) :
				pResource{ pInResource }
				, pDevCon{ pInDevCon }
				, Subresource{ InSubresource }
			{
				BOOST_ASSERT(pDevCon);
				BOOST_ASSERT(pResource);
				ZeroMemory(&M, sizeof(M));
				HRESULT hr_mapped = pDevCon->Map(pResource, Subresource, InMapType, InMapFlags, &M);
				if (FAILED(hr_mapped))
				{
					throw HRException(hr_mapped, "Map failed");
				}
			}
			~MapHelper()
			{
				pDevCon->Unmap(pResource, Subresource);
			}

			const D3D11_MAPPED_SUBRESOURCE& GetMap() const { return M; }

		private:
			ID3D11Resource * pResource = nullptr;
			UINT Subresource;
			ID3D11DeviceContext *pDevCon = nullptr;
			D3D11_MAPPED_SUBRESOURCE M;
		};
	}

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

	bool TextureFilledWithColor(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pTex, UINT InSubresource, const TexelColor& InRefColor, bool bMatchAlpha)
	{
		MapHelper M { pDevCon, pTex, InSubresource, D3D11_MAP_READ };
		D3D11_TEXTURE2D_DESC desc;
		pTex->GetDesc(&desc);


		for (int Y = 0; Y < static_cast<int>(desc.Height); Y++)
		{
			for (int X = 0; X < static_cast<int>(desc.Width); X++)
			{
				TexelColor Texel = ::Test::IMPL::GetTexelColor(M.GetMap(), desc.Format, IntVec{X,Y});
				if (false == AreTexelsMatch(InRefColor, Texel, bMatchAlpha))
				{
					return false;
				}
			}
		}		
		return true;
	}

	TexelColor GetTexelColorAt(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pTex, const IntVec& InCoord, UINT InSubresource)
	{
		MapHelper M{ pDevCon, pTex, InSubresource, D3D11_MAP_READ };

		D3D11_TEXTURE2D_DESC desc;
		pTex->GetDesc(&desc);

		TexelColor Texel = ::Test::IMPL::GetTexelColor(M.GetMap(), desc.Format, InCoord);

		return Texel;
	}

	TexelColor GetTexelColor(const D3D11_MAPPED_SUBRESOURCE& InSubresource, DXGI_FORMAT InFormat, const IntVec& InCoord)
	{
		return ::Test::IMPL::GetTexelColor(InSubresource.pData, InFormat, InSubresource.RowPitch, InCoord);
	}

	TexelColor GetTexelColor(const void* pInData, DXGI_FORMAT InFormat, UINT InRowPitch, const IntVec& InCoord)
	{
		BOOST_ASSERT(pInData);
		const void* const pTexelData = GetTexelPtr(pInData, InFormat, InRowPitch, InCoord);
		return TexelColor(InFormat, pTexelData);
	}

	void ZeroMainMip_ByMap(ID3D11DeviceContext* pDevCon, ID3D11Texture2D* pInTexture, D3D11_MAP InMap)
	{
		D3D11_TEXTURE2D_DESC Desc;
		pInTexture->GetDesc(&Desc);
		
		MapHelper M{ pDevCon, pInTexture, 0, InMap };
		ZeroMemory(M.GetMap().pData, M.GetMap().RowPitch * Desc.Height);
	}

	DXGI_FORMAT ReadTexel(void* pOutTexel, ID3D11DeviceContext* pInDevCon, ID3D11Texture2D* pTexture, UINT InRow, UINT InX)
	{
		MapHelper M{pInDevCon, pTexture, 0, D3D11_MAP_READ};

		// Reading texel
		D3D11_TEXTURE2D_DESC desc;
		pTexture->GetDesc(&desc);


		const UINT texelSize = GetFormatTexelSize(desc.Format);
		CopyTexelFromMIP(pOutTexel, M.GetMap().pData, texelSize, M.GetMap().RowPitch, InRow, InX);

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