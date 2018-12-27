#pragma once

namespace Eng
{
namespace Svc
{

struct SServiceProps;
class IServiceContainer;
class IServiceFactory;
class IResolvedService;

/**
* Registered (but NOT necessarily resolved) service
*/
class IService
{
public:
	virtual ~IService() = default;

	/**
	* Owner container
	*/
	virtual IServiceContainer* GetServiceContainer() const = 0;

	/**
	* Properties
	*/
	virtual const SServiceProps& GetProps() const = 0;

	/**
	* Factory
	*/
	virtual IServiceFactory* GetFactory() const = 0;

	/**
	* Count of resolved instances of this service.
	*/
	virtual int GetNumResolved() const = 0;

	/**
	* Associated resolved service with the given index, if the service was resolved.
	* @Precondition: Index must be in range [0, Count of resolved)
	*/
	virtual IResolvedService* GetResolved(int InIndex) const = 0;
};

} // Svc::Eng
} // Eng