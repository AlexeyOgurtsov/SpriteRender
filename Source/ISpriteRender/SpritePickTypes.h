#pragma once

#include "ISprite/SpriteTypedefs.h"
#include <d3d11.h>
#include <boost/assert.hpp>
#include <iomanip>

namespace Dv
{
namespace Spr
{
namespace Ren
{
	/**
	* Result of picking.
	*/
	struct SPickResult
	{
		/**
		* Id of the picked object (valid only if pick result is valid).
		*/
		PickObjectId ObjectId = ZERO_PICK_OBJECT_ID;

		/**
		* Returns true if contains a valid meaningful pick result.
		*/
		bool IsValid() const { return IsValidPickObjectId(ObjectId); }

		/**
		* Returns true if pick failed.
		*/
		bool IsFailed() const { return false == IsValid(); }

		/**
		* Default ctor: creates result of invalid picking.
		*/
		SPickResult() = default;

		/**
		* Ctor: normal or special pick id may be passed.
		*/
		SPickResult(PickObjectId InObjectId) :
			ObjectId{InObjectId} 
		{
			BOOST_ASSERT
			(
				IsValid()
				|| (ZERO_PICK_OBJECT_ID == ObjectId)
			);
		}
	};
	template<class Strm> 
	Strm& operator<<(Strm& S, const SPickResult& InResult)
	{
		S << "ObjectId: " << ObjectId << " (" << GetPickObjectIdValidityStr(ObjectId) << ")" << std::endl;
		return S;
	}
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv