#pragma once

#include "Util/ServiceContainer/IServiceFactory.h"
#include "D3DSubsysServiceTypeTraits.h"
#include "D3DSubsys.h"

namespace Tet
{
	using D3DSubsysInitializer = ::Tet::D3DSubsysInitializer;

	class D3DSubsysServiceFactory : public Eng::Svc::TTypedServiceFactory<D3DSubsys>
	{
	public:
		D3DSubsysServiceFactory(const D3DSubsysInitializer& InInitializer)
		:	Eng::Svc::TTypedServiceFactory<D3DSubsys>{ D3DSubsysServiceInstanceType{} }
		,	Initializer{InInitializer} {}
	
	protected:
		/**
		* Typed service factory functions
		*/
		virtual ServiceObjectType* CreateTyped(ResolvedServiceType pInThisService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer) override;
		virtual void Destroy(ResolvedServiceType pInService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer) override;
		
	private:
		D3DSubsysInitializer Initializer;
	};
} // Tet