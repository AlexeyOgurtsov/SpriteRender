#include "SpriteRenderSubsystem.h"
#include "QSpriteRender/INTERFACE/SpriteRenderSubsystemInitializer.h"
#include "QSpriteRender/Material/DefaultSpriteMaterial.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{


SpriteRenderSubsystem::SpriteRenderSubsystem(const SSpriteRenderSubsystemInitializer& InInitializer) :
	_environment{ InInitializer }
,	_render(&_environment)
,	_materials{ SMaterialManagerInitializer{ &_environment.GetConfig().Shaders, _environment.GetAmbientContext(), InInitializer.Render.Device.pDev } }
{
	// Initializer checking:
	// The initializer struct is already checked in the factory function,
	// should we should NOT check it again.

	//GetLog() << "SpriteRenderSubsystem: initializing with the following config:" << std::endl;
	LogSpriteRenderInitializer(GetLog(), InInitializer);

	// Initializing materials
	_materials.Register(new DefaultSpriteMaterial{ SDefaultMaterialInitializer{ DEFAULT_MATERIAL_ID, _materials.GetShaderCompileEnv() } });
}

ISpriteMaterialManager* SpriteRenderSubsystem::GetMaterials()
{
	return &_materials;
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv