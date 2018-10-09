#pragma once

#include "SpriteContainer.h"
#include "Sprite.h"
#include "SpriteCommands.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class ISpriteGeometryBufferUpdateInterface;
class ISpriteGeometry;

struct SSpriteManagerInitializer
{
	ISpriteGeometryBufferUpdateInterface* pSpriteBuffer;

	SSpriteManagerInitializer(ISpriteGeometryBufferUpdateInterface* pInSpriteBuffer) :
		pSpriteBuffer(pInSpriteBuffer) {}
};

/**
* Creates sprites, manages them, registers geometries in the sprite buffer. 
*/
class SpriteManager
{
public:
	using SpriteIterator = SpriteContainer::SpriteIterator;
	using VisibleSpriteIterator = SpriteContainer::VisibleSpriteIterator;

	SpriteManager(const SSpriteManagerInitializer& InInitializer);

	int GetNumVisibleSprites() const { return _sprites.GetNumVisibleSprites(); }
	int GetNumSprites() const { return _sprites.GetNumSprites(); }

	SpriteIterator GetSpriteIterator_Begin() const { return _sprites.begin(); }
	SpriteIterator GetSpriteIterator_End() const { return _sprites.end(); }

	VisibleSpriteIterator GetVisibleSpriteIterator_Begin() const { return _sprites.begin_visible(); }
	VisibleSpriteIterator GetVisibleSpriteIterator_End() const { return _sprites.end_visible(); }

	/**
	* Returns the first sprite (or returns nullptr if none available)
	*/
	Sprite* First() const;

	/**
	* Returns the last sprite (or returns nullptr if none available)
	*/
	Sprite* Last() const;

	/**
	* Returns the first visible sprite (or returns nullptr if none available)
	*/
	Sprite* FirstVisible() const;

	/**
	* Returns the last visible sprite (or returns nullptr if none available)
	*/
	Sprite* LastVisible() const;

	/**
	* Tries to find sprite by id.
	*
	* @returns: nullptr if NOT found.
	*/
	Sprite* FindById(SpriteId InId) const;

	Sprite* CreateSprite(const SCreateSpriteArgs& InArgs);
	void DeleteSprite(SpriteId InId);

	void MoveSpriteZOrderAfter(SpriteId InId, SpriteId ZBeforeSpriteId);

	void MoveSpriteZOrderAfter(Sprite* pInSprite, SpriteId ZBeforeSpriteId);

	/**
	* To be called from the Sprite only.
	*/
	void UpdateSpriteGeometry(Sprite* pInSender, const SSpriteGeometryData& InGeometry);

	/**
	* To be called from the Sprite only.
	*/
	void UpdateSpriteTransparency(Sprite* pInSender, ESpriteTransparency InTransparency);

	/**
	* To be called from the Sprite only.
	*/
	void UpdateSpriteMaterial(Sprite* pInSender, MaterialInstanceRenderStateHandle pInRenderState);

	/**
	* To be called from the Sprite only.
	*/
	void ShowSprite(Sprite* pInSender);

	/**
	* To be called from the Sprite only.
	*/
	void HideSprite(Sprite* pInSender);

private:
	Sprite* _CreateSpriteObject(const SCreateSpriteArgs& InArgs, ISpriteGeometry* pInGeometry);

	SpriteContainer _sprites;
	ISpriteGeometryBufferUpdateInterface* _pSpriteBuffer;
}; 

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv