#pragma once

#include "AppEnvironment.h"
#include <windows.h>

namespace Eng::Svc
{
	class IServiceContainer;
} // Eng::Svc

namespace Tet
{
	AppEnvironment DeployEnvironment(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont);
	AmbientContext DeployAmbientEnvironment(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont);
} // Tet