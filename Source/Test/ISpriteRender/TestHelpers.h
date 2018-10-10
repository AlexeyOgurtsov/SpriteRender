#pragma once

/**
* To be included in any header that contains unit tests.
*/
#include "ISpriteRenderTestUtils/ISpriteRenderFwd.h"
#include "ISpriteRender/ISpriteRender.h"
#include <boost/test/included/unit_test.hpp>
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
	// ~ Sprite render test suite paths End
} // Test::ISpr