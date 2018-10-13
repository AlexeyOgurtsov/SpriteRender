#pragma once

/**
* Sprite render namespaces and minimal typedefs.
*
* To be included in almost any sprite render helper file.
*/

#include "ISprite/SpriteTypedefs.h"
#include "ISprite/SpriteCanvasTypedefs.h"

/**
* Namespace declarations (to make it possible to use namespace aliases)
*/
namespace Dv::Spr
{
	namespace Ren {} // Ren
	namespace Math {} // Math
} // Dv::Spr

namespace Test
{
	// ~ Sprite render namespaces Begin
	namespace MySpr = ::Dv::Spr;
	namespace MySprRen = ::Dv::Spr::Ren;
	namespace MySprMath = ::Dv::Spr::Math;
	// ~ Sprite render namespaces End

	// ~ Sprite render typedefs Begin
	using CanvasId = MySpr::SpriteCanvasId;
	using SprId = MySpr::SpriteId;
} // Test