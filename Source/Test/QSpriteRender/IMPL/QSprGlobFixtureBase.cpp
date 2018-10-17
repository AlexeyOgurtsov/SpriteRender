#pragma once

#include "Test/QSpriteRender/QSprGlobFixtureBase.h"
#include "Test/QSpriteRender/Config/DefaultGlobalConfig.h"
#include "Test/QSpriteRender/CustomEnvironment.h"

namespace Test::ISpr::QSpr
{
	namespace
	{
		IMPL::CustomEnvironment& GetCustomEnv() { return *IMPL::CustomEnvironment::Get(); }
	}

	QSprGlobFixtureBase::QSprGlobFixtureBase()
	{
		TesterConfig DefaultConfig;
		InitDefaultGlobalConfig(DefaultConfig);
		InitGlobalISpr(DefaultConfig);

		// WARNING! SetSpriteRenderManager must be called AFTER InitGlobalISpr
		SetSpriteRenderManager(new SpriteRenderSubsystemManager());
		Set_GloballyReadyForTesting("QSprGlobFixtureBase");
		
		T_LOG("QSprGlobalFixtureBase: Construction DONE");
	}

	void QSprGlobFixtureBase::DoSpriteRenderManager_CustomSetup()
	{
		SpriteRenderCustomSetuper Setuper;
		InitDefaultCustomConfig(Setuper, GetConfig());
		GetCustomEnv().ResetDefaultCustomSettings(Setuper);
	}

	QSprGlobFixtureBase::~QSprGlobFixtureBase()
	{
		// @TODO
	}
} // Test::ISpr::QSpr