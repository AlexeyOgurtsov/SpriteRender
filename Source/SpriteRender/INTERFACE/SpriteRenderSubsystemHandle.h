#pragma once

/**
* Contains a handle for storing the ISpriteRenderinterface.
*/

#include "ISpriteRenderSubsystem.h"
#include <memory>

namespace Dv
{
namespace Spr
{
namespace Ren
{
	using SpriteRenderSubsystemHandle = std::unique_ptr<ISpriteRenderSubsystem>;
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv