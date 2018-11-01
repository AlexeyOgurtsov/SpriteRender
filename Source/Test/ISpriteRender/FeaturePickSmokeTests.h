#pragma once

/**
* Minimal smoke tests for feature of picking.
*
* Should NEVER contain sofisticated tests or corner cases tests
* ( just "Should work" tests ).
*/

#include <boost/test/included/unit_test.hpp>
#include "ISprPerTestFixture_Custom.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE(Suite_FeaturePickSmoke)

	std::string const TestName_FeaturePickSmoke_PickBrackets = std::string("TestPickBrackets");
	std::string const TestPath_FeatureManyCanvasSmoke_PickBrackets = SuitePath_FeaturePickSmoke + std::string("/") + TestName_FeaturePickSmoke_PickBrackets;

	BOOST_FIXTURE_TEST_CASE(TestPickBrackets, ISprPerTestFixture_Custom)
	{
		MarkNeverInteractive();
		SetupTest(TestPath_FeatureManyCanvasSmoke_PickBrackets.c_str());

		// @TODO
		//BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginUpdates());
		//BOOST_REQUIRE_NO_THROW(GetSprRen()->EndUpdates());
	}


	BOOST_AUTO_TEST_SUITE_END() // Suite_FeaturePickSmoke
} // Test::ISpr