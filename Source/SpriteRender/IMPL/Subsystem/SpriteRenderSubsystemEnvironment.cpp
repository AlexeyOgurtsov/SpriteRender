#include "SpriteRenderSubsystemEnvironment.h"
#include "SpriteRender/INTERFACE/SpriteRenderSubsystemInitializer.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

Environment::Environment(const SSpriteRenderSubsystemInitializer& InInitializer) :
	_ambientContext{ SAmbientContextInitializer{ InInitializer.Render.Log.pLog } }
,	_spriteGeometryBuffer{ SSpriteGeometryBufferInitializer{ InInitializer.Render.Device.pDev, InInitializer.Render.InitialSpriteBufferCapacity} }
,	_renResources{ D3D::SRenResourcesInitializer{ &_ambientContext, InInitializer.Render.Device.pDev, InInitializer.Render.Device.pImmDevCon, InInitializer.Render } }
{
	// TODO: Perform initialization here
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv