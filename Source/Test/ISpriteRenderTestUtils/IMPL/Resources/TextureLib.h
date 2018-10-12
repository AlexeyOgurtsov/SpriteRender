#pragma once

#include "../../CONFIG/TesterConfig_Resources.h"
#include "../Utils/TextureElement.h"
#include <fstream>

namespace Test::IMPL
{
	class Textures;

	constexpr UINT DEFAULT_MINIMAL_TEX_SIZE  = 10;

	class TextureLib
	{
	public:
		TextureLib(const TesterConfig_Resources_Textures& InConfig, std::ofstream* pInLog, Textures* pInTextures);
		~TextureLib();

		const TesterConfig_Resources_Textures& GetConfig() const { return Config; }

		TextureElement& GetRedTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetGreenTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetBlueTexture_10_10(DXGI_FORMAT InFormat);
		TextureElement& GetWhiteTexture_10_10(DXGI_FORMAT InFormat);

	private:
		std::ofstream& GetLog() const { return *pLog; }

		TextureElement& GetLazyTexture_10_10(TextureElement** pOutTex, const uint8_t InColor[4], DXGI_FORMAT InFormat);
		TextureElement& GetLazyTexture_R8G8B8A8_UNORM(TextureElement** pOutTex, UINT InWidth, UINT InHeight, const uint8_t InColor[4], UINT InNumMips = 1);

		TextureElement* _pRedTexture_10_10_R8G8B8A8_UNORM = nullptr;
		TextureElement* _pBlueTexture_10_10_R8G8B8A8_UNORM = nullptr;
		TextureElement* _pGreenTexture_10_10_R8G8B8A8_UNORM = nullptr;
		TextureElement* _pWhiteTexture_10_10_R8G8B8A8_UNORM = nullptr;

		TesterConfig_Resources_Textures Config;
		std::ofstream* pLog = nullptr;
		Textures* pTextures = nullptr;
	};
} // Test::IMPL