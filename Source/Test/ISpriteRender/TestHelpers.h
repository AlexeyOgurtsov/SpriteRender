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
	// ~ Sprite render test suite paths End
} // Test::ISpr