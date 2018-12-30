#include "AppDeployer.h"
#include "Util/ServiceContainer/ServiceContainerTypedInterface.h"
#include "Tet/HelperSubsys/Log/LogServiceTypeTraits.h"
#include "Tet/HelperSubsys/ConfigManager/ConfigManagerServiceTypeTraits.h"
#include "Tet/HelperSubsys/ConfigManager/ConfigManagerServiceTypeTraits.h"

namespace Tet
{
	Tet::AppEnvironment DeployEnvironment(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont)
	{
		AppEnvironment Env;
		Env.SetAmbient(DeployAmbientEnvironment(hInstance, lpCmdLine, pInSvcCont));
		return Env;
	}

	Tet::AmbientContext DeployAmbientEnvironment(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont)
	{
		Eng::Svc::TResolvedService<Tet::Log> Log = Eng::Svc::Resolve(pInSvcCont, LogServiceInstanceType{});
		Eng::Svc::TResolvedService<Tet::CfgManager> CfgManager = Eng::Svc::Resolve(pInSvcCont, ConfigManagerServiceInstanceType{});

		AmbientContext Ambient 
		{
			Log.GetServiceObject(),
			CfgManager.GetServiceObject()
		};
		return Ambient;
	}
} // Tet