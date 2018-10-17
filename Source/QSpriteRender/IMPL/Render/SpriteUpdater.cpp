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
	BOOST_ASSERT_MSG(false, "SpriteUpdater::CreateSprite: NOT yet impl");
	/*
	SSpriteGeometryData const geometryData = FillGeometryDataStruct(InCommand.Props.Geometry);
	SSpriteData const renderLayerSpriteData = FillSpriteDataStruct(InCommand);
	SCreateSpriteArgs const createSpriteArgs(InCommand, renderLayerSpriteData, geometryData);
	_pCanvas->CreateSprite(createSpriteArgs);
	*/
}

void SpriteUpdater::DeleteSprite(const Ren::SSpriteDeleteCommandInitializer& InCommand)
{
	BOOST_ASSERT_MSG(false, "SpriteUpdater::DeleteSprite");
	//_pCanvas->DeleteSprite(InCommand.TargetSpriteId);
}

void SpriteUpdater::HideSprite(const Ren::SSpriteHideCommandInitializer& InCommand)
{
	BOOST_ASSERT_MSG(false, "SpriteUpdater::HideSprite");
	//_pCanvas->HideSprite(InCommand.TargetSpriteId);
}

void SpriteUpdater::ShowSprite(const Ren::SSpriteShowCommandInitializer& InCommand)
{
	BOOST_ASSERT_MSG(false, "SpriteUpdater::ShowSprite");
	//_pCanvas->ShowSprite(InCommand.TargetSpriteId);
}

void SpriteUpdater::SetSpriteTransparency(const Ren::SSpriteSetTransparencyCommandInitializer& InCommand)
{
	BOOST_ASSERT_MSG(false, "SpriteUpdater::SetSpriteTransparency()");
	//_pCanvas->SetSpriteTransparency(InCommand.TargetSpriteId, InCommand.Transparency);
}

void SpriteUpdater::SetSpriteGeometry(const Ren::SSpriteSetGeometryCommandInitializer& InCommand)
{
	BOOST_ASSERT_MSG(false, "SpriteUpdater::SetSpriteGeometry: NOT yet impl");
	//SSpriteGeometryData const geometryData = FillGeometryDataStruct(InCommand.Geometry);
	//_pCanvas->SetSpriteGeometry(InCommand.TargetSpriteId, geometryData);
}


void SpriteUpdater::SetSpriteMaterial(const Ren::SSpriteSetMaterialCommandInitializer& InCommand)
{
	BOOST_ASSERT_MSG(false, "SpriteUpdater::SetSpriteMaterial: NOT yet impl");
	//_pCanvas->SetMaterialInstance(InCommand.TargetSpriteId, DowncastMaterialInstanceRSHandle(InCommand.pRenderState));
}

void SpriteUpdater::SetSpriteZOrder(const Ren::SSpriteSetZOrderCommandInitializer& InCommand)
{
	BOOST_ASSERT_MSG(false, "SpriteUpdater::SetSpriteZOrder: NOT yet impl");
	//_pCanvas->MoveSpriteZOrderAfter(InCommand.TargetSpriteId, InCommand.ZBeforeSpriteId);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv