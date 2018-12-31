#include "QSpriteRenderServiceRegister.h"
#include "QSpriteRenderServiceTypeTraits.h"
#include "QSpriteRenderServiceFactory.h"
#include "Util/ServiceContainer/ServiceContainerTypedInterface.h"
#include <memory>
#include <boost/assert.hpp>

namespace Tet
{
	void RegSvc_QSpriteRender(const ::Dv::Spr::QRen::SSpriteRenderSubsystemInitializer& InInitializer, Eng::Svc::IServiceContainer* pInSvcContainer)
	{
		static std::unique_ptr<QSpriteRenderServiceFactory> pFactory;
		BOOST_ASSERT_MSG(nullptr == pFactory.get(), "Tet::RegSvc_QSpriteRender: QSpriteRender svc already registered");
		pFactory.reset(new QSpriteRenderServiceFactory{ InInitializer });
		Eng::Svc::RegisterService(pInSvcContainer, QSpriteRenderServiceTypeTraits{}, pFactory.get());
	}
} // Tet