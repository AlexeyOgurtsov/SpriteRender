#pragma once

#include "../Utils/Textures.h"
#include "../../CONFIG/TesterConfig_Resources.h"
#include "TextureLib.h"
#include <memory>
#include <fstream>

namespace Test::IMPL
{
	class D3DDevice;

	class Resources
	{
	public:
		Resources(std::ofstream* pInLog, D3DDevice* pInD3D, const TesterConfig_Resources& InConfig);
		~Resources();

		std::ofstream& GetLog() const { return *pLog; }

		Textures* GetTextures() const { return pTextures.get(); }
		TextureLib* GetTextureLib() const { return pTextureLib.get(); }

		void Tick(std::ofstream& InLog, float DeltaSeconds);
		void BeginFrame(std::ofstream& InLog, float DeltaSeconds);
		void EndFrame(std::ofstream& InLog);
		
	private:
		void InitTextures(const TesterConfig_Resources_Textures& InConfig);
		void InitTextureLib(const TesterConfig_Resources_Textures& InConfig);

		std::ofstream* pLog;
		D3DDevice* pD3D = nullptr;
		TesterConfig_Resources Config;
		std::unique_ptr<Textures> pTextures;
		std::unique_ptr<TextureLib> pTextureLib;
	};
} // Test::IMPL