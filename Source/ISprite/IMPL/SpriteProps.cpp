#include "../SpriteProps.h"
#include "boost/assert.hpp"

namespace Dv
{
namespace Spr
{ 
	bool SSpritePickProps::CanBePicked() const
	{
		return IsValidPickObjectId(PickId);
	}

	const SSpritePickProps SSpritePickProps::Disabled { ZERO_PICK_OBJECT_ID };
} // Dv::Spr
} // Dv