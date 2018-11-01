#include "Canvas.h"
#include "../RenUtils/RenResources.h"
#include "../Utils/CommonSystem.h"
#include "../Environment/AmbientContext.h"

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
,	_coordSystem(InInitializer.CreateArgs.GetCoordSystem(), _props.RTRect.GetAspectWidthOverHeight())
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
	SpriteSetRenderInitializer RenderInitializer
	{
		InInitializer.CreateArgs.GetTargetCanvasId(),
		InInitializer.bDebug, 
		InInitializer.CreateArgs.GetName(), 
		InInitializer.pAmbientContext, pSprites.get(), InInitializer.pRenResources 
	};
	pRender.reset(new SpriteSetRender(RenderInitializer));
	UpdateRenderCoordSystem();
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
	_coordSystem.UpdateAspectWidthOverHeight(InNewRect.GetAspectWidthOverHeight());
	UpdateRenderCoordSystem();
}

void Canvas::UpdateCoordSystem(const SCanvasCoordSystem& InCoordSystem)
{
	// It seams linke an excessive, but we update both props and coord system manager here
	// (maybe we pass props somewhere as a struct).
	_props.CoordSystem = InCoordSystem;
	_coordSystem.UpdateCoordSystem(InCoordSystem);
	UpdateRenderCoordSystem();
}

void Canvas::UpdateRenderCoordSystem()
{
	pRender->UpdateCoordSystem(_coordSystem.GetMatrix(), _coordSystem.GetCoordSystem(), _props.RTRect);
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
	pRender->FlushD3D();
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

CanvasCoordSystemManager::CanvasCoordSystemManager(const SCanvasCoordSystem& InCoordSystem, float InAspectWidthOverHeight) :
	AspectWidthOverHeight{InAspectWidthOverHeight}
,	CoordSystem{ InCoordSystem }
{
	RecalculateMatrix();
}

void CanvasCoordSystemManager::UpdateCoordSystem(const SCanvasCoordSystem& InCoordSystem)
{
	CoordSystem = InCoordSystem;
	RecalculateMatrix();
}

void CanvasCoordSystemManager::UpdateAspectWidthOverHeight(float InAspectWidthOverHeight)
{
	AspectWidthOverHeight = InAspectWidthOverHeight;
	RecalculateMatrix();
}

void CanvasCoordSystemManager::RecalculateMatrix()
{
	// WARNING!!! known that matrix is in the column-first storage format:
	Matrix[0][0] = 1.0F;	Matrix[1][0] = 0.0F;	Matrix[2][0] = 0.0F; 	Matrix[3][0] = 0.0F;
	Matrix[0][1] = 0.0F;	Matrix[1][1] = 1.0F;	Matrix[2][1] = 0.0F; 	Matrix[3][1] = 0.0F;
	Matrix[0][2] = 0.0F;	Matrix[1][2] = 0.0F;	Matrix[2][2] = 1.0F; 	Matrix[3][2] = 0.0F;
	Matrix[0][3] = 0.0F;	Matrix[1][3] = 0.0F;	Matrix[2][3] = 0.0F; 	Matrix[3][3] = 1.0F;

	float const LeftX = GetLeftX();
	float const RightX = GetRightX();
	float const BottomY = GetBottomY();
	float const TopY = GetTopY();
	float const Width = GetWidth();
	float const Height = GetHeight();
	float const ZNear = GetZNear();
	float const Diff_ZFar_ZNear = GetZFar()-GetZNear();

	Matrix[0][0] = 2.0F / Width;
	Matrix[1][1] = 2.0F / Height;
	Matrix[2][2] = 1.0F / (Diff_ZFar_ZNear);

	// Last line
	Matrix[0][3] = -(LeftX+RightX)/Width;
	Matrix[1][3] = -(TopY+BottomY)/Height;
	Matrix[2][3] = -ZNear/(Diff_ZFar_ZNear);
	Matrix[3][3] = 1.0F;
}


float CanvasCoordSystemManager::GetWidth() const
{
	return 2.0F * GetCoordSystem().GetHalfWidth(GetAspectWidthOverHeight());
}

float CanvasCoordSystemManager::GetHeight() const
{
	return 2.0F * GetCoordSystem().HalfHeight;
}

float CanvasCoordSystemManager::GetLeftX() const
{
	return GetCoordSystem().GetLeftX(GetAspectWidthOverHeight());
}

float CanvasCoordSystemManager::GetRightX() const
{
	return GetCoordSystem().GetRightX(GetAspectWidthOverHeight());
}

float CanvasCoordSystemManager::GetBottomY() const
{
	return GetCoordSystem().GetBottomY();
}

float CanvasCoordSystemManager::GetTopY() const
{
	return GetCoordSystem().GetTopY();
}

float CanvasCoordSystemManager::GetZNear() const
{
	return 0.01F; // @TODO: Provide correct ZNear
}

float CanvasCoordSystemManager::GetZFar() const
{
	return 1.0F; // @TODO: Provide correct ZFar
}

} // Dv::Spr::QRen::IMPL

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv