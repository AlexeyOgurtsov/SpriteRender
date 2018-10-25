#pragma once

#include "SpriteGeometryData.h"
#include "ISprite/SpriteTypedefs.h"
#include "ISprite/SpriteRenderMode.h"
#include "QSpriteRender/Material/MaterialInstanceRenderState.h"
#include "../RenUtils/BufferAlloc.h"
#include "SpriteContainerTypes.h"
#include <optional>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

struct SSpriteInitializer
{
	SpriteId Id = NULL_SPRITE_ID;
	SSpriteGeometryData Geometry;
	SSpriteRenderMode RenderMode;
	MaterialInstanceRenderStateHandle pMaterialRenderState;
	D3D::BufferAlloc VBAlloc;
	bool bVisible = false;

	SSpriteInitializer() = default;
	SSpriteInitializer(SpriteId InId, const SSpriteGeometryData& InGeometryData, const SSpriteRenderMode& InRenderMode, const MaterialInstanceRenderStateHandle& pInMaterialRenderState, const D3D::BufferAlloc& InAlloc, bool bInVisible) :
		Id{InId}
	,	Geometry{ InGeometryData }
	,	RenderMode{ InRenderMode }
	,	pMaterialRenderState{pInMaterialRenderState}
	,	bVisible{bInVisible} {}
};

struct SpriteIterator
{
	SpriteStorageList::iterator It_Storage;
	SpriteList::iterator It_ByZOrder;

	/**
	* Default ctor.
	* WARNING!!! It's much more readable if we initialize the iterator with direct assignments than with ctor
	* (as iterators has the same type).
	*/
	SpriteIterator() = default;
};

/**
* Sprite
*/
class Sprite
{
public:
	Sprite(const SSpriteInitializer& InInitializer);

	SpriteId GetId() const { return Id; }
	const SSpriteGeometryData& GetGeometry() const { return Geometry; }
	const SSpriteRenderMode& GetRenderMode() const { return RenderMode; }
	const MaterialInstanceRenderStateHandle& GetMaterialRenderState() const { return pMaterialRenderState; }
	const D3D::BufferAlloc& GetVBAlloc() const { return VBAlloc; }
	bool IsVisible() const { return bVisible; }
	bool IsHidden() const;

	void SetVBAlloc(const D3D::BufferAlloc& InAlloc);
	void SetGeometry(const SSpriteGeometryData& InGeometry);
	void SetRenderMode(const SSpriteRenderMode& InRenderMode);
	void SetMaterialRenderState(const MaterialInstanceRenderStateHandle& InHandle);
	void SetVisibility(bool bInVisible);
	void SetTransparency(ESpriteTransparency InTransparency);

	bool IsTransparent() const;

	const SpriteIterator& GetIterator() const;
	void BindIterator(const SpriteIterator& InIterator);

private:
	SpriteId Id;
	SSpriteGeometryData Geometry;
	SSpriteRenderMode RenderMode;
	MaterialInstanceRenderStateHandle pMaterialRenderState;
	D3D::BufferAlloc VBAlloc;
	bool bVisible;
	std::optional<SpriteIterator> Iterator;
};

} // Dv::Spr::QRen::IMPL

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv