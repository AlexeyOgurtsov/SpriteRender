#pragma once

#include "Util/ServiceContainer/IServiceFactory.h"
#include "QSpriteRenderServiceTypeTraits.h"
#include "QSpriteRender.h"
#include "QSpriteRender/INTERFACE/SpriteRenderSubsystemInitializer.h"

namespace Tet
{
	using QSpriteRenderSubsystemInitializer = ::Dv::Spr::QRen::SSpriteRenderSubsystemInitializer;

	class QSpriteRenderServiceFactory : public Eng::Svc::TTypedServiceFactory<QSpriteRenderSubsystem>
	{
	public:
		QSpriteRenderServiceFactory(const QSpriteRenderSubsystemInitializer& InInitializer)
		:	Eng::Svc::TTypedServiceFactory<QSpriteRenderSubsystem>{ QSpriteRenderServiceInstanceType{} }
		,	Initializer{InInitializer} {}
	
	protected:
		/**
		* Typed service factory functions
		*/
		virtual ServiceObjectType* CreateTyped(ResolvedServiceType pInThisService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer) override;
		virtual void Destroy(ResolvedServiceType pInService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer) override;
		
	private:
		QSpriteRenderSubsystemInitializer Initializer;
	};
} // Tet