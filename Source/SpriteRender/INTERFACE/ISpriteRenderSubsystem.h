#pragma once

namespace Dv
{
namespace Spr
{
namespace Ren
{

class ISpriteRender;
class ISpriteMaterialManager;

/**
* Provides access to all of the components of the sprite render.
*/
class ISpriteRenderSubsystem
{
public:
	virtual ~ISpriteRenderSubsystem() = default;

	virtual ISpriteRender* GetRender() = 0;
	const ISpriteRender* GetRender() const;

	virtual ISpriteMaterialManager* GetMaterials() = 0;
	const ISpriteMaterialManager* GetMaterials() const;
};

inline const ISpriteRender* ISpriteRenderSubsystem::GetRender() const
{
	return const_cast<ISpriteRenderSubsystem*>(this)->GetRender();
}

inline const ISpriteMaterialManager* ISpriteRenderSubsystem::GetMaterials() const
{
	return const_cast<ISpriteRenderSubsystem*>(this)->GetMaterials();
}

} // Dv::Spr::Ren
} // Dv::Spr
} // DV