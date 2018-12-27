#pragma once

/**
* Provides a full set of headers for using the service container
* through its abstract interface.
*/

#include "IServiceContainer.h"
#include "IService.h"
#include "IResolvedService.h"
#include "ServiceProps.h"
// We should NOT include the service factory interface header here,
// because typically we never use service factories directly,
// when using a service container.