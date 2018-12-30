#include "ConfigManagerServiceRegister.h"
#include "ConfigManagerServiceTypeTraits.h"
#include "ConfigManagerServiceFactory.h"
#include "Util/ServiceContainer/ServiceContainerTypedInterface.h"
#include <memory>
#include <boost/assert.hpp>

namespace Tet
{
	void RegSvc_ConfigManager(std::string_view InName, Eng::Svc::IServiceContainer* pInSvcContainer)
	{
		static std::unique_ptr<ÑonfigManagerServiceFactory> pFactory;
		BOOST_ASSERT_MSG(nullptr == pFactory.get(), "Tet::RegSvc_ConfigManager: Config manager svc already registered");
		pFactory.reset(new ÑonfigManagerServiceFactory{InName});
		Eng::Svc::RegisterService(pInSvcContainer, ConfigManagerServiceTypeTraits{}, pFactory.get());
	}
} // Tet