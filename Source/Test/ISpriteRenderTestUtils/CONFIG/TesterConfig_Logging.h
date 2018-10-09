#pragma once

#include <string>

namespace Test
{
	struct TesterConfig_Logging
	{
		std::string BaseLogPath;
		bool bLoggingEnabled = true;
		
		std::string MainLogName = "MainLog.txt";
	};
	std::string GetMainLogFullFilename(const TesterConfig_Logging& InConfig);
} // Test