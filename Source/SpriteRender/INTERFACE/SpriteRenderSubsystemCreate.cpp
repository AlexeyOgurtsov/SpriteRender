#include "SpriteRenderSubsystemCreate.h"
#include "SpriteRender/IMPL/Subsystem/SpriteRenderSubsystem.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{

SpriteRenderSubsystemHandle CreateSpriteRenderSubsystem(const SSpriteRenderSubsystemInitializer& InInitializer)
{
	AssertInitializerValid(InInitializer);
	return std::make_unique<IMPL::SpriteRenderSubsystem>(InInitializer);
}

} // Dv::Spr::Ren
} // Dv::Spr
} // DV