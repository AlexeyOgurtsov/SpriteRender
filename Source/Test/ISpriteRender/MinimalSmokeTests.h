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
		MarkNeverInteractive();
		SetupTest(TestPath_MinimalSmoke_UpdateBrackets.c_str());

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
		MarkNeverInteractive();
		SetupTest(TestPath_MinimalSmoke_Canvasses.c_str());

		const CanvasId CanvId = 0;
		
		// WARNING!!! Here we should NOT test creating multiple canvasses,
		// because not necessarily that concrete SpriteRender implementation 
		// supports more than once canvas!
		BOOST_TEST_CHECKPOINT("Create");
		BOOST_REQUIRE_NO_THROW(GetSprRen()->CreateCanvas(/*bDebug=*/true, CanvId, std::string("Canvas"), MySpr::SCanvasPickProps::Disabled, GetRTWidth(), GetRTHeight()));
		
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
		MarkNeverInteractive();
		SetupTest(TestPath_MinimalSmoke_Render_Canvas.c_str());

		const CanvasId CanvId = 0;

		// WARNING!!! Here we should NOT test creating multiple canvasses,
		// because not necessarily that concrete SpriteRender implementation 
		// supports more than once canvas!
		BOOST_TEST_CHECKPOINT("CreateCanvas");
		BOOST_REQUIRE_NO_THROW(GetSprRen()->CreateCanvas(/*bDebug=*/true, CanvId, std::string("Canvas"), MySpr::SCanvasPickProps::Disabled, GetRTWidth(), GetRTHeight()));

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
		MarkNeverInteractive();
		SetupTest(TestPath_MinimalSmoke_Render_AllVisibleCanvasses.c_str());

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
	std::string const TestPath_MinimalSmoke_Updater_Sprite = SuitePath_MinimalSmoke_Updater + std::string("/") + TestName_MinimalSmoke_Updater_Sprite;
	BOOST_FIXTURE_TEST_CASE(TestSprite, ISprPerTestFixture_SingleCanvas_Smoke)
	{
		// WARNING!!! We should NOT test for transparency support here, 
		// because not necessarily any sprite render implementation supports transparency!

		SetupTest(TestPath_MinimalSmoke_Updater_Sprite.c_str());

		BOOST_TEST_CHECKPOINT("Create material");		
		const TexelColor INIT_SPRITE_COLOR = TexelColor::GetBlue(GetTextureFormat());
		Handle_SprMaterialInstance const pMat = GetMatInst_Blue_10_10();
		const TexelColor SPRITE_COLOR_TO_CHANGE_TO = TexelColor::GetRed(GetTextureFormat());
		Handle_SprMaterialInstance const pMatToChangeTo = GetMatInst_Red_10_10();

		// ~Sprite params Begin
		const MySprMath::SSize SPR_SIZE = ScreenPart(QUARTER, HALF);
		const MySprMath::SVec2 INIT_POS = PointAt(LEFT, QUARTER_FROM_BOTTOM);
		const MySprMath::SVec2 POSITION_TO_MOVE = PointAt(EIGHTH_FROM_LEFT, CENTER);
		// ~Sprite params End

		TSSprite Sprite( MySpr::SSpritePickProps::Disabled, GetTestMat_Blue(), SPR_SIZE, INIT_POS );

		BOOST_TEST_CHECKPOINT("CreateSprite");		
		{
			BOOST_REQUIRE_NO_THROW
			(
				CreateSprite(Sprite);
			);
		}

		BOOST_TEST_CHECKPOINT("ShowSprite");
		BOOST_REQUIRE_NO_THROW(ShowSprite(Sprite));
		{
			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(false == RT_Cleared(pChecker.get(), /*bMatchAlpha=*/false));
				BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, Sprite.GetHandle(), Sprite.GetInitUniColor()));
			}
		}

		BOOST_TEST_CHECKPOINT("Move");
		{
			BOOST_REQUIRE_NO_THROW
			(
				SetSpritePosition(Sprite, POSITION_TO_MOVE);
			);
			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, Sprite.GetHandle(), Sprite.GetInitUniColor()));
				BOOST_REQUIRE(SpriteHiddenAt(pChecker, Sprite.GetHandle(), INIT_POS));
			}
		}

		BOOST_TEST_CHECKPOINT("UpdateMaterial");
		{
			BOOST_REQUIRE_NO_THROW
			(
				SetSpriteMaterial(Sprite, pMatToChangeTo);
			);
			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, Sprite.GetHandle(), SPRITE_COLOR_TO_CHANGE_TO));
			}
		}

		BOOST_TEST_CHECKPOINT("Hide sprite");
		{
			BOOST_REQUIRE_NO_THROW(HideSprite(Sprite.GetHandle()));

			BOOST_TEST_CHECKPOINT("CommitFrameAfterHide");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteHidden(pChecker, Sprite.GetHandle()));
			}		
		}

		BOOST_TEST_CHECKPOINT("DeleteSprite");
		{
			BOOST_REQUIRE_NO_THROW(DeleteSprite(Sprite));
		}
	} // TestSprite

	std::string const TestName_MinimalSmoke_Updater_FewSprites = std::string("TestFewSprites");
	std::string const TestPath_MinimalSmoke_Updater_FewSprites = SuitePath_MinimalSmoke_Updater + std::string("/") + TestName_MinimalSmoke_Updater_FewSprites;
	BOOST_FIXTURE_TEST_CASE
	(
		TestFewSprites, ISprPerTestFixture_SingleCanvas_Smoke,
		*boost::unit_test::depends_on(TestPath_MinimalSmoke_Updater_Sprite)
	)
	{
		// Testing that different sprites are controlled independently.

		// WARNING!!! We should NOT test for transparency support here, 
		// because not necessarily any sprite render implementation supports transparency!

		SetupTest(TestPath_MinimalSmoke_Updater_FewSprites.c_str());

		/**
		* Material to be used to flush the sprites into a new color.
		*/
		const TSMaterial NEW_MATERIAL = GetTestMat_White();

		TSMaterialVector const SpriteMaterials = GetTSMaterials(10);
		TSSpriteVector Sprites = LayoutSprites(SpriteMaterials);


		BOOST_TEST_CHECKPOINT("Create sprites");
		for (TSSprite& S : Sprites)
		{
			BOOST_REQUIRE_NO_THROW(CreateSprite(S));
		}

		BOOST_TEST_CHECKPOINT("ShowSprite");
		{
			for (TSSprite& S : Sprites)
			{
				BOOST_REQUIRE_NO_THROW(ShowSprite(S));
			}

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				for (TSSprite& S : Sprites)
				{
					BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, S.GetHandle(), S.GetInitUniColor()));
				}
			}
		}

		BOOST_TEST_CHECKPOINT("SetSpriteMaterial");
		{
			for(int i = 0; i < static_cast<int>(Sprites.size()); i++)
			{			
				TSSprite* pS = &Sprites[i];
				BOOST_REQUIRE_NO_THROW(SetSpriteMaterial(*pS, NEW_MATERIAL.GetInitMatInst()));

				BOOST_TEST_CHECKPOINT("CommitFrame");
				{
					IFrameCheckContextHandle pChecker = CommitFrame();
					BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, pS->GetHandle(), NEW_MATERIAL.GetInitUniColor()));
					for (int j = i + 1; j < static_cast<int>(Sprites.size()); j++)
					{
						TSSprite* OldSprite = &Sprites[j];
						BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, OldSprite->GetHandle(), OldSprite->GetInitUniColor()));
					}
				}
			}
		}
	}

	std::string const TestName_MinimalSmoke_Updater_FewSprites_Hide = std::string("TestFewSpritesHide");
	std::string const TestPath_MinimalSmoke_Updater_FewSprites_Hide = SuitePath_MinimalSmoke_Updater + std::string("/") + TestName_MinimalSmoke_Updater_FewSprites_Hide;
	BOOST_FIXTURE_TEST_CASE
	(
		TestFewSpritesHide, ISprPerTestFixture_SingleCanvas_Smoke,
		*boost::unit_test::depends_on(TestPath_MinimalSmoke_Updater_FewSprites)
	)
	{
		// Testing that any sprite in the middle can be hidden, without affecting others.
		SetupTest(TestPath_MinimalSmoke_Updater_FewSprites_Hide.c_str());

		TSMaterialVector const SpriteMaterials = GetTSMaterials(10);
		TSSpriteVector const Sprites = PrepareSprites(SpriteMaterials);

		BOOST_ASSERT_MSG(Sprites.size() >= 7, "Always must be not less than (to prevent indices to be out of range and call hide twice on already hidden sprite)");

		UINT const MiddleSpriteIndex = Sprites.size() / 2;

		BOOST_TEST_CHECKPOINT("Hiding sprite in the middle");
		{
			BOOST_REQUIRE_NO_THROW(HideSprite(Sprites[MiddleSpriteIndex]));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckVisibility(pChecker, Sprites));
			}
		}

		BOOST_TEST_CHECKPOINT("Hiding sprites before and after the middle");
		{
			BOOST_REQUIRE_NO_THROW(HideSprite(Sprites[MiddleSpriteIndex - 1]));
			BOOST_REQUIRE_NO_THROW(HideSprite(Sprites[MiddleSpriteIndex + 1]));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckVisibility(pChecker, Sprites));
			}
		}

		BOOST_TEST_CHECKPOINT("Hiding sprites at the beginning");
		{
			BOOST_REQUIRE_NO_THROW(HideSprite(Sprites[0]));
			BOOST_REQUIRE_NO_THROW(HideSprite(Sprites[1]));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckVisibility(pChecker, Sprites));
			}
		}

		BOOST_TEST_CHECKPOINT("Hiding sprites at the end");
		{
			BOOST_REQUIRE_NO_THROW(HideSprite(Sprites[Sprites.size() - 1]));
			BOOST_REQUIRE_NO_THROW(HideSprite(Sprites[Sprites.size() - 2]));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_TEST(CheckVisibility(pChecker, Sprites));
			}
		}
	}

	std::string const TestName_MinimalSmoke_Updater_SpriteDefaultZOrder = std::string("TestSpriteDefaultZOrder");
	std::string const TestPath_MinimalSmoke_Updater_SpriteDefaultZOrder = SuitePath_MinimalSmoke_Updater + std::string("/") + TestName_MinimalSmoke_Updater_SpriteDefaultZOrder;
	BOOST_FIXTURE_TEST_CASE
	(
		TestSpriteDefaultZOrder, ISprPerTestFixture_SingleCanvas_Smoke,
		*boost::unit_test::depends_on(TestPath_MinimalSmoke_Updater_FewSprites_Hide)
	)
	{
		// Testing that default ZOrder of created sprites is correct:
		// Newest sprite is always added on top by default:
		SetupTest(TestPath_MinimalSmoke_Updater_SpriteDefaultZOrder.c_str());

		TSMaterialVector const SpriteMaterials = GetTSMaterials(10);
		TSSpriteVector const Sprites = PrepareSprites(SpriteMaterials);

		const int MaxVisibleSpriteIndex = static_cast<int>(Sprites.size() - 1);
		const TSSprite* pMaxZOrderSprite = &(Sprites.back());

		// Not: here we created sprites, that are not overlapped, so user can see them and their order in interactive mode

		BOOST_TEST_CHECKPOINT("Moving to position of sprite with maximal ZOrder");
		// Moving to position of sprite with maximum Z order should not affect visibility of the sprite with maximum visibility

		for (int i = 0; i < MaxVisibleSpriteIndex; i++)
		{
			const TSSprite* pSpriteToMove = &Sprites[i];
			BOOST_REQUIRE_NO_THROW(SetSpritePosition(*pSpriteToMove, pMaxZOrderSprite->GetInitPos()));

			BOOST_TEST_CHECKPOINT("CommitFrame");
			{
				IFrameCheckContextHandle pChecker = CommitFrame();
				BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, pMaxZOrderSprite->GetHandle(), pMaxZOrderSprite->GetInitUniColor()));
			}
		}
	}

	BOOST_AUTO_TEST_SUITE_END() // SuiteUpdater

	BOOST_AUTO_TEST_SUITE_END() // SuiteMinimalSmoke
} // Test::ISpr