#include "SpriteContainer.h"
#include "Sprite.h"
#include <iterator>
#include <algorithm>
#include <optional>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

SpriteContainer::~SpriteContainer()
{
	// we need default implementation of the destructor
}

Sprite* SpriteContainer::First() const
{
	if (_spritesInZOrder.empty()) return nullptr;
	return _spritesInZOrder.front();
}

Sprite* SpriteContainer::Last() const
{
	if (_spritesInZOrder.empty()) return nullptr;
	return _spritesInZOrder.back();
}

Sprite* SpriteContainer::FirstVisible() const
{
	if (_visibleSpritesInZOrder.empty()) return nullptr;
	return _visibleSpritesInZOrder.front();
}

Sprite* SpriteContainer::LastVisible() const
{
	if (_visibleSpritesInZOrder.empty()) return nullptr;
	return _visibleSpritesInZOrder.back();
}

Sprite* SpriteContainer::FindById(SpriteId InId) const
{
	auto it = _sprites.find(InId);
	if(it == _sprites.end()) { return nullptr; }
	return it->second.get();
}

void SpriteContainer::RegisterSprite(Sprite* pInSprite, SpriteId ZBeforeSpriteId)
{
	_sprites.emplace(pInSprite->GetId(), SpriteHolder{pInSprite});
	_InsertSpriteAfter(pInSprite, ZBeforeSpriteId);
}

void SpriteContainer::DeleteSprite(Sprite* pInSprite)
{
	auto it = _sprites.find(pInSprite->GetId());
	if(it == _sprites.end())
	{
		return;
	}
	_spritesInZOrder.remove(pInSprite);
	_visibleSpritesInZOrder.remove(pInSprite);
}

void SpriteContainer::MoveSpriteZOrder(Sprite* pInSprite, SpriteId ZBeforeSpriteId)
{
	_sprites.erase(pInSprite->GetId());
	_visibleSpritesInZOrder.remove(pInSprite);
	_InsertSpriteAfter(pInSprite, ZBeforeSpriteId);
}

void SpriteContainer::ShowSprite(Sprite* pInSprite)
{
	// WARNING!!! We may NOT check here, whether the sprite was visible or not before,
	// because its state already updated by the sprite.
	auto it_ZOrder = _FindIterator_InZOrder(pInSprite->GetId());
	_EnlistSpriteAsVisible(pInSprite, std::next(it_ZOrder));
}

void SpriteContainer::HideSprite(Sprite* pInSprite)
{
	// WARNING!!! We may NOT check here, whether the sprite was visible or not before,
	// because its state already updated by the sprite.
	auto it_ZOrder = _FindIterator_InZOrder(pInSprite->GetId());
	_visibleSpritesInZOrder.remove(pInSprite);
}

/**
* Last is a next-to-the-last-iterator
*
* @returns: nullopt if not found.
*/
template<class Iterator>
std::optional<Iterator> FindVisibleSpriteBackward(Iterator first, Iterator last)
{
	Iterator it = last;
	while( true )
	{
		if(it == first) 
		{
			return std::nullopt;
		}
		--it;
		if((*it)->IsVisible())
		{
			return it;
		}
	}
}

/**
* Enlists the given sprite into the list of visible sprites.
* Never checks whether the sprite already enlisted.
*
* @returns: iterator into the container of visible sprites.
*/
VisibleSpriteContainerByZOrder::iterator SpriteContainer::_EnlistSpriteAsVisible(Sprite* pInSprite, SpriteContainerByZOrder::iterator InItZAfterSprite)
{
	auto const opt_it_ZVisibleZBeforeSprite = FindVisibleSpriteBackward(_spritesInZOrder.begin(), InItZAfterSprite);
	if (opt_it_ZVisibleZBeforeSprite.has_value())
	{
		Sprite* const pVisibleZBeforeSprite = *opt_it_ZVisibleZBeforeSprite.value();
		VisibleSpriteContainerByZOrder::iterator const it_VisibleZBeforeSprite = _FindIterator_InZOrderVisible(pVisibleZBeforeSprite);
		return _visibleSpritesInZOrder.insert(std::next(it_VisibleZBeforeSprite), pInSprite);
	}
	else
	{
		return _visibleSpritesInZOrder.insert(_visibleSpritesInZOrder.end(), pInSprite);
	}
}

/**
* Performs insertion into ZOrder and visible ZOrder container.
* @returns: iterator to ZOrder container of the inserted sprite.
*/
SpriteContainerByZOrder::iterator SpriteContainer::_InsertSpriteAfter(Sprite* pInSprite, SpriteId ZBeforeSpriteId)
{
	if(NULL_SPRITE_ID == ZBeforeSpriteId || _sprites.empty())
	{
		auto const it_ZOrder = _spritesInZOrder.insert(_spritesInZOrder.end(), pInSprite);
		_visibleSpritesInZOrder.insert(_visibleSpritesInZOrder.end(), pInSprite);
		return it_ZOrder;
	}
	
	// Because ZBeforeSpriteId must always correspond to an existing sprite,
	// the returned iterator is never end iterator, so the next iterator always exists.
	auto const it_ZBeforeSprite = _FindIterator_InZOrder(ZBeforeSpriteId);
	auto it_ZOrder = _spritesInZOrder.insert(std::next(it_ZBeforeSprite), pInSprite);

	auto const opt_it_ZVisibleZBeforeSprite = _EnlistSpriteAsVisible(pInSprite, std::next(it_ZBeforeSprite));

	return it_ZOrder;
}

SpriteContainerByZOrder::iterator SpriteContainer::_FindIterator_InZOrder(SpriteId InId)
{
	Sprite* const pSprite = FindById(InId);
	BOOST_ASSERT_MSG(pSprite, "SpriteContainer::_FindIterator_InZOrder: sprite with the given ID not registered");
	auto it = std::find(_spritesInZOrder.begin(), _spritesInZOrder.end(), pSprite);
	BOOST_ASSERT(it != _spritesInZOrder.end());
	return it;
}

VisibleSpriteContainerByZOrder::iterator SpriteContainer::_FindIterator_InZOrderVisible(Sprite* pInSprite)
{
	auto it = std::find(_visibleSpritesInZOrder.begin(), _visibleSpritesInZOrder.end(), pInSprite);
	BOOST_ASSERT_MSG(it != _visibleSpritesInZOrder.end(), "SpriteContainer::_FindIterator_InZOrderVisible: sprite with the given pointer is NOT registered in the container of visible sprites sorted by ZOrder");
	return it;
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv