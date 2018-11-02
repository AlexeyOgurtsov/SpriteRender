#pragma once

/**
* Minimal smoke tests for feature of picking.
*
* Should NEVER contain sofisticated tests or corner cases tests
* ( just "Should work" tests ).
*/

#include <boost/test/included/unit_test.hpp>
#include "ISprPerTestFixture_CustomPick.h"
#include "ISprPerTestFixture_SingleCanvas_SmokePick.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE(Suite_FeaturePick)
	BOOST_AUTO_TEST_SUITE(Suite_FeaturePickSmoke)

	std::string const TestName_FeaturePickSmoke_PickBrackets = std::string("TestPickBrackets");
	std::string const TestPath_FeaturePickSmoke_PickBrackets = SuitePath_FeaturePickSmoke + std::string("/") + TestName_FeaturePickSmoke_PickBrackets;

	BOOST_FIXTURE_TEST_CASE(TestPickBrackets, ISprPerTestFixture_CustomPick)
	{
		BOOST_ASSERT_MSG(GetConfig().Pick.bEnabled, "The test demands that picking is enabled in the config (maybe you forget to use the correct fixture for the test suite?)");

		MarkNeverInteractive();
		SetupTest(TestPath_FeaturePickSmoke_PickBrackets.c_str());

		BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginPickFrame());
		BOOST_REQUIRE_NO_THROW(GetSprRen()->EndPickFrame());	
	}

	std::string const TestName_FeaturePickSmoke_PickCanvasses = std::string("TestPickCanvasses");
	std::string const TestPath_FeaturePickSmoke_PickCanvasses = SuitePath_FeaturePickSmoke + std::string("/") + TestName_FeaturePickSmoke_PickCanvasses;
	BOOST_FIXTURE_TEST_CASE
	(
		TestPickCanvasses, ISprPerTestFixture_CustomPick,
		*boost::unit_test::depends_on(TestPath_FeaturePickSmoke_PickBrackets.c_str())
	)
	{
		BOOST_ASSERT_MSG(GetConfig().Pick.bEnabled, "The test demands that picking is enabled in the config (maybe you forget to use the correct fixture for the test suite?)");

		MarkNeverInteractive();
		SetupTest(TestPath_FeaturePickSmoke_PickCanvasses.c_str());

		const CanvasId CanvId = 0;

		// WARNING!!! Here we should NOT test creating multiple canvasses,
		// because not necessarily that concrete SpriteRender implementation 
		// supports more than once canvas!
		BOOST_TEST_CHECKPOINT("Create");
		//  We are creating here sprite with enabled pick initially
		BOOST_REQUIRE_NO_THROW(GetSprRen()->CreateCanvas(/*bDebug=*/true, CanvId, std::string("Canvas"), MySpr::SCanvasPickProps::Enabled, GetRTWidth(), GetRTHeight()));

		BOOST_TEST_CHECKPOINT("Enable/Disable picking");
		{
			BOOST_REQUIRE_NO_THROW(GetSprRen()->DisableCanvasPicking(CanvId));
			BOOST_REQUIRE_NO_THROW(GetSprRen()->DisableCanvasPicking(CanvId));

			BOOST_REQUIRE_NO_THROW(GetSprRen()->EnableCanvasPicking(CanvId));
			BOOST_REQUIRE_NO_THROW(GetSprRen()->EnableCanvasPicking(CanvId));
		}

		BOOST_TEST_CHECKPOINT("Disable picking second time");
		{
			BOOST_REQUIRE_NO_THROW(GetSprRen()->DisableCanvasPicking(CanvId));
			BOOST_REQUIRE_NO_THROW(GetSprRen()->DisableCanvasPicking(CanvId));
		}

		BOOST_TEST_CHECKPOINT("BeginPickFrame/EndPickFrame");
		{
			BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginPickFrame());
			BOOST_REQUIRE_NO_THROW(GetSprRen()->RenderAllPickableCanvasses());
			BOOST_REQUIRE_NO_THROW(GetSprRen()->EndPickFrame());
		}

		BOOST_TEST_CHECKPOINT("Delete");
		{
			BOOST_REQUIRE_NO_THROW(GetSprRen()->DeleteCanvas(CanvId));
		}
	}

	BOOST_AUTO_TEST_SUITE(SuitePickRender, *boost::unit_test::depends_on(TestPath_FeaturePickSmoke_PickBrackets.c_str()))

	std::string const TestName_MinimalSmoke_Render_PickableCanvas = std::string("TestPickableCanvas");
	std::string const TestPath_MinimalSmoke_Render_PickableCanvas = SuitePath_FeaturePick_Render + std::string("/") + TestName_MinimalSmoke_Render_PickableCanvas;
	BOOST_FIXTURE_TEST_CASE(TestCanvasPickFrame, ISprPerTestFixture_CustomPick)
	{
		BOOST_ASSERT_MSG(GetConfig().Pick.bEnabled, "The test demands that picking is enabled in the config (maybe you forget to use the correct fixture for the test suite?)");

		MarkNeverInteractive();
		SetupTest(TestPath_MinimalSmoke_Render_PickableCanvas.c_str());

		const CanvasId CanvId = 0;

		// WARNING!!! Here we should NOT test creating multiple canvasses,
		// because not necessarily that concrete SpriteRender implementation 
		// supports more than once canvas!
		BOOST_TEST_CHECKPOINT("CreateCanvas");
		BOOST_REQUIRE_NO_THROW(GetSprRen()->CreateCanvas(/*bDebug=*/true, CanvId, std::string("Canvas"), MySpr::SCanvasPickProps::Enabled, GetRTWidth(), GetRTHeight()));

		BOOST_TEST_CHECKPOINT("ShowCanvas");
		BOOST_REQUIRE_NO_THROW(GetSprRen()->ShowCanvas(CanvId));

		BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginPickFrame());
		BOOST_REQUIRE_NO_THROW(GetSprRen()->RenderPickableCanvas(CanvId));
		BOOST_REQUIRE_NO_THROW(GetSprRen()->EndPickFrame());
	}

	std::string const TestName_MinimalSmoke_Render_AllPickableCanvasses = std::string("TestAllPickableCanvasses");
	std::string const TestPath_MinimalSmoke_Render_AllPickableCanvasses = SuitePath_FeaturePick_Render + std::string("/") + TestName_MinimalSmoke_Render_AllPickableCanvasses;
	BOOST_FIXTURE_TEST_CASE(TestAllPickableCanvasses, ISprPerTestFixture_CustomPick)
	{
		BOOST_ASSERT_MSG(GetConfig().Pick.bEnabled, "The test demands that picking is enabled in the config (maybe you forget to use the correct fixture for the test suite?)");

		MarkNeverInteractive();
		SetupTest(TestPath_MinimalSmoke_Render_AllPickableCanvasses.c_str());

		BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginPickFrame());
		BOOST_REQUIRE_NO_THROW(GetSprRen()->RenderAllPickableCanvasses());
		BOOST_REQUIRE_NO_THROW(GetSprRen()->EndPickFrame());
	}

	BOOST_AUTO_TEST_SUITE_END() // SuitePickRender

	BOOST_AUTO_TEST_SUITE
	(
		SuitePickResult,
		*boost::unit_test::depends_on(SuitePath_FeaturePick_Render.c_str())
	);

	std::string const TestName_MinimalSmoke_PickResult_Sprite = std::string("TestSpritePick");
	std::string const TestPath_MinimalSmoke_PickResult_Sprite = SuitePath_FeaturePick_PickResult + std::string("/") + TestName_MinimalSmoke_PickResult_Sprite;
	BOOST_FIXTURE_TEST_CASE(TestSpritePick, ISprPerTestFixture_SingleCanvas_SmokePick)
	{
		// WARNING!!! We should NOT test for transparency support here, 
		// because not necessarily any sprite render implementation supports transparency!

		SetupTest(TestName_MinimalSmoke_PickResult_Sprite.c_str());		

		BOOST_TEST_CHECKPOINT("Prepare pickable sprite");

		const MySprMath::SSize SPR_SIZE = ScreenPart(QUARTER, HALF);
		const MySprMath::SVec2 INIT_POS = PointAt(LEFT, QUARTER_FROM_BOTTOM);
		const MySprMath::SVec2 POSITION_TO_MOVE = PointAt(EIGHTH_FROM_LEFT, CENTER);

		MySpr::PickObjectId const ObjId = DEFAULT_PICK_OBJECT_ID;
		MySpr::PickObjectId const NewObjectId = (ObjId + 1);
		BOOST_ASSERT_MSG(ObjId != NewObjectId, " TestPath_MinimalSmoke_PickResult_Sprite: object id must differ");
		TSSpriteVector Sprites;
		int const SpriteIndex = PreparePickSprite
		(
			&Sprites, ObjId, GetTestMat_Green(), INIT_POS, SPR_SIZE
		);
		TSSprite& S = Sprites[SpriteIndex];

		BOOST_TEST_CHECKPOINT("Checking that sprite is pickable");
		{
			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpritePickedAs(pChecker, S.GetHandle(), ObjId));
			}
		}

		BOOST_TEST_CHECKPOINT("After sprite is hidden, it's still must be pickable");
		{
			HideSprite(S);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpritePickedAs(pChecker, S.GetHandle(), ObjId));
			}
		}

		BOOST_TEST_CHECKPOINT("After moved the sprite must be non-picked at the old position and picked on the new");
		{
			BOOST_REQUIRE_NO_THROW
			(
				SetSpritePosition(S, POSITION_TO_MOVE);
			);
			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				// For extra check:
				BOOST_REQUIRE(SpritePickedAs(pChecker, S.GetHandle(), ObjId));
				BOOST_REQUIRE(SpriteNotPickedAt(pChecker, S.GetHandle(), INIT_POS));
			}
		}

		BOOST_TEST_CHECKPOINT("Disable picking");
		{
			DisableSpritePick(S);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteNotPicked(pChecker, S.GetHandle()));
			}
		}


		BOOST_TEST_CHECKPOINT("Disable picking second time");
		{
			DisableSpritePick(S);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteNotPicked(pChecker, S.GetHandle()));
			}
		}

		BOOST_TEST_CHECKPOINT("When sprite is not pickable, showing it should not make it pickable");
		{
			ShowSprite(S);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteNotPicked(pChecker, S.GetHandle()));
			}
		}

		BOOST_TEST_CHECKPOINT("Enabling pick state should make sprite pickable");
		{
			EnableSpritePick(S);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpritePickedAs(pChecker, S.GetHandle(), ObjId));
			}
		}

		BOOST_TEST_CHECKPOINT("Enabling pick state second time");
		{
			EnableSpritePick(S);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpritePickedAs(pChecker, S.GetHandle(), ObjId));
			}
		}

		BOOST_TEST_CHECKPOINT("Setting up object id");
		{
			SetSpriteObjectId(S, NewObjectId);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpritePickedAs(pChecker, S.GetHandle(), NewObjectId));
			}
		}

		BOOST_TEST_CHECKPOINT("After making sprite non-picked or hiding, assigned pick object id must conserve");
		{
			HideSprite(S);
			DisableSpritePick(S);
			ShowSprite(S);
			EnableSpritePick(S);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpritePickedAs(pChecker, S.GetHandle(), NewObjectId));
			}
		}
	}

	std::string const TestName_FeaturePickSmoke_PickResult_PickFewSprites = std::string("TestPickFewSprites");
	std::string const TestPath_FeaturePickSmoke_PickResult_PickFewSprites = SuitePath_FeaturePick_PickResult + std::string("/") + TestName_FeaturePickSmoke_PickResult_PickFewSprites;
	BOOST_FIXTURE_TEST_CASE
	(
		TestPickFewSprites, ISprPerTestFixture_SingleCanvas_SmokePick,
		*boost::unit_test::depends_on(TestPath_MinimalSmoke_PickResult_Sprite)
	)
	{
		// Testing that different sprites may be assigned with a different object id,
		// and that some sprites of the same canvas may be pickable, while others at the same time are not.

		// WARNING!!! We should NOT test for transparency support here, 
		// because not necessarily any sprite render implementation supports transparency!

		SetupTest(TestPath_FeaturePickSmoke_PickResult_PickFewSprites.c_str());

		TSMaterialVector const SpriteMaterials = GetTSMaterials(10);
		TSSpriteVector Sprites = PrepareSprites(SpriteMaterials);

		BOOST_TEST_CHECKPOINT("Initially prepared sprites are not pickable");
		{
			IFrameCheckContextHandle pChecker = CommitFrame();
			BOOST_TEST(CheckAllNotPickable(pChecker, Sprites));
		}

		BOOST_TEST_CHECKPOINT("Assign id and check non-pickable still");
		{
			AssignSpriteObjectIds(Sprites);

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckAllNotPickable(pChecker, Sprites));
			}
		}

		BOOST_TEST_CHECKPOINT("Enable picking for all and check");
		{
			for (int i = 0; i < static_cast<int>(Sprites.size()); i++)
			{
				TSSprite* pS = &Sprites[i];
				BOOST_REQUIRE_NO_THROW(EnableSpritePick(*pS));

				BOOST_TEST_CHECKPOINT("CommitFrame");
				{
					IFrameCheckContextHandle pChecker = CommitFrame();
					BOOST_TEST(CheckPickability(pChecker, Sprites));
				}
			}
		}
	}


	std::string const TestName_FeaturePickSmoke_PickResult_PickFewSprites_Hide = std::string("TestPickFewSpritesHide");
	std::string const TestPath_FeaturePickSmoke_PickResult_PickFewSprites_Hide = SuitePath_FeaturePick_PickResult + std::string("/") + TestName_FeaturePickSmoke_PickResult_PickFewSprites_Hide;
	BOOST_FIXTURE_TEST_CASE
	(
		TestPickFewSpritesHide, ISprPerTestFixture_SingleCanvas_SmokePick,
		*boost::unit_test::depends_on(TestPath_FeaturePickSmoke_PickResult_PickFewSprites)
	)
	{
		// Testing that pickability of any sprite in the middle can be changed, without affecting others.
		SetupTest(TestPath_FeaturePickSmoke_PickResult_PickFewSprites_Hide.c_str());

		TSMaterialVector const SpriteMaterials = GetTSMaterials(10);
		TSSpriteVector Sprites = PrepareSprites(SpriteMaterials);

		BOOST_ASSERT_MSG(Sprites.size() >= 7, "Always must be not less than (to prevent indices to be out of range)");		
		AssignSpriteObjectIds(Sprites);

		UINT const MiddleSpriteIndex = Sprites.size() / 2;

		BOOST_TEST_CHECKPOINT("Making pickable in the middle");
		{
			BOOST_REQUIRE_NO_THROW(EnableSpritePick(Sprites[MiddleSpriteIndex]));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckPickability(pChecker, Sprites));
			}
		}

		BOOST_TEST_CHECKPOINT("Making pickable before and after the middle");
		{
			BOOST_REQUIRE_NO_THROW(EnableSpritePick(Sprites[MiddleSpriteIndex - 1]));
			BOOST_REQUIRE_NO_THROW(EnableSpritePick(Sprites[MiddleSpriteIndex + 1]));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckPickability(pChecker, Sprites));
			}
		}

		BOOST_TEST_CHECKPOINT("Making sprites pickable at the beginning");
		{
			BOOST_REQUIRE_NO_THROW(EnableSpritePick(Sprites[0]));
			BOOST_REQUIRE_NO_THROW(EnableSpritePick(Sprites[1]));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckPickability(pChecker, Sprites));
			}
		}

		BOOST_TEST_CHECKPOINT("Making sprites at the end pickable");
		{
			BOOST_REQUIRE_NO_THROW(EnableSpritePick(Sprites[Sprites.size() - 1]));
			BOOST_REQUIRE_NO_THROW(EnableSpritePick(Sprites[Sprites.size() - 2]));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckPickability(pChecker, Sprites));
			}
		}
	}

	BOOST_AUTO_TEST_SUITE_END() // SuitePickResult

	BOOST_AUTO_TEST_SUITE_END() // Suite_FeaturePickSmoke
	BOOST_AUTO_TEST_SUITE_END() // Suite_FeaturePick
} // Test::ISpr