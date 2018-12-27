#include "ServiceContainerAccess.h"
#include "IMPL/ServiceContainer.h"

namespace Eng
{
namespace Svc
{

/**
* Factory function for the default service container.
*/
ServiceContainerHandle CreateServiceContainer(const SServiceContainerInitializer& InInitializer)
{
	return ServiceContainerHandle{new IMPL::ServiceContainer{InInitializer}};
}

} // Eng::Svc
} // Eng
