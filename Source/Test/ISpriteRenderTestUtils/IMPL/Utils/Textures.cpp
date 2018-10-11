#include "Textures.h"
#include "../../Utils/TestUtils.h"
#include <boost/assert.hpp>

namespace Test::IMPL
{
UINT ChooseTextureBindFlags(const STexInitializer& InInitializer)
{
	if (InInitializer.MipLevels == 0)
	{
		return InInitializer.BindFlags | D3D11_BIND_RENDER_TARGET;
	}
	else
	{
		return InInitializer.BindFlags;
	}
}

UINT ChooseTextureMiscFlags(const STexInitializer& InInitializer)
{
	if (InInitializer.MipLevels == 0)
	{
		return D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	else
	{
		return 0;
	}
}

TextureHandle CreateStagingTexture2D_Uninitialized
(
	ID3D11Device* pInDev, 
	UINT InWidth, UINT InHeight,
	DXGI_FORMAT InFormat, 
	UINT BindFlags, UINT InCpuAccessFlags,
	UINT InMipLevels
)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = InWidth;
	desc.Height = InHeight;
	desc.Format = InFormat;
	desc.MipLevels = InMipLevels;
	desc.ArraySize = 1;
	desc.SampleDesc.Quality = 0;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = InCpuAccessFlags;
	desc.BindFlags = BindFlags;
	desc.MiscFlags = 0;

	ID3D11Texture2D* pTempTexture = nullptr;
	HRESULT hr = pInDev->CreateTexture2D(&desc, nullptr, &pTempTexture);
	if (FAILED(hr))
	{
		throw HRException(hr, "CreateDefaultTexture2D_Uninitialized FAILED");
	}
	return TextureHandle(pTempTexture);
}


TextureHandle CreateTexture2D(ID3D11Device* pInDev, const STexInitializer& InInitializer, const TextureData& InTextureData)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = InInitializer.Props.Width;
	desc.Height = InInitializer.Props.Height;
	desc.Format = InInitializer.Props.Format;
	desc.MipLevels = InInitializer.MipLevels;
	desc.ArraySize = 1;
	desc.SampleDesc.Quality = 0;
	desc.SampleDesc.Count = 1;
	desc.Usage = InInitializer.MemoryClass.GetUsage();
	desc.CPUAccessFlags = InInitializer.MemoryClass.GetCPUAccessFlags();
	desc.BindFlags = ChooseTextureBindFlags(InInitializer);
	desc.MiscFlags = ChooseTextureMiscFlags(InInitializer);

	ID3D11Texture2D* pTempTexture = nullptr;
	HRESULT hr = pInDev->CreateTexture2D(&desc, &InTextureData, &pTempTexture);
	if(FAILED(hr))
	{
		throw HRException(hr, "CreateTexture2D FAILED");
	}
	return TextureHandle(pTempTexture);
} 

ShaderResourceViewHandle CreateTextureShaderView(ID3D11Device* pInDev, ID3D11Texture2D* pInTexture)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	HRESULT hr = pInDev->CreateShaderResourceView(pInTexture, nullptr, &pSRV);
	if (FAILED(hr))
	{
		throw HRException(hr, "CreateTextureShaderView FAILED");
	}
	return ShaderResourceViewHandle{pSRV};
}

TextureElement CreateTextureElement(ID3D11Device* pInDev, const STexInitializer& InInitializer, const TextureData& InTextureData)
{
	TextureHandle pTexture = CreateTexture2D(pInDev, InInitializer, InTextureData);
	ShaderResourceViewHandle pSRV = CreateTextureShaderView(pInDev, pTexture.get());
	return TextureElement(std::move(pTexture), std::move(pSRV), InInitializer.Props, InInitializer.MemoryClass);
}

Textures::Textures(const STexturesInitializer& InInitializer) :
	_pDev(InInitializer.pDev)
,	_pDevCon(InInitializer.pDevCon)
{
	BOOST_ASSERT(_pDev);
	BOOST_ASSERT(_pDevCon);
}

TextureElement* Textures::AddTexture(const STexInitializer& InInitializer, const TextureData& InTextureData)
{
	TextureElement textureElement = CreateTextureElement(_pDev, InInitializer, InTextureData);
	if (InInitializer.MipLevels == 0)
	{
		_pDevCon->GenerateMips(textureElement.GetShaderResourceView());
	}
	_textures.emplace_back(std::move(textureElement));
	return &_textures.back();
}
} //  Test::IMPL