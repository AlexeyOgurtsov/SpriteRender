#pragma once

/**
* Provides a factory function for the default service container.
*/

// Service container interface header: because of ServiceContainerHandle
#include "IServiceContainer.h"
#include "ServiceContainerInitializer.h"

namespace Eng
{
namespace Svc
{

struct SServiceContainerInitializer;

/**
* Factory function for the default service container.
*/
ServiceContainerHandle CreateServiceContainer(const SServiceContainerInitializer& InInitializer);

} // Eng::Svc
} // Eng

