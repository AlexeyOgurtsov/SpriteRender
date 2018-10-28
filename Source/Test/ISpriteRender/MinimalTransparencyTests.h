#pragma once

/**
* Minimal Transparency feature tests that any implementation should provide.
*
* Includes primitive additive alpha-blending.
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
		// We alternatingly show Opaque sprites, then transparent sprite over it, then check result,
		// then again for another opaque/transparent pair.

		SetupTest(TestPath_MinimalTransparency_TestAlpha.c_str());		

		TSSpriteVector Sprites;

		std::vector<EBlendOp> const Ops = { EBlendOp::Additive };

		TSMaterialVector const OpaqueMaterials = GetTSMaterials(10, TSMaterialGenProps::StrictOnlyMain);
		TSMaterialVector const TransparentMaterials =
		{
			GetTestMat_Red(/*Alpha*/EColorBrightnessLevel::Off, /*Brightness*/EColorBrightnessLevel::Highest), 
			GetTestMat_Red(/*Alpha*/EColorBrightnessLevel::Low, /*Brightness*/EColorBrightnessLevel::Highest)
		};

		for (int k = 0; k < static_cast<int>(Ops.size()); k++)
		{
			EBlendOp Op = Ops[k];
			for (int i = 0; i < static_cast<int>(OpaqueMaterials.size()); i++)
			{
				const TSMaterial* pOpaqueMaterial = &OpaqueMaterials[i];

				for (int j = 0; j < static_cast<int>(TransparentMaterials.size()); j++)
				{
					int SprOpaqueIndex = PrepareSprite(&Sprites, *pOpaqueMaterial);

					BOOST_TEST_CHECKPOINT("CommitFrame");
					{
						// Opaque sprite must always override the transparent, when rendered over it
						IFrameCheckContextHandle pChecker = CommitFrame();
						TSSprite* pSprOpaque = &Sprites[SprOpaqueIndex];
						BOOST_REQUIRE(SpriteVisibleAsColor(pChecker, pSprOpaque->GetHandle(), pSprOpaque->GetInitUniColor()));
					}

					const TSMaterial* pTransparentMaterial = &TransparentMaterials[j];

					int SprTransparentIndex = PrepareSprite(&Sprites, SpriteTransparencyMode::AlphaBlend_Additive, *pTransparentMaterial);

					BOOST_TEST_CHECKPOINT("CommitFrame");
					{
						// Transparent material must blend correctly with opaque when rendered over it
						IFrameCheckContextHandle pChecker = CommitFrame();
						TSSprite* pSprTransparent = &Sprites[SprTransparentIndex];
						// WARNING!!! We need to update spr opaque pointer here, because the array me be reallocated!!!
						TSSprite* pSprOpaque = &Sprites[SprOpaqueIndex];
						BOOST_REQUIRE(SpriteBlendedProperly(pChecker, /*Src=*/*pSprTransparent, /*Dest=*/*pSprOpaque, Op));
					}
				}
			}
		}
	}
} // Test::ISpr

BOOST_AUTO_TEST_SUITE_END() // SuiteMinimalTransparency