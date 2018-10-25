#include "SpriteContainerTypes.h"
#include "Sprite.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

	SpriteListIterator::SpriteListIterator() :
		pCont{ nullptr }
		, Visibility{ ESpriteVisibilityFilter::None }
	{
	}

	SpriteListIterator::SpriteListIterator(const ContType* InCont, ConstIteratorType InIt, ESpriteVisibilityFilter InVisibility) :
		It{ InIt }
		, pCont{ InCont }
		, Visibility{ InVisibility }
	{
		MakeEnd_IfIteratorEnd();
		if (IsNotEnd() && false == SpriteValidForIterator())
		{
			++*this;
		}
	}

	bool SpriteListIterator::MakeEnd_IfIteratorEnd()
	{
		if (It == pCont->cend())
		{
			pCont = nullptr;
			return true;
		}
		return false;
	}

	const Sprite* SpriteListIterator::operator*() const
	{
		return GetPtr();
	}

	const Sprite* SpriteListIterator::GetPtr() const
	{
		BOOST_ASSERT(IsNotEnd());
		return *It;
	}

	const Sprite* SpriteListIterator::operator->() const
	{
		BOOST_ASSERT(IsNotEnd());
		return GetPtr();
	}

	SpriteListIterator SpriteListIterator::operator++()
	{
		BOOST_ASSERT(IsNotEnd());
		while (true)
		{
			++It;
			if (MakeEnd_IfIteratorEnd())
			{
				return *this;
			}
			if (SpriteValidForIterator())
			{
				break;
			}
		}
		return *this;
	}

	bool SpriteListIterator::SpriteValidForIterator() const
	{
		BOOST_ASSERT(IsNotEnd());
		switch (Visibility)
		{
		case ESpriteVisibilityFilter::None:
			return true;

		case ESpriteVisibilityFilter::OnlyHidden:
			return (false == (*It)->IsVisible());

		case ESpriteVisibilityFilter::OnlyVisible:
			return (*It)->IsVisible();
		}
		return true;
	}

	SpriteListIterator SpriteListIterator::operator++(int)
	{
		auto It = *this;
		++It;
		return It;
	}

	SpriteListIterator SpriteListIterator::operator--()
	{
		while (true)
		{
			--It;
			if (MakeEnd_IfIteratorEnd())
			{
				return *this;
			}
			if (SpriteValidForIterator())
			{
				break;
			}
		}
		return *this;
	}

	SpriteListIterator SpriteListIterator::operator--(int)
	{
		auto It = *this;
		--It;
		return It;
	}

	bool SpriteListIterator::IsNotEnd() const
	{
		return !IsEnd();
	}

	bool SpriteListIterator::operator==(const SpriteListIterator& InOther)
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

	bool SpriteListIterator::operator!=(const SpriteListIterator& InOther)
	{
		return !(operator==(InOther));
	}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv