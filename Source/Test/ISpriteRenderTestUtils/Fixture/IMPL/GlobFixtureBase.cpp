#include "ISpriteRenderTestUtils/Fixture/GlobFixtureBase.h"
#include "ISpriteRenderTestUtils/IMPL/Environment.h"
#include <boost/assert.hpp>

namespace Test
{
	namespace {
		/**
		* Helper to get the environment within the FixtureBase
		*/
		IMPL::Environment* GetEnv() { return IMPL::Environment::Get(); }
	}

	GlobFixtureBase::GlobFixtureBase()
	{
		// @TODO
	}

	GlobFixtureBase::~GlobFixtureBase()
	{
		// @TODO
	}

	void GlobFixtureBase::InitGlobal(const TesterConfig& InDefaultConfig)
	{
		PushConfig(InDefaultConfig);
		InitMinimalCore();
		ReInitViewport();
		ReInitD3DDevice();
		ReInitResources();
	}

	void GlobFixtureBase::Set_GloballyReadyForTesting(const char* pInReason)
	{
		GetEnv()->BeginTesting(pInReason);
	}

	void GlobFixtureBase::Unset_GloballyReadyForTesting(const char* pInReason)
	{
		GetEnv()->EndTesting(pInReason);
	}

	void GlobFixtureBase::SetSpriteRenderManager(ISpriteRenderSubsystemManager* pInManager)
	{
		T_LOG("GlobFixtureBase::SetSpriteRenderManager...");
		BOOST_ASSERT(pInManager);
		GetEnv()->SetSpriteRenderManager(pInManager);
		UpdateSpriteRenderManagerState(pInManager);
		T_LOG("GlobFixtureBase::SetSpriteRenderManager DONE");
	}

	/**
	* Updates the entire parameters of the sprite render manager (config, device settings etc.)
	* based on the current state of the application.
	*/
	void GlobFixtureBase::UpdateSpriteRenderManagerState(ISpriteRenderSubsystemManager* pInManager)
	{
		BOOST_ASSERT(pInManager);
		// WARNING!!! Log is to be binded FIRST of ALL
		{
			T_LOG("Binding log for " << pInManager->GetName() << "...");
			pInManager->SetLog(&GetLog());
			T_LOG("Binding log for " << pInManager->GetName() << " DONE");
		}

		if (IsDefaultConfigSet())
		{
			T_LOG("GlobFixtureBase::SetSpriteRenderManager: default config is already set - notifying the sprite render manager");
			pInManager->OnDefaultTesterConfigUpdated(GetConfig());
		}

		T_LOG("D3D update...");
		pInManager->OnD3DUpdated(GetEnv()->GetD3DDevice());
		T_LOG("D3D update DONE");

		T_LOG("Custom parameters update...");
		DoSpriteRenderManager_CustomSetup();
		T_LOG("Custom parameters update DONE");
	}
} // Test