#pragma once

#include "QSpriteRender/INTERFACE/ISpriteRenderSubsystem.h" 
#include "SpriteRenderSubsystemEnvironment.h"
#include "QSpriteRender/IMPL/Render/SpriteRender.h"
#include "QSpriteRender/Material/MaterialManager.h"

namespace Dv
{
namespace Spr
{
namespace QRen
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

	virtual Ren::ISpriteRender* GetRender() override { return &_render; }
	virtual ISpriteMaterialManager* GetMaterials() override;

private:
	std::ofstream& GetLog() const { return _environment.GetAmbientContext()->GetLog(); }

	// WARNING!!! The initialization order DOES matter:
	// Materials DO depend on the environment
	Environment _environment;
	MaterialManager _materials;
	SpriteRender _render;
};

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv