#pragma once

#include <list>
#include <memory>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{
	class Sprite;

	using SpriteList = std::list<Sprite*>;
	using SpriteStorageList = std::list<std::unique_ptr<Sprite>>;

	enum class ESpriteVisibilityFilter
	{
		None,
		OnlyHidden,
		OnlyVisible
	};

	class SpriteListIterator
	{
	public:
		using ContType = SpriteList;
		using ConstIteratorType = ContType::const_iterator;

		/**
		* Creates End iterator.
		*/
		SpriteListIterator();
		SpriteListIterator(const ContType* InCont, ConstIteratorType InIt, ESpriteVisibilityFilter InVisibility);

		bool operator! () const { return IsEnd(); }
		bool IsEnd() const { return pCont == nullptr; }
		bool IsNotEnd() const;
		operator bool() const { return IsNotEnd(); }

		const Sprite* operator*() const;
		const Sprite* GetPtr() const;
		const Sprite* operator->() const;

		SpriteListIterator operator++();
		SpriteListIterator operator++(int);
		SpriteListIterator operator--();
		SpriteListIterator operator--(int);

		bool operator==(const SpriteListIterator& InOther);
		bool operator!=(const SpriteListIterator& InOther);

	private:
		bool MakeEnd_IfIteratorEnd();
		bool SpriteValidForIterator() const;

		ESpriteVisibilityFilter Visibility;
		const ContType * pCont = nullptr;
		ConstIteratorType It;
	};

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv