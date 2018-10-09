#pragma once

/**
* Provides interface for creating the sprite render.
*/
#include "SpriteRenderSubsystemHandle.h"
#include "SpriteRenderSubsystemInitializer.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{

/**
* The sprite render subsystem factory function.
*/
SpriteRenderSubsystemHandle CreateSpriteRenderSubsystem(const SSpriteRenderSubsystemInitializer& InInitializer);

} // Dv::Spr::Ren
} // Dv::Spr
} // DV