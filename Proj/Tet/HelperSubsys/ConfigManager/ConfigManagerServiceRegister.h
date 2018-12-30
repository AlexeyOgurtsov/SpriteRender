#pragma once

#include <string_view>

namespace Eng::Svc
{
	class IServiceContainer;
} // Eng::Svc

namespace Tet
{
	void RegSvc_ConfigManager(std::string_view InName, Eng::Svc::IServiceContainer* pInSvcContainer);
} // Tet