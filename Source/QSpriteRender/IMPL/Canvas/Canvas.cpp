#include "Canvas.h"
#include "../RenUtils/RenResources.h"
#include "../Utils/CommonSystem.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{

namespace IMPL
{

Canvas::Canvas(const SCanvasInitializer& InInitializer) :
	_id(InInitializer.CreateArgs.GetTargetCanvasId())
,	_bVisible(InInitializer.CreateArgs.ShouldShow())
,	_props(InInitializer.CreateArgs.GetProps())
{
	// SpriteManager
	SSpriteManagerInitializer SpriteManagerInitializer 
	{
		InInitializer.CreateArgs.GetName(),
		InInitializer.pAmbientContext, 
		InInitializer.pRenResources->GetDev(),
		InInitializer.pRenResources->GetDevCon(),
		InInitializer.bDebug
	};
	SpriteManagerInitializer.bAutoResizeable = InInitializer.bAutoResizeable;
	SpriteManagerInitializer.MaxSprites = InInitializer.MaxSprites;
	pSprites.reset(new SpriteManager{ SpriteManagerInitializer });

	// Render
	SpriteSetRenderInitializer RenderInitializer { InInitializer.pAmbientContext, pSprites.get(), InInitializer.pRenResources };
	pRender.reset(new SpriteSetRender(RenderInitializer));
}

int Canvas::GetCapacityInSprites() const
{
	return pSprites->GetCapacityInSprites();
}

void Canvas::BindStorageIterator(CanvasStorage::iterator InIt)
{
	Iterator.It_CanvasStorage = InIt;
}

void Canvas::BindZOrderIterator(CanvasList::iterator InIt)
{
	Iterator.It_CanvasList = InIt;
}

void Canvas::UpdateRect(const SRenderLayerCanvasRect& InNewRect)
{
	_props.RTRect = InNewRect;
}

bool Canvas::IsHidden() const
{
	return false == IsVisible();
}

void Canvas::SetVisibility(bool bInVisible)
{
	_bVisible = bInVisible;
}

void Canvas::Show()
{
	_bVisible = true;
}

void Canvas::Hide()
{
	_bVisible = false;
}

void Canvas::FlushD3D()
{
	pSprites->FlushD3D();
}

void Canvas::Render(ID3D11DeviceContext* pInDevCon, UINT InVBSlot)
{
	pRender->Render(pInDevCon, InVBSlot);
}

void Canvas::CreateSprite(const SCreateSpriteArgs& InArgs)
{
	pSprites->CreateSprite(InArgs);
}

void Canvas::DeleteSprite(SpriteId InId)
{
	pSprites->DeleteSprite(InId);
}

void Canvas::ShowSprite(SpriteId InId)
{
	pSprites->ShowSprite(InId);
}

void Canvas::HideSprite(SpriteId InId)
{
	pSprites->HideSprite(InId);
}

void Canvas::MoveSpriteZOrderAfter(SpriteId InId, SpriteId InZBeforeSpriteId)
{
	pSprites->MoveSpriteZOrderAfter(InId, InZBeforeSpriteId);
}

void Canvas::SetSpriteTransparency(SpriteId InId, ESpriteTransparency InTransparency)
{
	pSprites->SetSpriteTransparency(InId, InTransparency);
}

void Canvas::SetMaterialInstance(SpriteId InId, MaterialInstanceRenderStateHandle pInRenderState)
{
	pSprites->SetMaterialInstance(InId, pInRenderState);
}

void Canvas::SetSpriteGeometry(SpriteId InId, const SSpriteGeometryData& InGeometry)
{
	pSprites->SetSpriteGeometry(InId, InGeometry);
}

} // Dv::Spr::QRen::IMPL

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv