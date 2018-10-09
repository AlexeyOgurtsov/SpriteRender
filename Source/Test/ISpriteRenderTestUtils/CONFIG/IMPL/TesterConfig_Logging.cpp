#include "../TesterConfig_Logging.h"

namespace Test
{
	std::string GetMainLogFullFilename(const TesterConfig_Logging& InConfig)
	{
		return InConfig.BaseLogPath + std::string("\\") + InConfig.MainLogName;
	}
} // Test