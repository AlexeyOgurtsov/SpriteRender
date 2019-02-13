#include "D3DSubsysServiceRegister.h"
#include "D3DSubsysServiceTypeTraits.h"
#include "D3DSubsysServiceFactory.h"
#include "Util/ServiceContainer/ServiceContainerTypedInterface.h"
#include <memory>
#include <boost/assert.hpp>

namespace Tet
{
	void RegSvc_D3DSubsys(const Tet::D3DSubsysInitializer& InInitializer, Eng::Svc::IServiceContainer* pInSvcContainer)
	{
		static std::unique_ptr<D3DSubsysServiceFactory> pFactory;
		BOOST_ASSERT_MSG(nullptr == pFactory.get(), "Tet::RegSvc_D3DSubsys: D3DSubsys svc already registered");
		pFactory.reset(new D3DSubsysServiceFactory{ InInitializer });
		Eng::Svc::RegisterService(pInSvcContainer, D3DSubsysServiceTypeTraits{}, pFactory.get());
	}
} // Tet