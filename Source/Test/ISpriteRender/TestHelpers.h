#pragma once

/**
* To be included in any header that contains unit tests.
*/
#include "ISpriteRenderTestUtils/ISpriteRenderFwd.h"
#include "ISpriteRender/ISpriteRender.h"
#include <string>

namespace Test::ISpr
{
	// ~ Sprite render namespaces Begin
	namespace MySpr = ::Dv::Spr;
	namespace MySprRen = ::Dv::Spr::Ren;
	namespace MySprMath = ::Dv::Spr::Math;
	// ~ Sprite render namespaces End
	
	// ~ Sprite render typedefs Begin
	using CanvasId = ::Dv::Spr::SpriteCanvasId;
	using SprId = ::Dv::Spr::SpriteId;

	// ~ Sprite render test suite paths Begin
	std::string const SuitePath_MinimalSmoke { "SuiteMinimalSmoke" };
	std::string const SuiteName_Render { "SuiteRender" };
	std::string const SuitePath_MinimalSmoke_Render = SuitePath_MinimalSmoke + std::string("/") + SuiteName_Render;
	std::string const SuiteName_Updater { "SuiteUpdater" };
	std::string const SuitePath_MinimalSmoke_Updater = SuitePath_MinimalSmoke + std::string("/") + SuiteName_Updater;
	// ~ Sprite render test suite paths End
} // Test::ISpr