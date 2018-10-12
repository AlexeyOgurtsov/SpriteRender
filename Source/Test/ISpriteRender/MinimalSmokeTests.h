#pragma once

/**
* Minimal smoke tests.
*
* Should NEVER contain sofisticated tests or corner cases tests
* ( just "Should work" tests ).
*
* All other unit tests should depend on this set of tests!
*/

#include <boost/test/included/unit_test.hpp>
#include "ISprPerTestFixture_Custom.h"
#include "ISprPerTestFixture_SingleCanvas_Smoke.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE(SuiteMinimalSmoke)

	std::string const TestName_MinimalSmoke_UpdateBrackets = std::string("TestUpdateBrackets");
	std::string const TestPath_MinimalSmoke_UpdateBrackets = SuitePath_MinimalSmoke + std::string("/") + TestName_MinimalSmoke_UpdateBrackets;
	BOOST_FIXTURE_TEST_CASE(TestUpdateBrackets, ISprPerTestFixture_Custom)
	{
		SetupTest(TestName_MinimalSmoke_UpdateBrackets.c_str());

		BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginUpdates());
		BOOST_REQUIRE_NO_THROW(GetSprRen()->EndUpdates());
	}

	std::string const TestName_MinimalSmoke_Canvasses = std::string("TestCanvasses");
	std::string const TestPath_MinimalSmoke_Canvasses = SuitePath_MinimalSmoke + std::string("/") + TestName_MinimalSmoke_Canvasses;
	BOOST_FIXTURE_TEST_CASE
	(
		TestCanvasses, ISprPerTestFixture_Custom, 
		*boost::unit_test::depends_on(TestPath_MinimalSmoke_UpdateBrackets.c_str())
	)
	{
		SetupTest(TestName_MinimalSmoke_Canvasses.c_str());

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

	BOOST_AUTO_TEST_SUITE(SuiteRender, *boost::unit_test::depends_on(TestPath_MinimalSmoke_Canvasses.c_str()))

	std::string const TestName_MinimalSmoke_Render_Canvas = std::string("TestCanvas");
	std::string const TestPath_MinimalSmoke_Render_Canvas = SuitePath_MinimalSmoke_Render + std::string("/") + TestName_MinimalSmoke_Render_Canvas;
	BOOST_FIXTURE_TEST_CASE(TestCanvas, ISprPerTestFixture_Custom)
	{
		SetupTest(TestName_MinimalSmoke_Render_Canvas.c_str());

		const CanvasId CanvId = 0;

		// WARNING!!! Here we should NOT test creating multiple canvasses,
		// because not necessarily that concrete SpriteRender implementation 
		// supports more than once canvas!
		BOOST_TEST_CHECKPOINT("CreateCanvas");
		BOOST_REQUIRE_NO_THROW(GetSprRen()->CreateCanvas(CanvId, GetRTWidth(), GetRTHeight()));

		BOOST_TEST_CHECKPOINT("ShowCanvas");
		BOOST_REQUIRE_NO_THROW(GetSprRen()->ShowCanvas(CanvId));

		BOOST_TEST_CHECKPOINT("Render");
		{
			BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginFrame());
			BOOST_REQUIRE_NO_THROW(GetSprRen()->RenderCanvas(CanvId));
			BOOST_REQUIRE_NO_THROW(GetSprRen()->EndFrame());
		}
	}

	std::string const TestName_MinimalSmoke_Render_AllVisibleCanvasses = std::string("TestAllVisibleCanvasses");
	std::string const TestPath_MinimalSmoke_Render_AllVisibleCanvasses = SuitePath_MinimalSmoke + std::string("/") + TestName_MinimalSmoke_Render_AllVisibleCanvasses;
	BOOST_FIXTURE_TEST_CASE
	(
		TestAllVisibleCanvasses, ISprPerTestFixture_Custom,
		*boost::unit_test::depends_on(TestPath_MinimalSmoke_Render_Canvas)
	)
	{
		SetupTest(TestName_MinimalSmoke_Render_AllVisibleCanvasses.c_str());

		BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginFrame());
		BOOST_REQUIRE_NO_THROW(GetSprRen()->RenderAllVisibleCanvasses());
		BOOST_REQUIRE_NO_THROW(GetSprRen()->EndFrame());
	}

	BOOST_AUTO_TEST_SUITE_END() // SuiteRender

	BOOST_AUTO_TEST_SUITE
	(
		SuiteUpdater, 
		*boost::unit_test::depends_on(SuitePath_MinimalSmoke_Render.c_str())
	)

	std::string const TestName_MinimalSmoke_Updater_Sprite = std::string("TestSprite");
	std::string const TestPath_MinimalSmoke_Updater_Sprite = SuitePath_MinimalSmoke + std::string("/") + TestName_MinimalSmoke_Updater_Sprite;
	BOOST_FIXTURE_TEST_CASE(TestSprite, ISprPerTestFixture_SingleCanvas_Smoke)
	{
		// WARNING!!! We should NOT test for transparency support here, 
		// because not necessarily any sprite render implementation supports transparency!

		SetupTest(TestName_MinimalSmoke_Updater_Sprite.c_str());

		BOOST_TEST_CHECKPOINT("Create material");
		Handle_SprMaterialInstance const pMat = GetMatInst_Blue_10_10();

		BOOST_TEST_CHECKPOINT("CreateSprite");
		constexpr float SPRITE_WIDTH = 2.0F / 5;
		constexpr float SPRITE_HEIGHT = 2.0F / 5;
		
		SprId SPR_ID = -1;
		BOOST_REQUIRE_NO_THROW
		(
			SPR_ID = CreateSprite(MySprMath::SVec2{0.0F, 0.0F}, SPRITE_WIDTH, SPRITE_HEIGHT, pMat);
		);

		BOOST_REQUIRE_NO_THROW(ShowSprite(SPR_ID));

		BOOST_TEST_CHECKPOINT("CommitFrame");
		{
			IFrameCheckContextHandle pChecker = CommitFrame();
			// @TODO: Check frame result
		}

		BOOST_TEST_CHECKPOINT("Hide sprite");
		BOOST_REQUIRE_NO_THROW(HideSprite(SPR_ID));

		//BOOST_TEST_CHECKPOINT("CommitFrameAfterHide");
		//{
		//	IFrameCheckContextHandle pChecker = CommitFrame();
		//	// @TODO: Here we may check render result again that sprite is not rendered
		//}		

		BOOST_TEST_CHECKPOINT("DeleteSprite");
		BOOST_REQUIRE_NO_THROW(DeleteSprite(SPR_ID));
	}

	BOOST_AUTO_TEST_SUITE_END() // SuiteUpdater

	BOOST_AUTO_TEST_SUITE_END() // SuiteMinimalSmoke
} // Test::ISpr