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
		ZeroMemory(_Texture_10_10_R8G8B8A8_UNORM, sizeof(_Texture_10_10_R8G8B8A8_UNORM));
		// @TODO: Initialize default textures
		T_LOG("TexturesLib: ctor DONE");
	}

	TextureLib::~TextureLib()
	{
		T_LOG("TexturesLib: ~DTOR...");
		T_LOG("TexturesLib: ~DTOR DONE");
	}

	TextureElement& TextureLib::GetLazyTexture_10_10_ByColor(TextureElement** pOutTex, bool bRed, bool bGreen, bool bBlue, EColorBrightnessLevel InAlphaBrighness, EColorBrightnessLevel BrightnessLevel, DXGI_FORMAT InFormat)
	{
		uint8_t const red_component = ComponentCoeffFromBool(bRed, BrightnessLevel);
		uint8_t const green_component = ComponentCoeffFromBool(bGreen, BrightnessLevel);
		uint8_t const blue_component = ComponentCoeffFromBool(bBlue, BrightnessLevel);
		uint8_t const alpha_component = static_cast<uint8_t>(255 * GetBrightnessFactor(InAlphaBrighness));
		uint8_t const color[4] = { red_component, green_component, blue_component, alpha_component };
		return GetLazyTexture_10_10(pOutTex, color, InFormat);
	}

	TextureElement& TextureLib::GetTexture_10_10(bool bRed, bool bGreen, bool bBlue, EColorBrightnessLevel InAlphaBrighness, EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat)
	{
		TextureElement** ppTex = GetArrayByColorElementPtr(_Texture_10_10_R8G8B8A8_UNORM, bRed, bGreen, bBlue, InAlphaBrighness, InBrightness);
		return GetLazyTexture_10_10_ByColor(ppTex, bRed, bGreen, bBlue, InAlphaBrighness, InBrightness, InFormat);
	}

	TextureElement& TextureLib::GetRedTexture_10_10(DXGI_FORMAT InFormat)
	{		
		return GetRedTexture_10_10(InFormat, EColorBrightnessLevel::Highest, EColorBrightnessLevel::Highest);
	}

	TextureElement& TextureLib::GetGreenTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetGreenTexture_10_10(InFormat, EColorBrightnessLevel::Highest, EColorBrightnessLevel::Highest);
	}

	TextureElement& TextureLib::GetBlueTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetBlueTexture_10_10(InFormat, EColorBrightnessLevel::Highest, EColorBrightnessLevel::Highest);
	}

	TextureElement& TextureLib::GetWhiteTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetWhiteTexture_10_10(InFormat, EColorBrightnessLevel::Highest, EColorBrightnessLevel::Highest);
	}

	TextureElement& TextureLib::GetRedGreenTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetRedGreenTexture_10_10(InFormat, EColorBrightnessLevel::Highest, EColorBrightnessLevel::Highest);
	}

	TextureElement& TextureLib::GetRedBlueTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetRedBlueTexture_10_10(InFormat, EColorBrightnessLevel::Highest, EColorBrightnessLevel::Highest);
	}

	TextureElement& TextureLib::GetGreenBlueTexture_10_10(DXGI_FORMAT InFormat)
	{
		return GetGreenBlueTexture_10_10(InFormat, EColorBrightnessLevel::Highest, EColorBrightnessLevel::Highest);
	}

	TextureElement& TextureLib::GetRedTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InAlphaBrighness, EColorBrightnessLevel InBrightness)
	{
		return GetTexture_10_10(true, false, false, InAlphaBrighness, InBrightness, InFormat);
	}

	TextureElement& TextureLib::GetGreenTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InAlphaBrighness, EColorBrightnessLevel InBrightness)
	{
		return GetTexture_10_10(false, true, false, InAlphaBrighness, InBrightness, InFormat);
	}

	TextureElement& TextureLib::GetBlueTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InAlphaBrighness, EColorBrightnessLevel InBrightness)
	{
		return GetTexture_10_10(false, false, true, InAlphaBrighness, InBrightness, InFormat);
	}

	TextureElement& TextureLib::GetWhiteTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InAlphaBrighness, EColorBrightnessLevel InBrightness)
	{
		return GetTexture_10_10(true, true, true, InAlphaBrighness, InBrightness, InFormat);
	}

	TextureElement& TextureLib::GetRedGreenTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InAlphaBrighness, EColorBrightnessLevel InBrightness)
	{
		return GetTexture_10_10(true, true, false, InAlphaBrighness, InBrightness, InFormat);
	}

	TextureElement& TextureLib::GetRedBlueTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InAlphaBrighness, EColorBrightnessLevel InBrightness)
	{
		return GetTexture_10_10(true, false, true, InAlphaBrighness, InBrightness, InFormat);
	}

	TextureElement& TextureLib::GetGreenBlueTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InAlphaBrighness, EColorBrightnessLevel InBrightness)
	{
		return GetTexture_10_10(false, true, true, InAlphaBrighness, InBrightness, InFormat);
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