#pragma once

#include "IServiceContainerFactoryAPI.h"
#include "ServiceProps.h"
#include <memory> // unique_ptr

namespace Eng
{ 
namespace Svc
{

struct SServiceProps;
class IService;
class IServiceContainer;
class IServiceFactory;


using ServiceContainerHandle = std::unique_ptr<IServiceContainer>;


class IServiceContainer :
	public IServiceContainerFactoryAPI
{
public:
	virtual ~IServiceContainer() = default;

	/**
	* Associates the given factory with the given service.
	* @Precondition: Must be done only once for the given service type.
	*/
	virtual IService* Register(const SServiceProps& InProps, IServiceFactory* pInFactory) = 0;

	/**
	* Resolve the given service name.
	* @Precondition: The service with the given name must be registered.
	*/
	virtual IResolvedService* Resolve(ServiceTypeNameConstRef InServiceTypeName) = 0;
};

} // Eng::Svc
} // Eng

/**
* TODO:
* 1. Tinker the destruction order 
* (should be opposite NOT the order of IResolvedService instance creation,
* but to the ordered of FINISHING them).
*
* TODO Hint:
* 1. Check that service type is a subtype of the corresponding type.
*/