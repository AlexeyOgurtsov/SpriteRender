#include "../SpriteProps.h"
#include "boost/assert.hpp"

namespace Dv
{
namespace Spr
{ 
	bool SSpritePickProps::CanBePicked() const
	{
		return Mode != ESpritePickMode::Disabled;
	}

	const SSpritePickProps SSpritePickProps::Disabled { ZERO_PICK_OBJECT_ID, ESpritePickMode::Disabled };
} // Dv::Spr
} // Dv