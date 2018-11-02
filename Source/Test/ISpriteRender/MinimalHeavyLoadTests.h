#pragma once

/**
* Tests for minimally-required capabilities with high counts of test objects.
*
* For example, when sprite count is high.
*/

#include <boost/test/included/unit_test.hpp>
#include "ISprPerTestFixture_Custom.h"
#include "ISprPerTestFixture_SingleCanvas_Smoke.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE
	(
		SuiteMinimalHeavyLoad
	)

	std::string const TestName_MinimalHeavyLoad_TestSprites = std::string("TestSprites");
	std::string const TestPath_MinimalHeavyLoad_TestSprites = SuitePath_MinimalHeavyLoad + std::string("/") + TestName_MinimalHeavyLoad_TestSprites;
	BOOST_FIXTURE_TEST_CASE
	(
		TestSprites, ISprPerTestFixture_SingleCanvas_Smoke,
		*boost::unit_test::depends_on(SuitePath_MinimalSmoke)
		*boost::unit_test::depends_on(SuitePath_MinimalTransparency)
		*boost::unit_test::depends_on(SuitePath_MinimalMoveZOrder)
	)
	{
		SetupTest(TestPath_MinimalHeavyLoad_TestSprites.c_str());

		const int NUM_SPRITES = 50;
		// @TODO: Check that number of sprites is in accordance with buffer capacity

		TSMaterialVector const SpriteMaterials = GetTSMaterials(NUM_SPRITES);
		TSSpriteVector const Sprites = PrepareSprites(SpriteMaterials);

		SprSize const MOVE_DELTA_SIZE = ScreenPart(0.0F, 0.05F);
		SprVec2 const MOVE_DELTA = SprVec2{ MOVE_DELTA_SIZE.Width, MOVE_DELTA_SIZE.Height };
		int const NUM_MOVE_FRAMES = 5;
		
		BOOST_TEST_CHECKPOINT("Checking that sprites are visible");
		{
			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckInitialVisibility(pChecker, Sprites));
			}
		}

		BOOST_TEST_CHECKPOINT("Moving");
		{
			for (int MoveFrameIndex = 0; MoveFrameIndex < NUM_MOVE_FRAMES; MoveFrameIndex++)
			{
				for (int i = 0; i < NUM_SPRITES; i++)
				{
					const TSSprite* pS = &Sprites[i];
					MySprMath::SVec2 OriginalPosition = pS->GetHandle()->GetOriginPosition();
					MySprMath::SVec2 NewSpritePosition = OriginalPosition - MOVE_DELTA;
					SetSpritePosition(*pS, NewSpritePosition);

					BOOST_TEST_CHECKPOINT("CommitFrame");
					{
						IFrameCheckContextHandle pChecker = CommitFrame();
						BOOST_TEST(CheckInitialVisibility(pChecker, Sprites));
					}
				}
			}
		}
	}

	BOOST_AUTO_TEST_SUITE_END() // SuiteMinimalHeavyLoad
} // Test::ISpr