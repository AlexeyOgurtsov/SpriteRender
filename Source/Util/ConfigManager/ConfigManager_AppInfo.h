#pragma once

#include "Core/APP/APP_INFO_EXTENDED.h"

namespace Dv::ConfigManager
{
	inline Core::AppInfoExtended CreateAppInfo()
	{
		std::string description;
		description.append("Slightly-slightly improved config manager:\n");
		description.append("+Constant support added\n");
		return Core::AppInfoExtended
		{
			Core::AppInfo 
			{
				 "ConfigManager", 
				Core::AppVersion(0,0,0,1)
			},
			description
		};
	}
} // Dv::ConfigManager