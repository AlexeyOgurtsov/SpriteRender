#include "QSpriteRenderServiceFactory.h"
#include "QSpriteRender/INTERFACE/SpriteRenderSubsystemCreate.h"

namespace Tet
{	
	QSpriteRenderServiceFactory::ServiceObjectType* QSpriteRenderServiceFactory::CreateTyped(ResolvedServiceType pInThisService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer)
	{
		return Dv::Spr::QRen::CreateSpriteRenderSubsystem(Initializer).release();
	}

	void QSpriteRenderServiceFactory::Destroy(ResolvedServiceType pInService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer)
	{
		// The service itself should NOT be destroyed here,
		// it will be destroyed automatically.
	}
} // Tet