#include "TextureLib.h"
#include "../Utils/Textures.h"
#include "../../Utils/TestUtils.h"

namespace Test::IMPL
{
	TextureLib::TextureLib(const TesterConfig_Resources_Textures& InConfig, std::ofstream* pInLog, Textures* pInTextures) :
		Config{InConfig}
	,	pLog{pInLog}
	,	pTextures{pInTextures}
	{
		BOOST_ASSERT(pLog);
	
		T_LOG("TexturesLib: ctor...");
		BOOST_ASSERT(pTextures);
		// @TODO: Initialize default textures
		T_LOG("TexturesLib: ctor DONE");
	}

	TextureLib::~TextureLib()
	{
		T_LOG("TexturesLib: ~DTOR...");
		T_LOG("TexturesLib: ~DTOR DONE");
	}

	TextureElement& TextureLib::GetRedTexture_10_10(DXGI_FORMAT InFormat)
	{
		TextureElement** ppTex = &_pRedTexture_10_10_R8G8B8A8_UNORM;
		constexpr uint8_t color[4] = { 255, 0, 0, 0 };
		return GetLazyTexture_10_10(ppTex, color, InFormat);
	}

	TextureElement& TextureLib::GetGreenTexture_10_10(DXGI_FORMAT InFormat)
	{
		TextureElement** ppTex = &_pGreenTexture_10_10_R8G8B8A8_UNORM;
		constexpr uint8_t color[4] = { 0, 255, 0, 0 };
		return GetLazyTexture_10_10(ppTex, color, InFormat);
	}

	TextureElement& TextureLib::GetBlueTexture_10_10(DXGI_FORMAT InFormat)
	{
		TextureElement** ppTex = &_pBlueTexture_10_10_R8G8B8A8_UNORM;
		constexpr uint8_t color[4] = { 0, 0, 255, 0 };
		return GetLazyTexture_10_10(ppTex, color, InFormat);
	}

	TextureElement& TextureLib::GetWhiteTexture_10_10(DXGI_FORMAT InFormat)
	{
		TextureElement** ppTex = &_pWhiteTexture_10_10_R8G8B8A8_UNORM;
		constexpr uint8_t color[4] = { 255, 255, 255, 0 };
		return GetLazyTexture_10_10(ppTex, color, InFormat);
	}

	TextureElement& TextureLib::GetLazyTexture_10_10(TextureElement** pOutTex, const uint8_t InColor[4], DXGI_FORMAT InFormat)
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return GetLazyTexture_R8G8B8A8_UNORM(pOutTex, DEFAULT_MINIMAL_TEX_SIZE, DEFAULT_MINIMAL_TEX_SIZE, InColor);

		default:
			BOOST_ASSERT_MSG(false, "TextureLib::GetLazyTexture_10_10: Case label for format NOT yet impl");
			break;
		}
		return **pOutTex;
	}


	TextureElement& TextureLib::GetLazyTexture_R8G8B8A8_UNORM(TextureElement** pOutTex, UINT InWidth, UINT InHeight, const uint8_t InColor[4], UINT InNumMips)
	{
		T_LOG("TextureLib::GetLazyTexture_R8G8B8A8_UNORM...");
		T_LOG("Width*Height=" << InWidth << "*" << InHeight);
		T_LOG("Color=" << "{" << InColor[3] << ";" << InColor[2] << ";" << InColor[1] << ";" << InColor[0] << "}");
		T_LOG("NumMips=" << InNumMips);
		BOOST_ASSERT(pOutTex);
		if (nullptr == *pOutTex)
		{
			T_LOG("Creating texture...");
			*pOutTex = AddUniformTexture_R8G8B8A8_UNORM(pTextures, InColor, InWidth, InHeight, InNumMips);
			T_LOG("Creating texture DONE");
		}
		else
		{
			T_LOG("Texture already cached.");
		}
		T_LOG("TextureLib::GetLazyTexture_R8G8B8A8_UNORM DONE");
		return **pOutTex;
	}	
} // Test::IMPL