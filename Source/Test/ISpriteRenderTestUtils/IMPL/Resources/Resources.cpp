#include "Resources.h"
#include "../D3DDevice/D3DDevice.h"
#include "../../Utils/TestUtils.h"

namespace Test::IMPL
{
	class D3DDevice;

	Resources::Resources(std::ofstream* pInLog, D3DDevice* pInD3D, const TesterConfig_Resources& InConfig) :
		pLog{pInLog}
	,	pD3D{pInD3D}
	,	Config{InConfig}
	{
		BOOST_ASSERT(pLog);
		BOOST_ASSERT(pD3D);

		T_LOG("Resources::CTOR...");
		InitTextures(InConfig.Textures);
		InitTextureLib(InConfig.Textures);
		T_LOG("Resources::CTOR DONE");
	}

	void Resources::Tick(std::ofstream& InLog, float DeltaSeconds)
	{
		T_LOG_TO(InLog, "Resources::Tick, DeltaSeconds=" << DeltaSeconds << "...");
		// Nothing is to be done here yet
		T_LOG_TO(InLog, "Resources::Tick DONE");
	}

	void Resources::BeginFrame(std::ofstream& InLog, float DeltaSeconds)
	{
		T_LOG_TO(InLog, "Resources::BeginFrame, DeltaSeconds=" << DeltaSeconds << "...");
		// Nothing is to be done here yet
		T_LOG_TO(InLog, "Resources::BeginFrame DONE");
	}

	void Resources::EndFrame(std::ofstream& InLog)
	{
		T_LOG_TO(InLog, "Resources::EndFrame...");
		// Nothing is to be done here yet
		T_LOG_TO(InLog, "Resources::EndFrame DONE");
	}

	void Resources::InitTextures(const TesterConfig_Resources_Textures& InConfig)
	{
		T_LOG("Resources::InitTextures...");
		STexturesInitializer Initializer { InConfig, &GetLog(), pD3D->GetDev(), pD3D->GetDevCon() };
		pTextures.reset(new Textures(Initializer));
		T_LOG("Resources::InitTextures DONE");
	}

	void Resources::InitTextureLib(const TesterConfig_Resources_Textures& InConfig)
	{
		T_LOG("Resources::InitTextureLib...");
		pTextureLib.reset(new TextureLib(InConfig, &GetLog(), pTextures.get()));
		T_LOG("Resources::InitTextureLib DONE");
	}

	Resources::~Resources()
	{
		T_LOG("Resources::~DTOR...");
		// Nothing is to be done here yet
		T_LOG("Resources::~DTOR DONE");
	}
} // Test::IMPL