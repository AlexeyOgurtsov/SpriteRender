#pragma once

/**
* Minimal smoke tests for feature of using multiple canvasses.
*
* Should NEVER contain sofisticated tests or corner cases tests
* ( just "Should work" tests ).
*
* All other unit tests should depend on this set of tests!
*/

#include <boost/test/included/unit_test.hpp>
#include "ISprPerTestFixture_Custom.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	BOOST_AUTO_TEST_SUITE(Suite_FeatureManyCanvasSmoke)

	std::string const TestName_FeatureManyCanvasSmoke_Canvasses = std::string("TestManyCanvasses");
	std::string const TestPath_FeatureManyCanvasSmoke_Canvasses = SuitePath_FeatureManyCanvassesSmoke + std::string("/") + TestName_FeatureManyCanvasSmoke_Canvasses;
	BOOST_FIXTURE_TEST_CASE
	(
		TestManyCanvasses, ISprPerTestFixture_Custom, 
		*boost::unit_test::depends_on(SuitePath_MinimalSmoke.c_str())
	)
	{
		MarkNeverInteractive();
		SetupTest(TestPath_FeatureManyCanvasSmoke_Canvasses.c_str());

		BOOST_TEST_CHECKPOINT("Initial checks");
		BOOST_REQUIRE_EQUAL(0, GetSprRen()->GetNumVisibleCanvasses());
		BOOST_REQUIRE_EQUAL(0, GetSprRen()->GetNumHiddenCanvasses());
		BOOST_REQUIRE_EQUAL(0, GetSprRen()->GetNumCanvasses());

		struct Canv
		{
			CanvasId CanvId;

			Canv(CanvasId InCanvId) :
				CanvId{ InCanvId } {}
		};
		std::vector<Canv> Canvasses =
		{
			Canv{0},
			Canv{1},
			Canv{2},
			Canv{3},
			Canv{4},
			Canv{5},
		};		
		BOOST_ASSERT_MSG(static_cast<int>(Canvasses.size()) <= static_cast<int>(GetSprRen()->GetMaxCanvasLimit()), "TestManyCanvasses: count of canvasses must always be not greater than total allowed canvas limit!");
		
		BOOST_TEST_CHECKPOINT("Create");
		{
			for (int i = 0; i < static_cast<int>(Canvasses.size()); i++)			
			{
				// WARNING!!! Do check BEFORE
				BOOST_REQUIRE_EQUAL(i, GetSprRen()->GetNumCanvasses());
				BOOST_REQUIRE_NO_THROW(GetSprRen()->CreateCanvas(/*bDebug=*/true, Canvasses[i].CanvId, std::string("Canvas") + std::to_string(i), GetRTWidth(), GetRTHeight()));
			}
			BOOST_REQUIRE_EQUAL(Canvasses.size(), GetSprRen()->GetNumCanvasses());
			BOOST_REQUIRE_EQUAL(Canvasses.size(), GetSprRen()->GetNumHiddenCanvasses());
			BOOST_REQUIRE_EQUAL(0, GetSprRen()->GetNumVisibleCanvasses());
		}
		
		BOOST_TEST_CHECKPOINT("Show");
		{
			for (int i = 0; i < static_cast<int>(Canvasses.size()); i++)
			{
				BOOST_TEST_CHECKPOINT("ShowOnce");
				BOOST_REQUIRE_EQUAL(i, GetSprRen()->GetNumVisibleCanvasses());
				BOOST_REQUIRE_NO_THROW(GetSprRen()->ShowCanvas(Canvasses[i].CanvId));

				BOOST_TEST_CHECKPOINT("ShowSecond");
				BOOST_REQUIRE_EQUAL(i+1, GetSprRen()->GetNumVisibleCanvasses());
				BOOST_REQUIRE_NO_THROW(GetSprRen()->ShowCanvas(Canvasses[i].CanvId));
			}
			BOOST_REQUIRE_EQUAL(Canvasses.size(), GetSprRen()->GetNumCanvasses());
			BOOST_REQUIRE_EQUAL(0, GetSprRen()->GetNumHiddenCanvasses());
			BOOST_REQUIRE_EQUAL(Canvasses.size(), GetSprRen()->GetNumVisibleCanvasses());
		}

		BOOST_TEST_CHECKPOINT("Delete_The_LastCanvas_AndCheck_ThatCanvasCounters_AreValid");
		{
			GetSprRen()->DeleteCanvas(Canvasses[Canvasses.size() - 1].CanvId);
			BOOST_REQUIRE_EQUAL((Canvasses.size()-1), GetSprRen()->GetNumCanvasses());
			BOOST_REQUIRE_EQUAL((Canvasses.size()-1), GetSprRen()->GetNumVisibleCanvasses());

			Canvasses.pop_back();
		}

		BOOST_TEST_CHECKPOINT("Hide");
		{
			for (int i = 0; i < static_cast<int>(Canvasses.size()); i++)
			{
				BOOST_TEST_CHECKPOINT("HideOnce");
				BOOST_REQUIRE_EQUAL(i, GetSprRen()->GetNumHiddenCanvasses());
				BOOST_REQUIRE_NO_THROW(GetSprRen()->HideCanvas(Canvasses[i].CanvId));

				BOOST_TEST_CHECKPOINT("HideSecond");
				BOOST_REQUIRE_EQUAL((i+1), GetSprRen()->GetNumHiddenCanvasses());
				BOOST_REQUIRE_NO_THROW(GetSprRen()->HideCanvas(Canvasses[i].CanvId));
			}
		}
		BOOST_REQUIRE_EQUAL(Canvasses.size(), GetSprRen()->GetNumCanvasses());
		BOOST_REQUIRE_EQUAL(GetSprRen()->GetNumCanvasses(), GetSprRen()->GetNumHiddenCanvasses());
		BOOST_REQUIRE_EQUAL(0, GetSprRen()->GetNumVisibleCanvasses());

		BOOST_TEST_CHECKPOINT("Update");
		{
			BOOST_REQUIRE_NO_THROW(GetSprRen()->BeginUpdates());
			for (int i = 0; i < static_cast<int>(Canvasses.size()); i++)
			{
				MySprRen::ISpriteUpdater* pUpdater = GetSprRen()->BeginCanvasUpdate(Canvasses[i].CanvId);
				BOOST_REQUIRE(pUpdater);
				BOOST_REQUIRE_NO_THROW(GetSprRen()->EndCanvasUpdate(pUpdater));
			}
			BOOST_REQUIRE_NO_THROW(GetSprRen()->EndUpdates());
		}

		BOOST_TEST_CHECKPOINT("Delete");
		{
			for (int i = 0; i < static_cast<int>(Canvasses.size()); i++)
			{
				BOOST_REQUIRE_EQUAL((Canvasses.size() - i), GetSprRen()->GetNumCanvasses());
				BOOST_REQUIRE_NO_THROW(GetSprRen()->DeleteCanvas(Canvasses[i].CanvId));
			}
			BOOST_REQUIRE_EQUAL(0, GetSprRen()->GetNumCanvasses());
			BOOST_REQUIRE_EQUAL(0, GetSprRen()->GetNumVisibleCanvasses());
			BOOST_REQUIRE_EQUAL(0, GetSprRen()->GetNumHiddenCanvasses());
		}
	}

	/*
	BOOST_AUTO_TEST_SUITE(SuiteRender, *boost::unit_test::depends_on(TestPath_MinimalSmoke_Canvasses.c_str()))

	std::string const TestName_MinimalSmoke_Render_Canvas = std::string("TestCanvas");
	std::string const TestPath_MinimalSmoke_Render_Canvas = SuitePath_FeatureManyCanvassesSmoke + std::string("/") + TestName_MinimalSmoke_Render_Canvas;
	BOOST_FIXTURE_TEST_CASE(TestCanvas, ISprPerTestFixture_Custom)
	{
		MarkNeverInteractive();
		SetupTest(TestPath_MinimalSmoke_Render_Canvas.c_str());

		const CanvasId CanvId = 0;

		// WARNING!!! Here we should NOT test creating multiple canvasses,
		// because not necessarily that concrete SpriteRender implementation 
		// supports more than once canvas!
		BOOST_TEST_CHECKPOINT("CreateCanvas");
		BOOST_REQUIRE_NO_THROW(GetSprRen()->CreateCanvas(CanvId, std::string("Canvas"), GetRTWidth(), GetRTHeight()));

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
	std::string const TestPath_MinimalSmoke_Render_AllVisibleCanvasses = SuitePath_FeatureManyCanvassesSmoke + std::string("/") + TestName_MinimalSmoke_Render_AllVisibleCanvasses;
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
	*/

	BOOST_AUTO_TEST_SUITE_END() // Suite_FeatureManyCanvasSmoke
} // Test::ISpr