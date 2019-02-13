#include "D3DSubsysServiceFactory.h"

namespace Tet
{	
	D3DSubsysServiceFactory::ServiceObjectType* QSpriteRenderServiceFactory::CreateTyped(ResolvedServiceType pInThisService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer)
	{
		return new D3DSubsys(Initializer);
	}

	void D3DSubsysServiceFactory::Destroy(ResolvedServiceType pInService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer)
	{
		// The service itself should NOT be destroyed here,
		// it will be destroyed automatically.
	}
} // Tet