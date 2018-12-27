#pragma once

#include "ServiceTypeId.h"

namespace Eng
{ 
namespace Svc
{

class IResolvedService;

/**
* Subset of the service container interface, available for the service factory.
*/
class IServiceContainerFactoryAPI
{
public:
	virtual ~IServiceContainerFactoryAPI() = default;

	/**
	* Resolves dependency for the given service.
	* @Preconditions: All preconditions of the Resolve method.
	*/
	virtual IResolvedService* ResolveDependency(ServiceTypeNameConstRef InServiceTypeName, IResolvedService* pForService) = 0;

	/**
	* Releases the given resolved service
	* @Implementation note: typically searches service by the unique name of the service type or its id.
	*/
	virtual void Release(IResolvedService* pInService) = 0;
}; 

} // Eng::Svc
} // Eng