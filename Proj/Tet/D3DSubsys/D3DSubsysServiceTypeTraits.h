#pragma once

#include "Util/ServiceContainer/ServiceTypeTraits.h"
#include "D3DSubsysServiceTypes.h"
#include "D3DSubsys.h"

namespace Tet
{
	struct D3DSubsysServiceInstanceType : public Eng::Svc::TServiceInstanceType<Tet::D3DSubsys>
	{
		D3DSubsysServiceInstanceType() :
			Eng::Svc::TServiceInstanceType<Tet::D3DSubsys> { Tet::SvcName_D3DSubsys }
		{
		}
	};

	struct D3DSubsysServiceTypeTraits : public Eng::Svc::TServiceTypeTraits<Tet::D3DSubsys>
	{
		D3DSubsysServiceTypeTraits() : 
			Eng::Svc::TServiceTypeTraits<Tet::D3DSubsys> { Tet::SvcName_D3DSubsys }
		{
		}
	};
} // Tet