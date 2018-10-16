#include "SpriteRenderSubsystemCreate.h"
#include "QSpriteRender/IMPL/Subsystem/SpriteRenderSubsystem.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{

SpriteRenderSubsystemHandle CreateSpriteRenderSubsystem(const SSpriteRenderSubsystemInitializer& InInitializer)
{
	AssertInitializerValid(InInitializer);
	return std::make_unique<IMPL::SpriteRenderSubsystem>(InInitializer);
}

} // Dv::Spr::QRen
} // Dv::Spr
} // DV