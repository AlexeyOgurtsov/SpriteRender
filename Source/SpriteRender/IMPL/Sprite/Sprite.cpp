#include "Sprite.h"
#include "SpriteRender/IMPL/Geometry/ISpriteGeometry.h"
#include "SpriteRender/IMPL/Geometry/SpriteGeometryData.h"
#include "SpriteManager.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

bool IsOnTopOfVisible(const Sprite* pInSprite)
{
	BOOST_ASSERT(pInSprite);
	BOOST_ASSERT_MSG(pInSprite->IsVisible(), "IsOnTopOfVisible: the sprite must be a visible sprite already");
	const SpriteManager* const pManager = pInSprite->GetManager();
	return (pInSprite == pManager->LastVisible());
}

Sprite::Sprite(const SSpriteInitializer& InInitializer) :
	_bVisible(InInitializer.bVisible)
,	_pSpriteManager(InInitializer.pSpriteManager)
,	_id(InInitializer.Id)
,	_data(InInitializer.Data)
,	_pGeometry(InInitializer.pGeometry)
{
	BOOST_ASSERT(_pSpriteManager);
}

void Sprite::Show()
{
	_bVisible = true;
	_pSpriteManager->ShowSprite(this);
}

void Sprite::Hide()
{
	_bVisible = false;
	_pSpriteManager->HideSprite(this);
}

void Sprite::UpdateGeometry(const SSpriteGeometryData& InGeometry)
{
	_pGeometry->UpdateSpriteGeometry(InGeometry);
	_pSpriteManager->UpdateSpriteGeometry(this, InGeometry);
}

void Sprite::UpdateTransparency(ESpriteTransparency& InTransparency)
{
	_data.RenderMode.Transparency = InTransparency;
	_pSpriteManager->UpdateSpriteTransparency(this, InTransparency);
}

void Sprite::UpdateMaterialInstance(MaterialInstanceRenderStateHandle pInRenderState)
{
	_data.pRenderState = std::move(pInRenderState);
	_pSpriteManager->UpdateSpriteMaterial(this, pInRenderState);
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv