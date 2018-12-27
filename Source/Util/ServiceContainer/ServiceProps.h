#pragma once

/**
* Includes types that describes a service.
*/

#include "ServiceTypeId.h"

namespace Eng
{ 
namespace Svc
{

struct SServiceProps
{
	/**
	* Unique name.
	*/
	ServiceTypeName          Name;

	SServiceProps() = default;
	SServiceProps(ServiceTypeNameConstRef InName) :
		Name(InName) {};
};

} // Eng::Svc
} // Eng