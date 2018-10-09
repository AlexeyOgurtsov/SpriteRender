#include "Canvas.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{

namespace IMPL
{

Canvas::Canvas(const SCanvasInitializer& InInitializer) :
	_id(InInitializer.CreateArgs.GetTargetCanvasId())
,	_bVisible(InInitializer.CreateArgs.ShouldShow())
,	_props(InInitializer.CreateArgs.GetProps())
,	_sprites(SSpriteManagerInitializer{InInitializer.pSpriteBuffer})
,	_render(SSpriteSetRenderInitializer{&_sprites, InInitializer.pRenResources})
{
	
}

void Canvas::UpdateRect(const SRenderLayerCanvasRect& InNewRect)
{
	_props.RTRect = InNewRect;
}

void Canvas::Show()
{
	_bVisible = true;
}

void Canvas::Hide()
{
	_bVisible = false;
}

void Canvas::RenderAndCache()
{
	_render.RenderAndCache();
}

void Canvas::Render(ID3D11DeviceContext* pInDevCon)
{
	_render.Render(pInDevCon);
}

void Canvas::MarkVertexBufferResetted(ID3D11Buffer* pInOldBuffer, ID3D11Buffer* pInNewBuffer)
{
	_render.MarkVertexBufferResetted(pInOldBuffer, pInNewBuffer);
}

void Canvas::CreateSprite(const SCreateSpriteArgs& InArgs)
{
	Sprite* pSprite = _sprites.CreateSprite(InArgs);
	_render.MarkSpriteCreated(pSprite);
}

void Canvas::DeleteSprite(SpriteId InId)
{
	Sprite* pSprite = _sprites.FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "Canvas::DeleteSprite: sprite with the given ID NOT FOUND");
	_render.MarkSpritePendingDelete(pSprite);
	_sprites.DeleteSprite(InId);
}

void Canvas::ShowSprite(SpriteId InId)
{
	Sprite* pSprite = _sprites.FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "Canvas::ShowSprite: sprite with the given ID NOT FOUND");
	pSprite->Show();
	_render.MarkSpriteShown(pSprite);
}

void Canvas::HideSprite(SpriteId InId)
{
	Sprite* pSprite = _sprites.FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "Canvas::HideSprite: sprite with the given ID NOT FOUND");
	pSprite->Hide();
	_render.MarkSpriteHidden(pSprite);
}

void Canvas::MoveSpriteZOrderAfter(SpriteId InId, SpriteId InZBeforeSpriteId)
{
	Sprite* pSprite = _sprites.FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "Canvas::MoveSpriteZOrderAfter: sprite with the given ID NOT FOUND");
	_sprites.MoveSpriteZOrderAfter(pSprite, InZBeforeSpriteId);
	_render.MarkSpriteZOrderChanged(pSprite);
}

void Canvas::SetSpriteTransparency(SpriteId InId, ESpriteTransparency InTransparency)
{
	Sprite* pSprite = _sprites.FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "Canvas::SetSpriteTransparency: sprite with the given ID NOT FOUND");
	pSprite->UpdateTransparency(InTransparency);
	_render.MarkSpriteRenderStateChanged(pSprite);
}

void Canvas::SetMaterialInstance(SpriteId InId, MaterialInstanceRenderStateHandle pInRenderState)
{
	Sprite* pSprite = _sprites.FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "Canvas::SetMaterialInstance: sprite with the given ID NOT FOUND");
	pSprite->UpdateMaterialInstance(pInRenderState);
	_render.MarkSpriteRenderStateChanged(pSprite);
}

void Canvas::SetSpriteGeometry(SpriteId InId, const SSpriteGeometryData& InGeometry)
{
	Sprite* pSprite = _sprites.FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "Canvas::SetSpriteGeometry: sprite with the given ID NOT FOUND");
	pSprite->UpdateGeometry(InGeometry);
	// we should NOT notify render that the geometry is changed.
}

} // Dv::Spr::Ren::IMPL

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv