#pragma once

/**
* Minimal sprite rotation feature tests that any implementation should provide.
*/

#include <boost/test/included/unit_test.hpp>
#include "ISprPerTestFixture_Custom.h"
#include "ISprPerTestFixture_SingleCanvas_Smoke.h"
#include "TestHelpers.h"


namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE
	(
		SuiteMinimalRotate
	)

	std::string const TestName_MinimalRotate_DefaultOrigin = std::string("TestDefaultOrigin");
	std::string const TestPath_MinimalRotate_DefaultOrigin = SuitePath_MinimalRotate + std::string("/") + TestName_MinimalRotate_DefaultOrigin;
	BOOST_FIXTURE_TEST_CASE
	(
		TestDefaultOrigin, ISprPerTestFixture_SingleCanvas_Smoke,
		*boost::unit_test::depends_on(SuitePath_MinimalSmoke)
	)
	{
		// Testing that rotation around default sprite origin works well

		SetupTest(TestPath_MinimalRotate_DefaultOrigin.c_str());
		
		TSSpriteVector Sprites;
		int SpriteIndex = PrepareSprite(&Sprites, MySpr::SSpritePickProps::Disabled, GetTestMat_Red());
		TSSprite* pS = &Sprites[SpriteIndex];

		constexpr float NUM_TESTS_PER_90 = 2;
		constexpr int NUM_90_PARTS = 4 * 2 + 1;

		constexpr float SINGLE_TEST_INCREMENT = 90.0F/ NUM_TESTS_PER_90;

		float CurrAngle = 0.0F;
		for (int NintyIndex = 0; NintyIndex < NUM_90_PARTS; NintyIndex++)
		{
			for (int CurrIncrement = 0; CurrIncrement < NUM_TESTS_PER_90; CurrIncrement++)
			{
				BOOST_REQUIRE_NO_THROW(SetRotationAngle(*pS, CurrAngle));

				BOOST_TEST_CHECKPOINT("CommitFrame");
				{
					IFrameCheckContextHandle pChecker = CommitFrame();
					BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, pS->GetHandle(), pS->GetInitUniColor()));
				}

				CurrAngle += SINGLE_TEST_INCREMENT;
			}
		}
	}

	std::string const TestName_MinimalRotate_WithCustomOrigin = std::string("TestWithCustomOrigin");
	std::string const TestPath_MinimalRotate_WithCustomOrigin = SuitePath_MinimalRotate + std::string("/") + TestName_MinimalRotate_WithCustomOrigin;
	BOOST_FIXTURE_TEST_CASE
	(
		TestWithCustomOrigin, ISprPerTestFixture_SingleCanvas_Smoke,
		*boost::unit_test::depends_on(TestPath_MinimalRotate_DefaultOrigin)
	)
	{
		// Testing that rotation around default sprite origin works well

		SetupTest(TestPath_MinimalRotate_DefaultOrigin.c_str());
		
		TSSpriteVector Sprites;
		int SpriteIndex = PrepareSprite
		(
			&Sprites, MySpr::SSpritePickProps::Disabled, GetTestMat_Red(),
			DEFAULT_SPRITE_CANV_SPACE_POSITION, DEFAULT_SPRITE_CANV_SPACE_SIZE,
			MySprMath::SVec2(DEFAULT_SPRITE_CANV_SPACE_SIZE.Width / 2.0F, DEFAULT_SPRITE_CANV_SPACE_SIZE.Height / 2.0F) // Let's take point in the middle to rotate around
		);
		TSSprite* pS = &Sprites[SpriteIndex];

		constexpr float NUM_TESTS_PER_90 = 2;
		constexpr int NUM_90_PARTS = 4 * 2 + 1;

		constexpr float SINGLE_TEST_INCREMENT = 90.0F/ NUM_TESTS_PER_90;

		float CurrAngle = 0.0F;
		for (int NintyIndex = 0; NintyIndex < NUM_90_PARTS; NintyIndex++)
		{
			for (int CurrIncrement = 0; CurrIncrement < NUM_TESTS_PER_90; CurrIncrement++)
			{
				BOOST_REQUIRE_NO_THROW(SetRotationAngle(*pS, CurrAngle));

				BOOST_TEST_CHECKPOINT("CommitFrame");
				{
					IFrameCheckContextHandle pChecker = CommitFrame();
					BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, pS->GetHandle(), pS->GetInitUniColor()));
				}

				CurrAngle += SINGLE_TEST_INCREMENT;
			}
		}
	}

	BOOST_AUTO_TEST_SUITE_END() // SuiteMinimalRotate
} // Test::ISpr