#include "ISpriteRenderTestUtils/Fixture/FixtureBase.h"
#include "ISpriteRenderTestUtils/IMPL/Environment.h"
#include <vector>
#include <boost/assert.hpp>

namespace Test
{
	namespace IMPL
	{
		class Environment;
	} // IMPL

	namespace {
		/**
		* Helper to get the environment within the FixtureBase
		*/
		IMPL::Environment* GetEnv() { return IMPL::Environment::Get(); }

		/**
		* Global tester config variable.
		*/
		std::vector<TesterConfig> gConfigStack;

	}

	UINT FixtureBase::GetRTWidth() const
	{
		return GetEnv()->GetD3DDevice()->GetRTWidth();
	}

	UINT FixtureBase::GetRTHeight() const
	{
		return GetEnv()->GetD3DDevice()->GetRTHeight();
	}

	bool FixtureBase::IsGloballyReadyForTesting() const
	{
		return GetEnv()->IsReadyForTesting();
	}

	std::ofstream& FixtureBase::GetLog() const
	{
		return GetEnv()->GetMainLog();
	}

	FixtureBase::FixtureBase() :
		bNeverInteractive{false}
	{
		// @TODO
	}

	FixtureBase::~FixtureBase()
	{
		// @TODO
	}

	TesterConfig* FixtureBase::BeginUpdateConfig()
	{
		T_LOG("FixtureBase::BeginUpdateConfig...");
		BOOST_ASSERT_MSG( ! bUpdatingConfig, "Config must be not in update state");
		T_LOG("FixtureBase::BeginUpdateConfig DONE");
		bUpdatingConfig = true;
		return &gConfigStack.back();
	}

	void FixtureBase::EndUpdateConfig(const TesterConfig* pConfig)
	{
		T_LOG("FixtureBase::EndUpdateConfig...");
		BOOST_ASSERT(pConfig);
		BOOST_ASSERT_MSG(bUpdatingConfig, "Config must be in update state");
		bUpdatingConfig = false;
		T_LOG("FixtureBase::EndUpdateConfig DONE");
	}

	const TesterConfig& FixtureBase::GetConfig()const
	{
		BOOST_ASSERT_MSG(IsDefaultConfigSet(), "FixtureBase::GetConfig: ever default config is NOT set yet");
		return gConfigStack.back();
	}

	bool FixtureBase::IsDefaultConfigSet() const
	{
		return gConfigStack.size() > 0;
	}

	void FixtureBase::PushConfig(const TesterConfig& InConfig)
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::PushConfig...");
		BOOST_ASSERT_MSG(!bUpdatingConfig, "Config must be not in update state");
		gConfigStack.push_back(InConfig);	
		UpdateEnvironment_BasedOnConfig(InConfig);	
		T_LOG("ISpriteRenderTestUtils: FixtureBase::PushConfig DONE");
	}

	void FixtureBase::PopConfig()
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::PopConfig...");
		BOOST_ASSERT_MSG(!bUpdatingConfig, "Config must be not in update state");
		BOOST_ASSERT_MSG(gConfigStack.size() > 1, "At least one config except the default must be pushed to pop config!");
		gConfigStack.pop_back();
		UpdateEnvironment_BasedOnConfig(gConfigStack.back());	
		T_LOG("ISpriteRenderTestUtils: FixtureBase::PopConfig DONE");
	}

	void FixtureBase::EnablePick()
	{
		if (GetConfig().Pick.bEnabled)
		{
			T_LOG("Pick already enabled in config");
		}
		else
		{
			T_LOG("Enable picking in config...");
			TesterConfig* const pCfg = BeginUpdateConfig();
			pCfg->Pick.bEnabled = true;
			EndUpdateConfig(pCfg);
			T_LOG("Enable picking in config DONE");
		}
	}

	void FixtureBase::DisablePick()
	{
		if (false == GetConfig().Pick.bEnabled)
		{
			T_LOG("Pick already disabled in config");
		}
		else
		{
			T_LOG("Disable picking in config ...");
			TesterConfig* const pCfg = BeginUpdateConfig();
			pCfg->Pick.bEnabled = false;
			EndUpdateConfig(pCfg);
			T_LOG("Disable picking in config DONE");
		}
	}

	void FixtureBase::UpdateEnvironment_BasedOnConfig(const TesterConfig& InConfig)
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::UpdateEnvironment_BasedOnConfig...");
		if (GetEnv()->GetSpriteRenderManager())
		{
			T_LOG("ISpriteRenderTestUtils: FixtureBase::FixtureBase::UpdateEnvironment_BasedOnConfig: Sprite render manager exists, notifying it that default config is updated");
			GetEnv()->GetSpriteRenderManager()->OnDefaultTesterConfigUpdated(InConfig);
			GetEnv()->NotifySpriteRenderManager_D3DDeviceUpdated(/*Reason:*/"FixtureBase::FixtureBase::UpdateEnvironment_BasedOnConfig");
		}
		T_LOG("ISpriteRenderTestUtils: FixtureBase::UpdateEnvironment_BasedOnConfig DONE");
	}

	void FixtureBase::InitMinimalCore()
	{
		// WARNING!!! Logging should NOT be performed here, because the log is NOT yet initialized!!!
		std::string MainLogFilename = GetMainLogFullFilename(GetConfig().Log);
		GetEnv()->ReInitMainLog(MainLogFilename.c_str());
	}

	void FixtureBase::ReInitViewport()
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitViewport...");
		BOOST_ASSERT_MSG(nullptr == GetEnv()->GetWnd(), "FixtureBase::ReInitViewport: viewport window RE initialization is NOT yet implemented");
		UINT NewWidth = GetConfig().Viewport.Width;
		UINT NewHeight = GetConfig().Viewport.Height;
		T_LOG("ISpriteRenderTestUtils: NewWidth = " << NewWidth << "; NewHeight=" << NewHeight);
		GetEnv()->ResetWindow(NewWidth, NewHeight);
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitViewport DONE");
	}

	void FixtureBase::ReInitD3DDevice()
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitD3DDevice...");
		GetEnv()->ReInit_D3DDevice(GetConfig().Viewport.Width, GetConfig().Viewport.Height, GetConfig().D3D);
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitD3DDevice DONE");
	}

	void FixtureBase::ReInitResources()
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitResources...");
		GetEnv()->ReInit_D3DResources(GetConfig().Resources);
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitResources DONE");
	}

	void FixtureBase::ReInit_SpriteRender()
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInit_SpriteRender...");
		GetEnv()->ReInit_SpriteRender();
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInit_SpriteRender DONE");
	}

	ISpriteRenderSubsystemManager* FixtureBase::GetSpriteRenderSubsystemManager() const
	{
		ISpriteRenderSubsystemManager* pManager = GetEnv()->GetSpriteRenderManager();
		BOOST_ASSERT_MSG(pManager, "FixtureBase::GetSpriteRenderSubsystemManager(): manager MUST be set!");
		return pManager;
	}

	DXGI_FORMAT FixtureBase::GetSprRen_DefaultTextureFormat_Diffuse() const
	{
		return GetSpriteRenderSubsystemManager()->GetDefaultTextureFormat_Diffuse();
	}

	bool FixtureBase::PromptPresentationMode_ReturnTrueIfQuit()
	{
		T_LOG("FixtureBase::PromptPresentationMode...");
		if (ShouldShowInteractiveMessages())
		{
			bool bDisableInteractiveMode = false;

			int Answer = MessageBox(NULL, "Should we continue in interactive mode? (cancel - quit)", "Question", MB_YESNOCANCEL);
			switch (Answer)
			{
			case IDNO:
				bDisableInteractiveMode = true;
				break;

			case IDYES:
				bDisableInteractiveMode = false;
				break;

			case IDCANCEL:
				return true;

			default:
				BOOST_ASSERT_MSG(false, "Should not get here: MessageBox returned wrong value");
			}

			if (bDisableInteractiveMode)
			{
				T_LOG("User chosen to disable interactive mode");
				DisableInteractiveModeGlobally();
			}
		}
		T_LOG("FixtureBase::PromptPresentationMode DONE");
		return false;
	}

	bool FixtureBase::ShouldShowInteractiveMessages() const
	{
		return GetConfig().Tester.bShowMessageBeforeTest && ( ! IsNeverInteractive() );
	}

	void FixtureBase::MarkNeverInteractive()
	{
		bNeverInteractive = true; 
	}

	void FixtureBase::DisableInteractiveModeGlobally()
	{
		T_LOG("Disable interactive mode...");
		bGloballyNeverInteractive = true;
		// WARNING!!! It should work ever is settings are NOT updated!
		/*		
		TesterConfig* pCfg = pFixture->BeginUpdateConfig();		
		pCfg->Tester.bShowMessageBeforeTest = false;
		pCfg->Tester.DelaySeconds = 0;
		pCfg->Tester.Presentation = ETestPresenation::NonStop;
		pFixture->EndUpdateConfig(pCfg);
		*/
		T_LOG("Disable interactive mode DONE");
	}

	bool FixtureBase::IsNeverInteractive() const
	{
		return bGloballyNeverInteractive || bNeverInteractive;
	}

	bool FixtureBase::bGloballyNeverInteractive = false;
} // Test