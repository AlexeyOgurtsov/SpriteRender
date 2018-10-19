#include "../Environment_PublicTypes.h"

namespace Test
{
	std::string EnvResetFlagsToString(UINT InFlags)
	{
		if (InFlags == 0) { return "0"; }

		std::string result;
		if (InFlags & RESET_NO_SPRITE_RENDER)
		{
			result.append("NO_SPRITE_RENDER;");
		}
		if (InFlags & RESET_NO_DEFAULT_RS)
		{
			result.append("NO_DEFAULT_RS;");
		}
		return result;
	}
} // Test