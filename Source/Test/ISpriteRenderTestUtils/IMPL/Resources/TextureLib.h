#pragma once

#include "../../CONFIG/TesterConfig_Resources.h"
#include "../Utils/TextureElement.h"
#include "../../Utils/TexelColor.h"
#include <fstream>

namespace Test::IMPL
{
	class Textures;
	using TextureArrayByColor = TArrayByColor<TextureElement*>;

	class TextureLib
	{
	public:
		TextureLib(const TesterConfig_Resources_Textures& InConfig, std::ofstream* pInLog, Textures* pInTextures);
		~TextureLib();

		const TesterConfig_Resources_Textures& GetConfig() const { return Config; }

		TextureElement& GetTexture_10_10(bool bRed, bool bGreen, bool bBlue, bool bAlpha, EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);

		TextureElement& GetRedTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetGreenTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetBlueTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetWhiteTexture_10_10(DXGI_FORMAT InFormat);

		TextureElement& GetRedGreenTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetRedBlueTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetGreenBlueTexture_10_10(DXGI_FORMAT InFormat);


		TextureElement& GetRedTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InBrightness);
		TextureElement& GetGreenTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InBrightness);
		TextureElement& GetBlueTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InBrightness);
		TextureElement& GetWhiteTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InBrightness);

		TextureElement& GetRedGreenTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InBrightness);
		TextureElement& GetRedBlueTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InBrightness);
		TextureElement& GetGreenBlueTexture_10_10(DXGI_FORMAT InFormat, EColorBrightnessLevel InBrightness);		

	private:
		std::ofstream& GetLog() const { return *pLog; }

		TextureElement& GetLazyTexture_10_10_ByColor(TextureElement** pOutTex, bool bRed, bool bGreen, bool bBlue, bool bAlpha, EColorBrightnessLevel InBrightness, DXGI_FORMAT InFormat);
		TextureElement& GetLazyTexture_10_10(TextureElement** pOutTex, const uint8_t InColor[4], DXGI_FORMAT InFormat);
		TextureElement& GetLazyTexture_R8G8B8A8_UNORM(TextureElement** pOutTex, UINT InWidth, UINT InHeight, const uint8_t InColor[4], UINT InNumMips = 1);

		/**
		* Array of texture of the noted size and type.
		*
		* First four indices: for corresponding index R/G/B/A component is enabled or not
		* Fifth index: Brightness level;
		*/
		TextureArrayByColor _Texture_10_10_R8G8B8A8_UNORM;	

		TesterConfig_Resources_Textures Config;
		std::ofstream* pLog = nullptr;
		Textures* pTextures = nullptr;
	};
} // Test::IMPL