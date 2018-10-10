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
	{
		BOOST_ASSERT_MSG(IsGloballyReadyForTesting(), "PerTestFixtureBase: Must be globally ready for testing (maybe global fixture is not set)!");
	}

	PerTestFixtureBase::~PerTestFixtureBase()
	{
		T_LOG("Destructing PerTestFixtureBase for '" << TestName << "'");

		BOOST_ASSERT_MSG(bSetUp, "PerTextFixtureBase::~DTOR: the test was NOT set up");
		Pause_IfEnabled();

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

	void PerTestFixtureBase::ShowTestInfo_IfEnabled()
	{
		if (GetDefaultConfig().Tester.bShowMessageBeforeTest)
		{
			std::string MsgText = std::string("Test: ") + TestName;
			std::string MsgCaption = std::string("Starting test");
			MessageBox(NULL, MsgText.c_str(), MsgCaption.c_str(), MB_OK);
		}
	}

	void PerTestFixtureBase::Pause_IfEnabled()
	{
		const TesterConfig_Tester& Cfg = GetDefaultConfig().Tester;

		switch (Cfg.Presentation)
		{
		case ETestPresenation::Delay:
			T_LOG("PerTestFixtureBase::Pause_IfEnabled: Delay presentation mode");
			T_LOG("Pausing for " << Cfg.DelaySeconds << " seconds...");
			Sleep(static_cast<DWORD>(Cfg.DelaySeconds * 100));
			T_LOG("Pausing DONE");
			break;

		case ETestPresenation::NonStop:
			// At this case no pausing is performed at all
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
