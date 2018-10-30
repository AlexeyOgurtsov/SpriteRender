#include "CanvasManagerContainerTypes.h"
#include "../Canvas/Canvas.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{
	SpriteCanvasListIterator::SpriteCanvasListIterator() :
		pCont{ nullptr }
		, Visibility{ ESpriteCanvasVisibilityFilter::None }
	{
	}

	SpriteCanvasListIterator::SpriteCanvasListIterator(const ContType* InCont, ConstIteratorType InIt, ESpriteCanvasVisibilityFilter InVisibility) :
		It{ InIt }
		, pCont{ InCont }
		, Visibility{ InVisibility }
	{
		MakeEnd_IfIteratorEnd();
		if (IsNotEnd() && false == CanvasValidForIterator())
		{
			++*this;
		}
	}

	bool SpriteCanvasListIterator::MakeEnd_IfIteratorEnd()
	{
		if (It == pCont->cend())
		{
			pCont = nullptr;
			return true;
		}
		return false;
	}

	Canvas* SpriteCanvasListIterator::operator*() const
	{
		return GetPtr();
	}

	Canvas* SpriteCanvasListIterator::GetPtr() const
	{
		BOOST_ASSERT(IsNotEnd());
		return *It;
	}

	Canvas* SpriteCanvasListIterator::operator->() const
	{
		BOOST_ASSERT(IsNotEnd());
		return GetPtr();
	}

	SpriteCanvasListIterator SpriteCanvasListIterator::operator++()
	{
		BOOST_ASSERT(IsNotEnd());
		while (true)
		{
			++It;
			if (MakeEnd_IfIteratorEnd())
			{
				return *this;
			}
			if (CanvasValidForIterator())
			{
				break;
			}
		}
		return *this;
	}

	bool SpriteCanvasListIterator::CanvasValidForIterator() const
	{
		BOOST_ASSERT(IsNotEnd());
		switch (Visibility)
		{
		case ESpriteCanvasVisibilityFilter::None:
			return true;

		case ESpriteCanvasVisibilityFilter::OnlyHidden:
			return (false == (*It)->IsVisible());

		case ESpriteCanvasVisibilityFilter::OnlyVisible:
			return (*It)->IsVisible();
		}
		return true;
	}

	SpriteCanvasListIterator SpriteCanvasListIterator::operator++(int)
	{
		auto It = *this;
		++It;
		return It;
	}

	SpriteCanvasListIterator SpriteCanvasListIterator::operator--()
	{
		while (true)
		{
			--It;
			if (MakeEnd_IfIteratorEnd())
			{
				return *this;
			}
			if (CanvasValidForIterator())
			{
				break;
			}
		}
		return *this;
	}

	SpriteCanvasListIterator SpriteCanvasListIterator::operator--(int)
	{
		auto It = *this;
		--It;
		return It;
	}

	bool SpriteCanvasListIterator::IsNotEnd() const
	{
		return !IsEnd();
	}

	bool SpriteCanvasListIterator::operator==(const SpriteCanvasListIterator& InOther)
	{
		if (IsEnd() || InOther.IsEnd())
		{
			if (InOther.pCont == pCont)
			{
				return true;
			}
		}
		else
		{
			return false;
		}
		BOOST_ASSERT_MSG(IsEnd() && InOther.IsEnd(), "At this point both ptr's are NOT ends");
		BOOST_ASSERT(InOther.pCont == pCont);
		return It == InOther.It;
	}

	bool SpriteCanvasListIterator::operator!=(const SpriteCanvasListIterator& InOther)
	{
		return !(operator==(InOther));
	}
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv