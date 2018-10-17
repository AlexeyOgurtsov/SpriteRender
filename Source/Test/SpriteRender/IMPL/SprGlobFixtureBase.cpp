#pragma once

#include "Test/SpriteRender/SprGlobFixtureBase.h"
#include "Test/SpriteRender/Config/DefaultGlobalConfig.h"
#include "Test/SpriteRender/CustomEnvironment.h"

namespace Test::ISpr::Spr
{
	namespace
	{
		IMPL::CustomEnvironment& GetCustomEnv() { return *IMPL::CustomEnvironment::Get(); }
	}

	SprGlobFixtureBase::SprGlobFixtureBase()
	{
		TesterConfig DefaultConfig;
		InitDefaultGlobalConfig(DefaultConfig);
		InitGlobalISpr(DefaultConfig);

		// WARNING! SetSpriteRenderManager must be called AFTER InitGlobalISpr
		SetSpriteRenderManager(new SpriteRenderSubsystemManager());
		Set_GloballyReadyForTesting("SprGlobFixtureBase");
		
		T_LOG("SprGlobalFixtureBase: Construction DONE");
	}

	void SprGlobFixtureBase::DoSpriteRenderManager_CustomSetup()
	{
		SpriteRenderCustomSetuper Setuper;
		InitDefaultCustomConfig(Setuper, GetConfig());
		GetCustomEnv().ResetDefaultCustomSettings(Setuper);
	}

	SprGlobFixtureBase::~SprGlobFixtureBase()
	{
		// @TODO
	}
} // Test::ISpr::Spr