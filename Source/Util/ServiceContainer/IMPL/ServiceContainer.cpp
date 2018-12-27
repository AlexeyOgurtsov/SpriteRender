#include "ServiceContainer.h"
#include "../ServiceContainerInitializer.h"
#include "../IServiceFactory.h"
#include <boost/assert.hpp>


namespace Eng
{
namespace Svc
{
namespace IMPL
{

ServiceContainer::ServiceContainer(const SServiceContainerInitializer& InInitializer)
{
}

IService* ServiceContainer::Register(const SServiceProps& InProps, IServiceFactory* pInFactory)
{
	BOOST_ASSERT_MSG(ServicesByServiceType.cend() == ServicesByServiceType.find(InProps.Name), "ServiceContainer::Register: factory for the given service type is already registered");

	IService* pService = Services.Add(Service{this, InProps, pInFactory});
	ServicesByServiceType.emplace(std::make_pair(InProps.Name, Services.LastIndex()));
	return pService;
}

IResolvedService* ServiceContainer::Resolve(ServiceTypeNameConstRef InServiceTypeName)
{
	Service* pService = TryGetServiceByTypeName(InServiceTypeName);
	BOOST_ASSERT_MSG(pService, "ServiceContainer::Resolve: service with the given type is NOT registered");

	// Now we support only the singleton lifetime management pattern,
	// that's why only one resolved service is allowed
	BOOST_ASSERT_MSG(pService->GetNumResolved() <= 1, "ServiceContainer::Resolve: now we support maximum one resolved service per service type");

	if (pService->IsSingletonResolved())
	{
		ResolvedService* pResolvedSingleton = pService->GetResolvedSingleton();

		// Here we preventing recursive dependencies
		BOOST_ASSERT_MSG(pResolvedSingleton->IsFinished(), "ServiceContainer::Resolve: recursive dependency detected - service is in the middle of the resolution, but resolution not yet finished");

		return pResolvedSingleton;
	}

	ResolvedServiceId resolvedServiceId = ResolvedServicesById.Len();
	auto pResolvedServiceHandle = std::make_unique<ResolvedService>(resolvedServiceId, pService);
	ResolvedService* pResolvedService = pResolvedServiceHandle.get();
	ResolvedServicesById.Add(pResolvedService);
	BOOST_ASSERT(ResolvedServicesById.LastIndex() == ResolvedServicesById.Last()->GetId());
	pService->AddResolved(std::move(pResolvedServiceHandle));

	void* pServiceObject = pService->GetFactory()->Create(pResolvedService, this);
	BOOST_ASSERT_MSG(pServiceObject, "ServiceContainer::Resolve: IServiceFactory::Create returned nullptr");

	_FinishServiceResolution(pResolvedService, pServiceObject);

	return pResolvedService;
}

IResolvedService* ServiceContainer::ResolveDependency(ServiceTypeNameConstRef InServiceTypeName, IResolvedService* pForService)
{
	BOOST_ASSERT_MSG(this == pForService->GetService()->GetServiceContainer(), "ServiceContainer::ResolveDependency: the service for which we resolve dependency must belong to this service container");
	IResolvedService* pDependency = Resolve(InServiceTypeName);
	ResolvedService* pFor = ResolvedServicesById[pForService->GetId()];
	pFor->SetDependencyResolved(InServiceTypeName, pDependency);
	return pDependency;
}

void ServiceContainer::Release(IResolvedService* pInService)
{
	// DO nothing:
	// Now we never destroy the objects as they are always singletons). 
}

ServiceContainer::~ServiceContainer()
{
	_PurgeAllServiceObjects();
}


void ServiceContainer::_FinishServiceResolution(ResolvedService* pService, void *pServiceObject)
{
	pService->MarkFinished(pServiceObject);
	FinishedResolvedServices.Add(pService);
}

void ServiceContainer::_PurgeAllServiceObjects()
{
	// Always destroy according to order opposite to the resolution order
	for (int resolvedServiceIndex = FinishedResolvedServices.Len(); --resolvedServiceIndex >= 0;)
	{
		IResolvedService* pResolvedService = FinishedResolvedServices[resolvedServiceIndex];
		pResolvedService->GetService()->GetFactory()->Destroy(pResolvedService, this);
	}
}

Service* ServiceContainer::TryGetServiceByTypeName(ServiceTypeNameConstRef InServiceTypeName)
{
	auto it = ServicesByServiceType.find(InServiceTypeName);
	if (it == ServicesByServiceType.cend()) { return nullptr; }
	auto pService = &(Services[it->second]);
	return pService;
}

Service::Service(IServiceContainer* pInContainer, const SServiceProps& InProps, IServiceFactory* pInFactory) :
	pContainer(pInContainer)
,	Props(InProps)
,	pFactory(pInFactory)
,	pNullOrResolvedService(nullptr)
{
	BOOST_ASSERT(pContainer);
	BOOST_ASSERT(pFactory);
}

int Service::GetNumResolved() const
{
	return (nullptr == pNullOrResolvedService) ? 0 : 1;
}

IResolvedService* Service::GetResolved(int InIndex) const
{
	BOOST_ASSERT(InIndex >= 0);
	BOOST_ASSERT(InIndex < GetNumResolved());
	return pNullOrResolvedService.get();
}

bool Service::IsSingletonResolved() const
{
	return (1 == GetNumResolved()) && (nullptr != pNullOrResolvedService);
}

ResolvedService* Service::GetResolvedSingleton() const
{
	BOOST_ASSERT_MSG(IsSingletonResolved(), "Service::GetResolvedSingleton: the given service is not a singleton or not resolved");
	return pNullOrResolvedService.get();
}

void Service::AddResolved(std::unique_ptr<ResolvedService>&& pInResolved)
{
	BOOST_ASSERT(pInResolved);
	BOOST_ASSERT_MSG(nullptr == pNullOrResolvedService, "Service::AddResolved: you may add resolved service only if resolved service NOT yet added");
	pNullOrResolvedService = std::move(pInResolved);
}

ResolvedService::ResolvedService(ResolvedServiceId InId, IService* pInService) :
	Id(InId)
,	pService(pInService)
,   pServiceObject(nullptr)
{
	BOOST_ASSERT(pService);
}

void ResolvedService::Release()
{
	pService->GetServiceContainer()->Release(this);
}

IResolvedService* ResolvedService::GetResolvedDependency(ServiceTypeNameConstRef InServiceType) const
{
	auto it = DependenciesByServiceTypeName.find(InServiceType);
	BOOST_ASSERT_MSG(it != DependenciesByServiceTypeName.cend(), "ResolvedService::GetResolvedDependency: Dependency for the given service type is NOT resolved");
	return it->second;
}

void ResolvedService::SetDependencyResolved(ServiceTypeNameConstRef InServiceType, IResolvedService* pInDependency)
{
	BOOST_ASSERT(pInDependency);
	auto [it, bRegisteredNow] = DependenciesByServiceTypeName.emplace(InServiceType, pInDependency);
	BOOST_ASSERT_MSG(bRegisteredNow, "ResolvedService::SetDependencyResolved: Dependency may NOT be resolved more than once for the same service type");
}

void ResolvedService::MarkFinished(void* pInServiceObject)
{
	BOOST_ASSERT(pInServiceObject);
	BOOST_ASSERT_MSG(nullptr == pServiceObject, "ResolvedService::MarkFinished: service may NOT be resolved yet");
	pServiceObject = pInServiceObject;
}

} // Eng::Svc::IMPL
} // Eng::Svc
} // Eng