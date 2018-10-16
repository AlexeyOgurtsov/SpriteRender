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
namespace QRen
{
	using SpriteRenderSubsystemHandle = std::unique_ptr<ISpriteRenderSubsystem>;
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv