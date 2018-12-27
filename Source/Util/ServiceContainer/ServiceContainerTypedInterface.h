#pragma once

/**
* Provides extra global functions and types for working 
* with the ServiceContainer in a type-safe mannter.
*/

#include "ServiceContainerInterface.h"
#include "ServiceTypeTraits.h"
#include "ServiceContainerFactoryInterface.h" // type-safe variants of the container interface subset for factories
#include "IServiceFactory.h" // for service factory type-safe checking

namespace Eng
{
namespace Svc
{

template<class ServiceTypeTraitsArg>
IService* RegisterService
(
	IServiceContainer* pContainer, 
	const ServiceTypeTraitsArg& ServiceType,
	TTypedServiceFactory<typename ServiceTypeTraitsArg::ServiceObjectType>* pFactory
);

template<class ServiceInstanceTypeArg>
TResolvedService<typename ServiceInstanceTypeArg::ServiceObjectType> Resolve
(
	IServiceContainer* pContainer, 
	const ServiceInstanceTypeArg& ServiceType
);



/**
* IMPLEMENTATION
*/
template<class ServiceTypeTraitsArg>
IService* RegisterService
(
	IServiceContainer* pContainer, 
	const ServiceTypeTraitsArg& ServiceType,
	TTypedServiceFactory<typename ServiceTypeTraitsArg::ServiceObjectType>* pFactory
)
{
	return pContainer->Register(ServiceType.Props, pFactory);
}


template<class ServiceInstanceTypeArg>
TResolvedService<typename ServiceInstanceTypeArg::ServiceObjectType> Resolve
(
	IServiceContainer* pContainer, 
	const ServiceInstanceTypeArg& ServiceType
)
{
	return TResolvedService<typename ServiceInstanceTypeArg::ServiceObjectType>{pContainer->Resolve(ServiceType.ServiceTypeName)};
}

} // Eng::Svc
} // Eng