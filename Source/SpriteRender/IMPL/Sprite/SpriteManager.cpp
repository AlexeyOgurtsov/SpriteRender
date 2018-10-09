#include "SpriteManager.h"
#include "SpriteRender/IMPL/Geometry/ISpriteGeometryBufferUpdateInterface.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

SpriteManager::SpriteManager(const SSpriteManagerInitializer& InInitializer) :
	_pSpriteBuffer(InInitializer.pSpriteBuffer)
{
	BOOST_ASSERT(_pSpriteBuffer);
}

Sprite* SpriteManager::FindById(SpriteId InId) const
{
	return _sprites.FindById(InId);
}

Sprite* SpriteManager::CreateSprite(const SCreateSpriteArgs& InArgs)
{
	ISpriteGeometry* pGeometry = _pSpriteBuffer->AddSprite(InArgs.GetRenderLayerGeometryData());
	Sprite* pSprite = _CreateSpriteObject(InArgs, pGeometry);
	_sprites.RegisterSprite(pSprite, InArgs.GetZBeforeSpriteId());
	return pSprite;
}

void SpriteManager::DeleteSprite(SpriteId InId)
{
	Sprite* pSprite = _sprites.FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "SpriteManager::DeleteSprite: sprite with the given id must be registered");
	pSprite->GetGeometry()->MarkSpriteAsDeletable();
	_sprites.DeleteSprite(pSprite);
}

void SpriteManager::MoveSpriteZOrderAfter(SpriteId InId, SpriteId ZBeforeSpriteId)
{
	Sprite* pSprite = _sprites.FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "SpriteManager::MoveSpriteZOrderAfter: sprite with the given id must be registered");
	MoveSpriteZOrderAfter(pSprite, ZBeforeSpriteId);
}

void SpriteManager::MoveSpriteZOrderAfter(Sprite* pInSprite, SpriteId ZBeforeSpriteId)
{
	_sprites.MoveSpriteZOrder(pInSprite, ZBeforeSpriteId);
}

void SpriteManager::UpdateSpriteGeometry(Sprite* pInSender, const SSpriteGeometryData& InGeometry)
{
	// Nothing is to do here yet
}

void SpriteManager::UpdateSpriteTransparency(Sprite* pInSender, ESpriteTransparency InTransparency)
{
	// Nothing is to do here yet
}

void SpriteManager::UpdateSpriteMaterial(Sprite* pInSender, MaterialInstanceRenderStateHandle pInRenderState)
{
	// Nothing is to do here yet
}

void SpriteManager::ShowSprite(Sprite* pInSender)
{
	_sprites.ShowSprite(pInSender);
}

void SpriteManager::HideSprite(Sprite* pInSender)
{
	_sprites.HideSprite(pInSender);
}

Sprite* SpriteManager::_CreateSpriteObject(const SCreateSpriteArgs& InArgs, ISpriteGeometry* pInGeometry)
{
	SSpriteInitializer initializer ( this, InArgs.GetTargetSpriteId(), InArgs.GetRenderLayerData(), pInGeometry );
	return new Sprite(initializer);
}

Sprite* SpriteManager::First() const
{
	return _sprites.First();
}


Sprite* SpriteManager::Last() const
{
	return _sprites.Last();
}

Sprite* SpriteManager::FirstVisible() const
{
	return _sprites.FirstVisible();
}

Sprite* SpriteManager::LastVisible() const
{
	return _sprites.LastVisible();
}


} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv