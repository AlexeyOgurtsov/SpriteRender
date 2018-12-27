#pragma once

#include "ServiceProps.h"

namespace Eng
{
namespace Svc
{

template<class ServiceObjectTypeArg>
struct TServiceInstanceType
{
	using ServiceObjectType = ServiceObjectTypeArg;
	ServiceTypeName ServiceTypeName;

	TServiceInstanceType(ServiceTypeNameConstRef InServiceTypeName) :
		ServiceTypeName(InServiceTypeName) {}
};

/**
* Base clase of the service type traits
*/
template<class ServiceObjectTypeArg>
struct TServiceTypeTraits
{
	using ServiceObjectType = ServiceObjectTypeArg;
	SServiceProps Props;

	TServiceTypeTraits(ServiceTypeNameConstRef InServiceTypeName) :
		Props{ SServiceProps { InServiceTypeName } } {}
	TServiceTypeTraits(const SServiceProps& InProps) :
		Props{ InProps } {}
};

} // Eng::Svc

} // Eng