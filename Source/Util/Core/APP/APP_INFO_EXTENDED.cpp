#include "APP_INFO_EXTENDED.h"

namespace Dv::Core
{
	void AppendAppInfoExtendedToString(std::string* pOutString, const AppInfoExtended& InValue)
	{
		AppendAppInfoToString(pOutString, InValue.Info);
		pOutString->append(";\n");
		pOutString->append(InValue.Description);
		if (InValue.Description.empty() || InValue.Description.back() != '\n')
		{
			pOutString->append("\n");
		}
	}

	std::string ToString(const AppInfoExtended& InValue)
	{
		std::string result;
		AppendAppInfoExtendedToString(&result, InValue);
		return result;
	}

	void AppendAppInfoToString(std::string* pOutString, const AppInfo& InValue)
	{
		pOutString->append(InValue.Name);
		pOutString->append(";");
		AppendAppVersionToString(pOutString, InValue.Version);
	}

	std::string ToString(const AppInfo& InValue)
	{
		std::string result;
		AppendAppInfoToString(&result, InValue);
		return result;
	}
} // Dv::Core