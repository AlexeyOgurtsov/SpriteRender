#pragma once

/**
* Minimal smoke tests.
*
* Should NEVER contain sofisticated tests or corner cases tests
* ( just "Should work" tests ).
*
* All other unit tests should depend on this set of tests!
*/

#include "ISprPerTestFixture_Custom.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE(SuiteMinimalSmoke)

	std::string const TestPath_UpdateBrackets = SuitePath_MinimalSmoke + std::string("/") + std::string("TestUpdateBrackets");
	BOOST_FIXTURE_TEST_CASE(TestUpdateBrackets, ISprPerTestFixture_Custom)
	{
		SetupTest("TestUpdateBrackets");

		BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginUpdates());
		BOOST_REQUIRE_NO_THROW(GetSprRen()->EndUpdates());
	}

	BOOST_FIXTURE_TEST_CASE
	(
		TestCanvasses, ISprPerTestFixture_Custom, 
		*boost::unit_test::depends_on(TestPath_UpdateBrackets.c_str())
	)
	{
		SetupTest("TestCanvasses");

		const CanvasId CanvId = 0;
		
		// WARNING!!! Here we should NOT test creating multiple canvasses,
		// because not necessarily that concrete SpriteRender implementation 
		// supports more than once canvas!
		BOOST_TEST_CHECKPOINT("Create");
		BOOST_REQUIRE_NO_THROW(GetSprRen()->CreateCanvas(CanvId, GetRTWidth(), GetRTHeight()));
		
		BOOST_TEST_CHECKPOINT("Show/Hide");
		{
			BOOST_REQUIRE_NO_THROW(GetSprRen()->ShowCanvas(CanvId));
			BOOST_REQUIRE_NO_THROW(GetSprRen()->HideCanvas(CanvId));
		}

		BOOST_TEST_CHECKPOINT("Update");
		{
			BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginUpdates());
			MySprRen::ISpriteUpdater* pUpdater = GetSprRen()->BeginCanvasUpdate(CanvId);
			BOOST_REQUIRE(pUpdater);
			BOOST_REQUIRE_NO_THROW(GetSprRen()->EndCanvasUpdate(pUpdater));
			BOOST_REQUIRE_NO_THROW(GetSprRen()->EndUpdates());
		}

		BOOST_TEST_CHECKPOINT("Delete");
		BOOST_REQUIRE_NO_THROW(GetSprRen()->DeleteCanvas(CanvId));
	}

	BOOST_AUTO_TEST_SUITE_END() // SuiteMinimalSmoke
} // Test::ISpr