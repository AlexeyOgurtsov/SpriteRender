#pragma once

#include "SpriteRender/INTERFACE/ISpriteRenderSubsystem.h" 
#include "SpriteRenderSubsystemEnvironment.h"
#include "SpriteRender/IMPL/Render/SpriteRender.h"
#include "SpriteRender/Material/MaterialManager.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{

struct SSpriteRenderSubsystemInitializer;

namespace IMPL
{

/**
* Implementation of the sprite render subsystem.
*/
class SpriteRenderSubsystem : public ISpriteRenderSubsystem
{
public:
	SpriteRenderSubsystem(const SSpriteRenderSubsystemInitializer& InInitializer);

	virtual ISpriteRender* GetRender() override { return &_render; }
	virtual ISpriteMaterialManager* GetMaterials() override;

private:
	std::ofstream& GetLog() const { return _environment.GetAmbientContext()->GetLog(); }

	// WARNING!!! The initialization order DOES matter:
	// Materials DO depend on the environment
	Environment _environment;
	MaterialManager _materials;
	SpriteRender _render;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv