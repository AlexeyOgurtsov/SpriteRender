#pragma once

/**
* Contains all headers, related to implementing a typed factory.
*/

#include "IServiceContainerFactoryAPI.h"
#include "TypedResolvedService.h"

namespace Eng
{
namespace Svc
{

/**
* Perform dependency resolution in a type-safe manner.
*/
template<class DependencyServiceInstanceTypeArg, class ForServiceObjectType>
TResolvedService<DependencyServiceInstanceTypeArg> ResolveDependency
(
	IServiceContainerFactoryAPI* pContainer,
	const DependencyServiceInstanceTypeArg& InDependencyServiceInstanceType,
	TResolvedService<ForServiceObjectType> pForService
);

template<class ServiceObjectType>
void ReleaseDependency(IServiceContainerFactoryAPI* pContainer, TResolvedService<ServiceObjectType> pService);



/**
* IMPLEMENTATION
*/
template<class DependencyServiceInstanceTypeArg, class ForServiceObjectType>
TResolvedService<DependencyServiceInstanceTypeArg> ResolveDependency
(
	IServiceContainerFactoryAPI* pContainer,
	const DependencyServiceInstanceTypeArg& InDependencyServiceInstanceType,
	TResolvedService<ForServiceObjectType> pForService
)
{
	IResolvedService* pResultService = pContainer->ResolveDependency(InDependencyServiceInstanceType.ServiceTypeName, pForService.GetResolvedService());
	return TResolvedService<DependencyServiceInstanceTypeArg>(pResultService);
}

template<class ServiceObjectType>
void ReleaseDependency(IServiceContainerFactoryAPI* pContainer, TResolvedService<ServiceObjectType> pService)
{
	pContainer->Release(pService.GetResolvedService());
}

} // Eng::Svc
} // Eng