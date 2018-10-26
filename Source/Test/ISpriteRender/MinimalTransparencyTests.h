#pragma once

/**
* Minimal Transparency feature tests that any implementation should provide.
*
* Includes primitive alpha-blending.
*
* Should NEVER contain sofisticated tests or corner cases tests
* ( just "Should work" tests ).
*
* All other unit tests of the Transparency feature should depend on this test!
*/

#include <boost/test/included/unit_test.hpp>
#include "ISprPerTestFixture_Custom.h"
#include "ISprPerTestFixture_SingleCanvas_Smoke.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE
	(
		SuiteMinimalTransparency
	)

	std::string const TestName_MinimalTransparency_TestAlpha = std::string("TestTestMoveOnZTop");
	std::string const TestPath_MinimalTransparency_TestAlpha = SuitePath_MinimalTransparency + std::string("/") + TestName_MinimalTransparency_TestAlpha;

	BOOST_FIXTURE_TEST_CASE
	(
		TestAlpha, ISprPerTestFixture_SingleCanvas_Smoke,
		*boost::unit_test::depends_on(SuitePath_MinimalMoveZOrder)
	)
	{
		// Testing that alpha-blending works.

		SetupTest(TestPath_MinimalTransparency_TestAlpha.c_str());

		TSMaterialVector const SpriteMaterials = GetTSMaterials(10);
		TSSpriteVector const Sprites = PrepareSprites(SpriteMaterials);

		// @TODO
	}
} // Test::ISpr

BOOST_AUTO_TEST_SUITE_END() // SuiteMinimalTransparency