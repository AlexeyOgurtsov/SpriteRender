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
		BOOST_ASSERT_MSG(GetEnv()->GetSpriteRenderManager(), "PerTextFixtureBase: WARNING! SpriteRenderManager must be set!");
	}

	PerTestFixtureBase::~PerTestFixtureBase()
	{
		BOOST_ASSERT_MSG(bSetUp, "PerTextFixtureBase::~DTOR: the test was NOT set up");

		T_LOG("END OF TEST '" << TestName <<  "' (From PerTestFixtureBase)");
	}

	
	void PerTestFixtureBase::SetupTestBase(const char* InTestName, unsigned int InResetFlags)
	{
		BOOST_ASSERT(InTestName);
		TestName = std::string(InTestName);

		T_LOG("TEST '" << InTestName << "' (From PerTestFixtureBase)");

		ResetEnv(InResetFlags);
	}

	void PerTestFixtureBase::ResetEnv(unsigned int InResetFlags)
	{
		T_LOG("TEST '" << TestName << "': RESETTING ENVIRONMENT..");
		// @TODO: Log flags
		BOOST_ASSERT_MSG(false, "@TODO: PerTestFixtureBase::ResetEnv: Not yet impl");
		T_LOG("TEST '" << TestName << "': ENV RESET DONE");
	}
	
	ISpriteRender* PerTestFixtureBase::GetSprRen() const
	{
		return GetEnv()->GetSpriteRenderManager()->GetSpriteRender();
	}
} // Test
