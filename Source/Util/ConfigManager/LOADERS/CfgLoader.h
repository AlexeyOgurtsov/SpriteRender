#pragma once

#include "../ConfigManager_Fwd.h"
#include "../CORE/ConfigManagerSystem.h"
#include "../CORE/SourceLocation.h"
#include "../CORE/InputStreamLexTypes.h"
#include "../CFG/ValueType_Types.h"
#include "CfgLoader_Types.h"

namespace Dv::ConfigManager
{
	class InputStream;

	CfgLoadResult LoadCfgFile(std::string_view InFilename, CfgManager* pManager, const CfgLoadParams& InParams = CfgLoadParams());
	CfgLoadResult LoadCfgFile(std::string_view InFilename, InputStream* pStream, CfgManager* pManager, const CfgLoadParams& InParams = CfgLoadParams());
} // Dv::ConfigManager