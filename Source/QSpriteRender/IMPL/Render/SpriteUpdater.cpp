#include "SpriteUpdater.h"
#include "QSpriteRender/IMPL/Canvas/Canvas.h"
#include "QSpriteRender/IMPL/Glue/Glue_InterfaceToImpl.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
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

void SpriteUpdater::CreateSprite(const Ren::SSpriteCreateCommandInitializer& InCommand)
{
	SSpriteGeometryData const geometryData = FillGeometryDataStruct(InCommand.Props.Geometry);
	SCreateSpriteArgs const createSpriteArgs(InCommand, geometryData);
	_pCanvas->CreateSprite(createSpriteArgs);	
}

void SpriteUpdater::DeleteSprite(const Ren::SSpriteDeleteCommandInitializer& InCommand)
{
	_pCanvas->DeleteSprite(InCommand.TargetSpriteId);
}

void SpriteUpdater::HideSprite(const Ren::SSpriteHideCommandInitializer& InCommand)
{
	_pCanvas->HideSprite(InCommand.TargetSpriteId);
}

void SpriteUpdater::ShowSprite(const Ren::SSpriteShowCommandInitializer& InCommand)
{
	_pCanvas->ShowSprite(InCommand.TargetSpriteId);
}

void SpriteUpdater::SetSpriteTransparency(const Ren::SSpriteSetTransparencyCommandInitializer& InCommand)
{
	_pCanvas->SetSpriteTransparency(InCommand.TargetSpriteId, InCommand.Transparency);
}

void SpriteUpdater::SetSpriteGeometry(const Ren::SSpriteSetGeometryCommandInitializer& InCommand)
{
	SSpriteGeometryData const geometryData = FillGeometryDataStruct(InCommand.Geometry);
	_pCanvas->SetSpriteGeometry(InCommand.TargetSpriteId, geometryData);
}


void SpriteUpdater::SetSpriteMaterial(const Ren::SSpriteSetMaterialCommandInitializer& InCommand)
{
	_pCanvas->SetMaterialInstance(InCommand.TargetSpriteId, DowncastMaterialInstanceRSHandle(InCommand.pRenderState));
}

void SpriteUpdater::SetSpriteZOrder(const Ren::SSpriteSetZOrderCommandInitializer& InCommand)
{
	_pCanvas->MoveSpriteZOrderAfter(InCommand.TargetSpriteId, InCommand.ZBeforeSpriteId);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv