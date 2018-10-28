#pragma once

/**
* Extra checks for corner cases for minimal set of supported commands.
*/

#include <boost/test/included/unit_test.hpp>
#include "ISprPerTestFixture_Custom.h"
#include "ISprPerTestFixture_SingleCanvas_Smoke.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE
	(
		SuiteMinimalExtraChecks
	)

	std::string const TestName_MinimalExtraChecks_TestShowHide = std::string("TestShowHide");
	std::string const TestPath_MinimalExtraChecks_TestShowHide = SuitePath_MinimalExtraChecks + std::string("/") + TestName_MinimalExtraChecks_TestShowHide;

	BOOST_FIXTURE_TEST_CASE
	(
		TestShowHide, ISprPerTestFixture_SingleCanvas_Smoke,
		*boost::unit_test::depends_on(SuitePath_MinimalSmoke)
	)
	{
		// Checks default for Show/Hide and tests that they can be called multiple times consecutively.

		SetupTest(TestPath_MinimalExtraChecks_TestShowHide.c_str());		

		TSSpriteVector Sprites;
		int const spriteIndex = PrepareSprite
		(
			&Sprites, GetTestMat_Green(), DEFAULT_SPRITE_CANV_SPACE_POSITION, DEFAULT_SPRITE_CANV_SPACE_SIZE, MySprMath::SVec2{ 0,0 }, 
			/*RotationAngle=*/45.0F, 
			/*bShow=*/false
		);
		TSSprite* const pS = &Sprites[spriteIndex];

		BOOST_TEST_CHECKPOINT("CommitFrame");
		{
			IFrameCheckContextHandle pChecker = CommitFrame();
			BOOST_REQUIRE(SpriteHidden(pChecker, *pS));
		}


		BOOST_TEST_CHECKPOINT("HideWhenAlreadyHidden");
		{
			HideSprite(*pS);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteHidden(pChecker, *pS));
			}
		}

		BOOST_TEST_CHECKPOINT("Show");
		{
			ShowSprite(*pS);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, pS->GetHandle(), pS->GetInitUniColor()));
			}
		}

		BOOST_TEST_CHECKPOINT("ShowSecondTime");
		{
			ShowSprite(*pS);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, pS->GetHandle(), pS->GetInitUniColor()));
			}
		}

		BOOST_TEST_CHECKPOINT("Hide");
		{
			HideSprite(*pS);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteHidden(pChecker, *pS));
			}
		}

		BOOST_TEST_CHECKPOINT("HideWhenAlreadyHiddenTwo");
		{
			HideSprite(*pS);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteHidden(pChecker, *pS));
			}
		}
	}
} // Test::ISpr

BOOST_AUTO_TEST_SUITE_END() // SuiteMinimalExtraChecks