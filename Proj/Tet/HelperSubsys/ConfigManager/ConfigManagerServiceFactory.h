#pragma once

#include "Util/ServiceContainer/IServiceFactory.h"
#include "ConfigManagerServiceTypeTraits.h"
#include "ConfigManager.h"

namespace Tet
{
	class ÑonfigManagerServiceFactory : public Eng::Svc::TTypedServiceFactory<Tet::CfgManager>
	{
	public:
		ÑonfigManagerServiceFactory(std::string_view InName) 
		:	Eng::Svc::TTypedServiceFactory<Tet::CfgManager>{ ConfigManagerServiceInstanceType{} }
		,	Name{ InName } {}

	protected:
		/**
		* Typed service factory functions
		*/
		virtual ServiceObjectType* CreateTyped(ResolvedServiceType pInThisService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer) override
		{
			return new Tet::CfgManager(Name);
		}

		virtual void Destroy(ResolvedServiceType pInService, Eng::Svc::IServiceContainerFactoryAPI* pInSvcContainer) override
		{
			// The service itself should NOT be destroyed here,
			// it will be destroyed automatically.
		}

	private:
		std::string Name;
	};
} // Tet