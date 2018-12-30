#pragma once

#include "Util/ServiceContainer/ServiceTypeTraits.h"
#include "ConfigManagerServiceTypes.h"
#include "ConfigManager.h"

namespace Tet
{
	struct ConfigManagerServiceInstanceType : public Eng::Svc::TServiceInstanceType<Tet::CfgManager>
	{
		ConfigManagerServiceInstanceType() :
			Eng::Svc::TServiceInstanceType<Tet::CfgManager> { Tet::SvcName_ConfigManager }
		{
		}
	};

	struct ConfigManagerServiceTypeTraits : public Eng::Svc::TServiceTypeTraits<Tet::CfgManager>
	{
		ConfigManagerServiceTypeTraits() : 
			Eng::Svc::TServiceTypeTraits<Tet::CfgManager> { Tet::SvcName_ConfigManager }
		{
		}
	};
} // Tet