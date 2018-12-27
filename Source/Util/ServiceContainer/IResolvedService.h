#pragma once

#include "ServiceTypeId.h"

namespace Eng
{
namespace Svc
{

class IService;

using ResolvedServiceId = int;

/**
* Handle for the resolved service.
*/
class IResolvedService
{
public:
	virtual ~IResolvedService() = default;

	/**
	* Id of the resolved service
	*/
	virtual ResolvedServiceId GetId() const = 0;

	/**
	* Service
	*/
	virtual IService* GetService() const = 0;

	/**
	* Returns object that represents the service.
	*/
	virtual void* GetServiceObject() const = 0;

	/**
	* Releases the service.
	* @Implementation note: Typically delegates the release request to the manager.
	*/
	virtual void Release() = 0;

	/**
	* Returns resolved dependency for the given service type
	* @precondition: dependency must be resolved.
	*/
	virtual IResolvedService* GetResolvedDependency(ServiceTypeNameConstRef InServiceType) const = 0;
};

} // Eng::Svc
} // Eng