#pragma once

#include "Util/ServiceContainer/IServiceFactory.h"
#include "QSpriteRenderServiceTypeTraits.h"
#include "QSpriteRender.h"
#include "QSpriteRenderSubsysInitializer.h"
#include "QSpriteRender/INTERFACE/SpriteRenderSubsystemInitializer.h" // @TODO: Remove

namespace Tet
{
// @TODO: Remove (use QSpriteRenderSubsysInitializer instead)
	using QSpriteRenderSubsystemInitializer = ::Dv::Spr::QRen::SSpriteRenderSubsystemInitializer;

	class QSpriteRenderServiceFactory : public Eng::Svc::TTypedServiceFactory<QSpriteRenderSubsystem>
	{
	public:
// @TODO: use QSpriteRenderSubsysInitializer instead
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
// @TODO: use QSpriteRenderSubsysInitializer instead
		QSpriteRenderSubsystemInitializer Initializer;
	};
} // Tet