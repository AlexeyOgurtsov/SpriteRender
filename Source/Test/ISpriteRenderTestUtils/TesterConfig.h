#pragma once

#include "Config/TesterConfig_Tester.h"
#include "Config/TesterConfig_Logging.h"
#include "Config/TesterConfig_Viewport.h"
#include "Config/TesterConfig_Resources.h"
#include "Config/TesterConfig_Pick.h"
#include "Config/D3DDeviceConfig.h"

namespace Test
{
	/**
	* Entire configuration of the tester
	*/
	struct TesterConfig
	{
		TesterConfig_Tester Tester;
		TesterConfig_Logging Log;
		TesterConfig_Viewport Viewport;
		TesterConfig_D3DDevice D3D;
		TesterConfig_Resources Resources;
		TesterConfig_Pick Pick;
	};
} // Test