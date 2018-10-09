#pragma once

#include "SpriteData.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

struct SSpriteGeometryData;
class ISpriteGeometry;
class SpriteManager;

struct SSpriteInitializer
{
	bool bVisible;
	SpriteManager* pSpriteManager;
	SpriteId Id;
	SSpriteData Data;
	ISpriteGeometry* pGeometry;

	SSpriteInitializer(SpriteManager* pInSpriteManager, SpriteId InId, const SSpriteData& InData, ISpriteGeometry* pInSpriteGeometry) :
		bVisible(false)
	,	pSpriteManager(pInSpriteManager)
	,	Id(InId)
	,	Data(InData)
	,	pGeometry(pInSpriteGeometry) {}
};

/**
* Handle of the sprite, managed by the sprite manager.
*/
class Sprite
{
public:
	Sprite(const SSpriteInitializer& InInitializer);

	SpriteManager* GetManager() const { return _pSpriteManager; }

	SpriteId GetId() const { return _id; }

	bool IsVisible() const { return _bVisible; }
	const SSpriteData& GetData() const { return _data; }
	ISpriteGeometry* GetGeometry() const { return _pGeometry; }
	ESpriteTransparency GetTransparency() const { return _data.RenderMode.Transparency; }

	/**
	* Render state helper getter.
	* However: NEVER use it create a shared-ptr from it!!!
	*/
	MaterialInstanceRenderState* GetRenderState() const { return _data.pRenderState.get(); }

	void Show();
	void Hide();
	void UpdateMaterialInstance(MaterialInstanceRenderStateHandle pInRenderState);
	void UpdateGeometry(const SSpriteGeometryData& InGeometry);
	void UpdateTransparency(ESpriteTransparency& InTransparency);

private:
	SpriteManager* _pSpriteManager = nullptr;
	SpriteId _id;
	SSpriteData _data;
	bool _bVisible;
	ISpriteGeometry* _pGeometry = nullptr;
};

/**
* Returns true if the given sprite is on the top of all visible sprites by the Z order.
*
* The sprite must be registered as a visible sprite in the sprite manager!
*/
bool IsOnTopOfVisible(const Sprite* pInSprite);

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv