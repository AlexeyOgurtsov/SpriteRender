#include "SpriteUpdater.h"
#include "SpriteRender/IMPL/Canvas/Canvas.h"
#include "SpriteRender/IMPL/Glue/Glue_InterfaceToImpl.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

SpriteUpdater::SpriteUpdater(const SSpriteUpdaterInitializer& InInitializer) :
	_pCanvas(InInitializer.pCanvas)
{
	BOOST_ASSERT(_pCanvas);
}

SpriteCanvasId SpriteUpdater::GetCanvasId() const
{
	return _pCanvas->GetId();
}

void SpriteUpdater::CreateSprite(const SSpriteCreateCommandInitializer& InCommand)
{
	SSpriteGeometryData const geometryData = FillGeometryDataStruct(InCommand.Props.Geometry);
	SSpriteData const renderLayerSpriteData = FillSpriteDataStruct(InCommand);
	SCreateSpriteArgs const createSpriteArgs(InCommand, renderLayerSpriteData, geometryData);
	_pCanvas->CreateSprite(createSpriteArgs);
}

void SpriteUpdater::DeleteSprite(const SSpriteDeleteCommandInitializer& InCommand)
{
	_pCanvas->DeleteSprite(InCommand.TargetSpriteId);
}

void SpriteUpdater::HideSprite(const SSpriteHideCommandInitializer& InCommand)
{
	_pCanvas->HideSprite(InCommand.TargetSpriteId);
}

void SpriteUpdater::ShowSprite(const SSpriteShowCommandInitializer& InCommand)
{
	_pCanvas->ShowSprite(InCommand.TargetSpriteId);
}

void SpriteUpdater::SetSpriteTransparency(const SSpriteSetTransparencyCommandInitializer& InCommand)
{
	_pCanvas->SetSpriteTransparency(InCommand.TargetSpriteId, InCommand.Transparency);
}

void SpriteUpdater::SetSpriteGeometry(const SSpriteSetGeometryCommandInitializer& InCommand)
{
	SSpriteGeometryData const geometryData = FillGeometryDataStruct(InCommand.Geometry);
	_pCanvas->SetSpriteGeometry(InCommand.TargetSpriteId, geometryData);
}


void SpriteUpdater::SetSpriteMaterial(const SSpriteSetMaterialCommandInitializer& InCommand)
{
	_pCanvas->SetMaterialInstance(InCommand.TargetSpriteId, DowncastMaterialInstanceRSHandle(InCommand.pRenderState));
}

void SpriteUpdater::SetSpriteZOrder(const SSpriteSetZOrderCommandInitializer& InCommand)
{
	_pCanvas->MoveSpriteZOrderAfter(InCommand.TargetSpriteId, InCommand.ZBeforeSpriteId);
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv