#pragma once

#include <string_view>

namespace Dv::ConfigManager
{
	class CfgManager;
	class IValueType;

	IValueType* GetRegisteredValueType(CfgManager* pManager, std::string_view InName);
} // Dv::ConfigManager