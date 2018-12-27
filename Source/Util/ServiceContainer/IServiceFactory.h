#pragma once

#include "TypedResolvedService.h"

namespace Eng
{ 
namespace Svc
{

class IServiceContainerFactoryAPI;
class IResolvedService;

/**
* Manages both creation and destruction of the service objects.
*/
class IServiceFactory
{
public:
	virtual ~IServiceFactory() = default;

	/**
	* Creates a service object.
	*
	* The implementation must resolve the necessary service dependencies
	* by itself, using the provided service container.
	*
	* @param pInThisService:
	*		Resolved service object related to the service, for which we create a service object
	* (passed because we need it to pass to the service container for resolving dependencies). 
	*
	* @param pInSvcContainer:
	*		Service container object to be used to resolve dependencies.
	*/
	virtual void* Create(IResolvedService* pInThisService, IServiceContainerFactoryAPI* pInSvcContainer) = 0;

	/**
	* Destroys a service object.
	*
	* The resolved service, corresponding to service, returned by the Create method of the same
	* factory object must be passed as an argument.
	*
	* The Release method must be manually called only for those dependencies,
	* that are no longer in use by the service object and all of its descendants.
	* Because of that we may NOT automotize the Release'ing process.
	*
	* WARNING!!! However, the Release method should NOT be called for the given service itself,
	* as it will be handled automatically.
	*/
	virtual void Destroy(IResolvedService* pInService, IServiceContainerFactoryAPI* pInSvcContainer) = 0;
};

/**
* Service factory attached type.
*
* WARNING!!! Typically you should derive factories from THIS factory class,
* instead of the IServiceFactory interface.
*/
template<class ServiceObjectTypeArg>
class TTypedServiceFactory : public IServiceFactory
{
public:
	using ServiceInstanceTypeType = TServiceInstanceType<ServiceObjectTypeArg>;
	using ServiceObjectType = ServiceObjectTypeArg;
	using ResolvedServiceType = TResolvedService<ServiceObjectType>;

	TTypedServiceFactory(const ServiceInstanceTypeType& InServiceInstanceType) :
		ServiceInstanceType{ InServiceInstanceType } {}

	/**
	* IServiceFactory interface implementation
	*
	* Sshould NOT be override, the typed function versions to overriden instead
	*/
	virtual void* Create(IResolvedService* pInThisService, IServiceContainerFactoryAPI* pInSvcContainer) override final
	{
		return CreateTyped(ResolvedServiceType(pInThisService), pInSvcContainer);
	}

	virtual void Destroy(IResolvedService* pInService, IServiceContainerFactoryAPI* pInSvcContainer) override	
	{
		return Destroy(ResolvedServiceType(pInService), pInSvcContainer);
	}

protected:
	ServiceInstanceTypeType ServiceInstanceType;
	/**
	* Typed service factory functions
	*/
	virtual ServiceObjectType* CreateTyped(ResolvedServiceType pInThisService, IServiceContainerFactoryAPI* pInSvcContainer) = 0;
	virtual void Destroy(ResolvedServiceType pInService, IServiceContainerFactoryAPI* pInSvcContainer) = 0;
};

} // Eng::Svc
} // Eng