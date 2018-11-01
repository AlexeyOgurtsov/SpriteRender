#pragma once

namespace Dv
{
namespace Spr
{
namespace Ren
{
	class ISpriteRender;
} // Dv::Spr::Ren

namespace QRen
{

	class ISpriteMaterialManager;

/**
* Provides access to all of the components of the Q sprite render.
*/
class ISpriteRenderSubsystem
{
public:
	virtual ~ISpriteRenderSubsystem() = default;

	virtual Ren::ISpriteRender* GetRender() = 0;
	const Ren::ISpriteRender* GetRender() const;

	virtual ISpriteMaterialManager* GetMaterials() = 0;
	const ISpriteMaterialManager* GetMaterials() const;

	virtual void Tick(float InDeltaSeconds) = 0;
};

inline const Ren::ISpriteRender* ISpriteRenderSubsystem::GetRender() const
{
	return const_cast<ISpriteRenderSubsystem*>(this)->GetRender();
}

inline const ISpriteMaterialManager* ISpriteRenderSubsystem::GetMaterials() const
{
	return const_cast<ISpriteRenderSubsystem*>(this)->GetMaterials();
}

} // Dv::Spr::QRen
} // Dv::Spr
} // DV