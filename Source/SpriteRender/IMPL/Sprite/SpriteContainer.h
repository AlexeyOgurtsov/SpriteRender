#pragma once

#include "ISprite/SpriteTypedefs.h"
#include <memory>
#include <list>
#include <unordered_map>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class Sprite;

using SpriteContainerByZOrder = std::list<Sprite*>;
using VisibleSpriteContainerByZOrder = std::list<Sprite*>;

/**
* Accounts sprites.
* Knows Z Order of the sprites.
*/

class SpriteContainer
{
public:
	using SpriteIterator = SpriteContainerByZOrder::const_iterator;
	using VisibleSpriteIterator = VisibleSpriteContainerByZOrder::const_iterator;

	~SpriteContainer();

	int GetNumSprites() const { return _spritesInZOrder.size(); }
	int GetNumVisibleSprites() const { return _visibleSpritesInZOrder.size(); }

	SpriteIterator begin() const { return _spritesInZOrder.begin(); }
	SpriteIterator end() const { return _spritesInZOrder.end(); }
	SpriteIterator cbegin() const { return _spritesInZOrder.cbegin(); }
	SpriteIterator cend() const { return _spritesInZOrder.cend(); }

	VisibleSpriteIterator begin_visible() const { return _visibleSpritesInZOrder.begin(); }
	VisibleSpriteIterator end_visible() const { return _visibleSpritesInZOrder.end(); }
	VisibleSpriteIterator cbegin_visible() const { return _visibleSpritesInZOrder.cbegin(); }
	VisibleSpriteIterator cend_visible() const { return _visibleSpritesInZOrder.cend(); }

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
	* Finds the sprite by the id.
	* @returns: nullptr if the sprite with the given id does NOT exist. 
	*/
	Sprite* FindById(SpriteId InId) const;

	/**
	* Registers sprite and owns it.
	* The sprite must be created in C++ dynamic memory.
	* 
	* @param ZBeforeSpriteId: id of the sprite that is to be located right before the given 
	* in the Z Order (if NULL sprite id is used, the sprite is to be added at the end of the ZOrder).
	*/
	void RegisterSprite(Sprite* pInSprite, SpriteId ZBeforeSpriteId);

	/**
	* Immediately deletes the sprite.
	*/
	void DeleteSprite(Sprite* pInSprite);

	/**
	* Updates Z Order of the given sprite by moving it right after the given sprite.
	*/
	void MoveSpriteZOrder(Sprite* pInSprite, SpriteId ZBeforeSpriteId);

	/**
	* Shows the given sprite.
	*/
	void ShowSprite(Sprite* pInSprite);

	/**
	* Hides the given sprite.
	*/
	void HideSprite(Sprite* pInSprite);

private:
	using SpriteHolder = std::unique_ptr<Sprite>;

	/**
	* See comments in the cpp file.
	*/
	SpriteContainerByZOrder::iterator _InsertSpriteAfter(Sprite* pInSprite, SpriteId ZBeforeSprite);

	/**
	* See comments in the cpp file.
	*/
	VisibleSpriteContainerByZOrder::iterator _EnlistSpriteAsVisible(Sprite* pInSprite, SpriteContainerByZOrder::iterator InItZAfterSprite);

	/**
	* Find iterator in the visible sprite container by the sprite pointer.
	*/
	VisibleSpriteContainerByZOrder::iterator _FindIterator_InZOrderVisible(Sprite* pInSprite);

	/**
	* Find iterator in the sprite container by the sprite id.
	*/
	SpriteContainerByZOrder::iterator _FindIterator_InZOrder(SpriteId InId);

	std::unordered_map<SpriteId, SpriteHolder> _sprites;
	SpriteContainerByZOrder _spritesInZOrder;
	VisibleSpriteContainerByZOrder _visibleSpritesInZOrder;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv