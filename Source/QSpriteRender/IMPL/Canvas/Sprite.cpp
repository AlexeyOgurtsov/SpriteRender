#include "Sprite.h"
#include "../Utils/CommonSystem.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{
	Sprite::Sprite(const SSpriteInitializer& InInitializer) :
		Id{InInitializer.Id}
	,	Geometry{InInitializer.Geometry}
	,	RenderMode{InInitializer.RenderMode}
	,	pMaterialRenderState{InInitializer.pMaterialRenderState}
	,	VBAlloc{InInitializer.VBAlloc}
	,	bVisible{InInitializer.bVisible}
	{
		BOOST_ASSERT(IsValidSpriteId(Id));
		BOOST_ASSERT(pMaterialRenderState);
		BOOST_ASSERT(InInitializer.VBAlloc.IsValid());
	}

	void Sprite::SetGeometry(const SSpriteGeometryData& InGeometry)
	{
		Geometry = InGeometry;
	}

	void Sprite::SetRenderMode(const SSpriteRenderMode& InRenderMode)
	{
		RenderMode = InRenderMode;
	}

	void Sprite::SetMaterialRenderState(const MaterialInstanceRenderStateHandle& InHandle)
	{
		pMaterialRenderState = InHandle;
	}

	void Sprite::SetVisibility(bool bInVisible)
	{
		bVisible = bInVisible;
	}

	void Sprite::SetVBAlloc(const D3D::BufferAlloc& InAlloc)
	{
		VBAlloc = InAlloc;
	}

	void Sprite::SetTransparency(ESpriteTransparency InTransparency)
	{
		RenderMode.Transparency = InTransparency;
	}

	bool Sprite::IsTransparent() const
	{
		return RenderMode.Transparency != ESpriteTransparency::BlendAdditive;
	}

	bool Sprite::IsHidden() const
	{
		return ! IsVisible();
	}

	const SpriteIterator& Sprite::GetIterator() const
	{
		BOOST_ASSERT_MSG(Iterator.has_value(), "Sprite::GetIterator: iterator is not binded");
		return Iterator.value();
	}

	void Sprite::BindIterator(const SpriteIterator& InIterator)
	{
		Iterator = InIterator;
	}
} // Dv::Spr::QRen::IMPL

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv
