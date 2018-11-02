#pragma once

/**
* To be included in any header that contains unit tests.
*/

#include "ISpriteRender/ISpriteRender.h"
#include "ISpriteRenderTestUtils/SprRenHelper/SprRenHelper_System.h"
#include "ISpriteRenderTestUtils/SprRenHelper/SprRenHelper_ISpriteRenderFwd.h"
#include "boost/test/predicate_result.hpp"
#include <string>

namespace Test::ISpr
{
	// ~ Sprite render test suite paths Begin
	std::string const SuitePath_MinimalSmoke { "SuiteMinimalSmoke" };
	std::string const SuiteName_Render { "SuiteRender" };
	std::string const SuitePath_MinimalSmoke_Render = SuitePath_MinimalSmoke + std::string("/") + SuiteName_Render;
	std::string const SuiteName_Updater { "SuiteUpdater" };
	std::string const SuitePath_MinimalSmoke_Updater = SuitePath_MinimalSmoke + std::string("/") + SuiteName_Updater;
		
	std::string const SuitePath_MinimalRotate { "SuiteMinimalRotate" };
	std::string const SuitePath_MinimalMoveZOrder { "SuiteMinimalMoveZOrder" };
	std::string const SuitePath_MinimalTransparency { "SuiteMinimalTransparency" };
	std::string const SuitePath_MinimalHeavyLoad { "SuiteMinimalHeavyLoad" };
	std::string const SuitePath_MinimalExtraChecks { "SuiteMinimalExtraChecks" };

	/**
	* Suite that contains all tests of the Pick feature.
	*/
	std::string const SuitePath_FeaturePick = "Suite_FeaturePick";
	std::string const SuitePath_FeaturePickSmoke { SuitePath_FeaturePick + std::string("/") + std::string("Suite_FeaturePickSmoke") };
	std::string const SuiteName_PickRender = "SuitePickRender";
	std::string const SuitePath_FeaturePick_Render = SuitePath_FeaturePickSmoke + std::string("/") + SuiteName_PickRender;
	std::string const SuiteName_PickResult = "SuitePickResult";
	std::string const SuitePath_FeaturePick_PickResult = SuitePath_FeaturePickSmoke + std::string("/") + SuiteName_PickResult;

	std::string const SuitePath_FeatureManyCanvassesSmoke { std::string("Suite_FeatureManyCanvasSmoke") };
	// ~ Sprite render test suite paths End
} // Test::ISpr