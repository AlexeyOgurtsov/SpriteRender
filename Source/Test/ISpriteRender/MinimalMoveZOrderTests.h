#pragma once

/**
* Minimal ZOrder feature tests that any implementation should provide.
*
* Includes Move-On-Z-Top test.
*
* Should NEVER contain sofisticated tests or corner cases tests
* ( just "Should work" tests ).
*
* All other unit tests of the MoveZOrder feature should depend on this test!
*/

#include <boost/test/included/unit_test.hpp>
#include "ISprPerTestFixture_Custom.h"
#include "ISprPerTestFixture_SingleCanvas_Smoke.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE
	(
		SuiteMinimalMoveZOrder
	)

	std::string const TestName_MinimalMoveZOrder_TestMoveOnZTop = std::string("TestTestMoveOnZTop");
	std::string const TestPath_MinimalMoveZOrder_TestMoveOnZTop = SuitePath_MinimalMoveZOrder + std::string("/") + TestName_MinimalMoveZOrder_TestMoveOnZTop;
	BOOST_FIXTURE_TEST_CASE
	(
		TestMoveOnZTop, ISprPerTestFixture_SingleCanvas_Smoke,
		*boost::unit_test::depends_on(SuitePath_MinimalSmoke)
	)
	{
		// Testing that Move on z top commands changes order;

		SetupTest(TestPath_MinimalMoveZOrder_TestMoveOnZTop.c_str());

		TSMaterialVector const SpriteMaterials = GetTSMaterials(10);
		TSSpriteVector const Sprites = PrepareSprites(SpriteMaterials);

		// We moving to position of the last sprite (which initially has the highest ZOrder)
		MoveAllSpritesTo(Sprites, Sprites[Sprites.size()-1].GetInitPos());

		for (int i = 0; i < static_cast<int>(Sprites.size()); i++)
		{
			const TSSprite* pSpriteToMoveOnTop = &Sprites[i];
			BOOST_CHECK_NO_THROW(MoveSpriteZOrderOnTop(*pSpriteToMoveOnTop));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, pSpriteToMoveOnTop->GetHandle(), pSpriteToMoveOnTop->GetInitUniColor()));
			}
		}
	}

	BOOST_AUTO_TEST_SUITE_END() // SuiteMinimalMoveZOrder
} // Test::ISpr