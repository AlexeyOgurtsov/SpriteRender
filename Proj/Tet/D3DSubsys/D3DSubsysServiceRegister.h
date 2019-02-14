#pragma once

#include "D3DSubsysInitializer.h"

namespace Eng::Svc
{
	class IServiceContainer;
} // Eng::Svc

namespace Tet
{
	struct D3DSubsysInitializer;
	void RegSvc_D3DSubsys(const Tet::D3DSubsysInitializer& InInitializer, Eng::Svc::IServiceContainer* pInSvcContainer);
} // Tet