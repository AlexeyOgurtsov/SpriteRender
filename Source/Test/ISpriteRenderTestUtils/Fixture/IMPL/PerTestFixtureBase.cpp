#include "../PerTestFixtureBase.h"
#include "../../Utils/TestUtils.h"
#include "../../IMPL/Environment.h"
#include "../../ISpriteRenderSubsystemManager.h"

namespace Test
{
	namespace 
	{
		IMPL::Environment* GetEnv() { return IMPL::Environment::Get(); }
	} // anonymous

	PerTestFixtureBase::PerTestFixtureBase() :
		bSetUp{false}
	,	TestName{"<UNSET>"}
	,	LocalFrameIndex{0}
	{
		BOOST_ASSERT_MSG(IsGloballyReadyForTesting(), "PerTestFixtureBase: Must be globally ready for testing (maybe global fixture is not set)!");
		PushConfig(GetConfig());
	}

	PerTestFixtureBase::~PerTestFixtureBase()
	{
		T_LOG("Destructing PerTestFixtureBase for '" << TestName << "'");

		BOOST_ASSERT_MSG(bSetUp, "PerTextFixtureBase::~DTOR: the test was NOT set up");
		Pause_IfEnabled();
		PopConfig();

		T_LOG("--------} END OF TEST '" << TestName <<  "' (From PerTestFixtureBase)");
	}

	
	void PerTestFixtureBase::SetupTestBase(const char* InTestName, unsigned int InResetFlags)
	{
		// We log empty lines before a new test
		for (int i = 0; i < 6; i++) { T_LOG(""); }		 		
		T_LOG("--------{ TEST '" << InTestName << "' (From PerTestFixtureBase)");

		BOOST_ASSERT_MSG(false == bSetUp, "PerTestFixtureBase::SetupTestBase: cannot call SetupTestBase twice!");
		BOOST_ASSERT(InTestName);
		TestName = std::string(InTestName);
		ShowTestInfo_IfEnabled();

		ResetEnv(InResetFlags);
		bSetUp = true;
	}

	void PerTestFixtureBase::ResetEnv(unsigned int InResetFlags)
	{
		T_LOG("TEST '" << TestName << "': RESETTING ENVIRONMENT..");
		IMPL::ResetEnvironment(InResetFlags);
		T_LOG("TEST '" << TestName << "': ENV RESET DONE");
	}

	void PerTestFixtureBase::OnFrameCheckContextReleased(IFrameCheckContext* pInSender)
	{
		T_LOG("TEST '" << TestName << "': FRAME CHECK CONTEXT RELEASED..");
		OnFrameCheckContextReleasedUser(pInSender);
		T_LOG("TEST '" << TestName << "': FRAME CHECK CONTEXT RELEASED DONE");
	}

	void PerTestFixtureBase::Quit()
	{
		T_LOG("TEST '" << TestName << "': QUITTING APPLICATION....");
		exit(2);
	}

	void PerTestFixtureBase::TickEnv(float InDeltaSeconds)
	{
		T_LOG("TEST '" << TestName << "': TICK ENV...");
		IMPL::TickEnvironment(GetLog(), InDeltaSeconds);
		if (GetEnv()->IsQuitRequested())
		{
			T_LOG("QUITTING... (Reason: requested from the application)");
			Quit();
		}
		Sleep(1);
		T_LOG("TEST '" << TestName << "': TICK ENV DONE");
	}

	void PerTestFixtureBase::BeginFrame()
	{
		GetEnv()->BeginFrame_FixDeltaSeconds(GetLog());

		T_LOG("TEST '" << TestName << "': Begin Frame " << GetLocalFrameIndex() << "...");
		IMPL::Environment_BeginFrame(GetLocalFrameIndex(), GetLog());
		T_LOG("TEST '" << TestName << "': Begin Frame DONE");
	}

	void PerTestFixtureBase::EndFrame()
	{
		T_LOG("TEST '" << TestName << "': End Frame " << GetLocalFrameIndex() << "...");
		IMPL::Environment_EndFrame(GetLocalFrameIndex(), GetLog());
		T_LOG("TEST '" << TestName << "': End Frame DONE");
	}

	float PerTestFixtureBase::GetDeltaSeconds_SinceLastBeginFrame() const
	{
		return GetEnv()->GetDeltaSeconds_SinceLastBeginFrame();
	}

	void PerTestFixtureBase::ShowTestInfo_IfEnabled()
	{
		if (ShouldShowInteractiveMessages())
		{
			BeginInteractive();
			std::string MsgText = std::string("Test: ") + TestName;
			std::string MsgCaption = std::string("Starting test");
			MessageBox(NULL, MsgText.c_str(), MsgCaption.c_str(), MB_OK);
			EndInteractive();
			if (PromptPresentationMode_ReturnTrueIfQuit())
			{
				T_LOG("Quitting (user chosen \"cancel\")");
				exit(0);
			}				
		}
	}

	void PerTestFixtureBase::BeginInteractive(bool bPauseTimers)
	{
		BOOST_ASSERT_MSG( ! bInteractive, "PerTestFixtureBase::BeginInteractive: already in interactive mode" );
		BOOST_ASSERT_MSG( ! IsNeverInteractive(), "PerTestFixtureBase::BeginInteractive: this function should NOT be called if IsNeverInteractive() is true" );

		T_LOG("PerTestFixtureBase::BeginInteractive...");
		if (bPauseTimers)
		{
			GetEnv()->BeginFrame_PauseDeltaCounter(GetLog());
		}
		bInteractive = true;
		T_LOG("PerTestFixtureBase::BeginInteractive DONE");
	}
	
	void PerTestFixtureBase::EndInteractive()
	{
		T_LOG("PerTestFixtureBase::EndInteractive...");		
		// WARNING!!! This function DO may be called ever if IsNeverInteractive()
		// - to make it possible to return to the normal mode when never interactive is true
		BOOST_ASSERT_MSG(bInteractive, "PerTestFixtureBase::BeginInteractive: not in interactive mode");
		GetEnv()->BeginFrame_ResumeDeltaCounter(GetLog());
		bInteractive = false;
		T_LOG("PerTestFixtureBase::EndInteractive DONE");
	}

	void PerTestFixtureBase::Pause_IfEnabled()
	{
		if(IsNeverInteractive())
		{	
			T_LOG("PerTestFixtureBase::Pause_IfEnabled: Skipping pause - IsNeverInteractive() is ON");
			return;
		}

		const TesterConfig_Tester& Cfg = GetConfig().Tester;

		switch (Cfg.Presentation)
		{
		case ETestPresenation::Delay:
			T_LOG("PerTestFixtureBase::Pause_IfEnabled: Delay presentation mode");
			T_LOG("Pausing for " << Cfg.DelaySeconds << " seconds...");
			BeginInteractive();
			Sleep(static_cast<DWORD>(Cfg.DelaySeconds * 100));
			EndInteractive();
			T_LOG("Pausing DONE");
			break;

		case ETestPresenation::NonStop:
			// At this case no pausing is performed at all
			T_LOG("PerTestFixtureBase::Pause_IfEnabled: Skipping pause - NON-stop presentation mode");
			return;

		default:
			BOOST_ASSERT_MSG(false, "PerTestFixtureBase::Pause_IfEnabled: Unknown test presentation enum value!");
		}
	}
	
	ISpriteRender* PerTestFixtureBase::GetSprRen() const
	{
		return GetEnv()->GetSpriteRenderManager()->GetSpriteRender();
	}
} // Test
