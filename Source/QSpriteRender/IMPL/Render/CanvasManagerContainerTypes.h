#pragma once

#include <memory>
#include <fstream>
#include <list>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{
	class Canvas;

	using CanvasStorage = std::list<std::unique_ptr<Canvas>>;
	using CanvasList = std::list<Canvas*>;

	struct CanvasIterator
	{
		CanvasStorage::iterator It_CanvasStorage;
		CanvasList::iterator It_CanvasList;
	};

	enum class ESpriteCanvasVisibilityFilter
	{
		None,
		OnlyHidden,
		OnlyVisible
	};

	class SpriteCanvasListIterator
	{
	public:
		using ContType = CanvasList;
		using ConstIteratorType = ContType::const_iterator;

		/**
		* Creates End iterator.
		*/
		SpriteCanvasListIterator();
		SpriteCanvasListIterator(const ContType* InCont, ConstIteratorType InIt, ESpriteCanvasVisibilityFilter InVisibility);

		bool operator! () const { return IsEnd(); }
		bool IsEnd() const { return pCont == nullptr; }
		bool IsNotEnd() const;
		operator bool() const { return IsNotEnd(); }

		Canvas* operator*() const;
		Canvas* GetPtr() const;
		Canvas* operator->() const;

		SpriteCanvasListIterator operator++();
		SpriteCanvasListIterator operator++(int);
		SpriteCanvasListIterator operator--();
		SpriteCanvasListIterator operator--(int);

		bool operator==(const SpriteCanvasListIterator& InOther);
		bool operator!=(const SpriteCanvasListIterator& InOther);

	private:
		bool MakeEnd_IfIteratorEnd();
		bool CanvasValidForIterator() const;

		ESpriteCanvasVisibilityFilter Visibility;
		const ContType * pCont = nullptr;
		ConstIteratorType It;
	};
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv