#pragma once

#include "IResolvedService.h"
#include "ServiceTypeTraits.h"

namespace Eng
{
namespace Svc
{

/**
* Typed resolved service handle.
*
* Always to be passed by value (pointer semantics)
*/
template<class ServiceObjectTypeArg>
class TResolvedService
{
public:
	using ServiceObjectType = ServiceObjectTypeArg;

	TResolvedService(IResolvedService* pInResolvedService) :
		pResolvedService(pInResolvedService) {}

	IResolvedService* operator->() const { return pResolvedService; }
	IResolvedService* GetResolvedService() const { return pResolvedService; }
	ServiceObjectType* GetServiceObject() const
	{
		return static_cast<ServiceObjectType*>(pResolvedService->GetServiceObject());
	}
private:
	IResolvedService* pResolvedService;
};

} // Eng::Svc
} // Eng