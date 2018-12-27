#pragma once

#include "../ServiceContainerInterface.h"
#include "Core/Cont/TVector.h"
#include <unordered_map>
#include <memory>

namespace Eng
{
namespace Svc
{

struct SServiceContainerInitializer;

namespace IMPL
{

class ResolvedService;

class Service : public IService
{
public:
	Service(IServiceContainer* pInContainer, const SServiceProps& InProps, IServiceFactory* pInFactory);

	virtual IServiceContainer* GetServiceContainer() const override { return pContainer; }
	virtual const SServiceProps& GetProps() const override { return Props; }
	virtual IServiceFactory* GetFactory() const override { return pFactory; }

	virtual int GetNumResolved() const override;
	virtual IResolvedService* GetResolved(int InIndex) const override;

	/**
	* Check both that current service is resolved, and its lifetime management pattern is singleton
	*/
	bool IsSingletonResolved() const;

	/**
	* Gets resolved singleton
	* @precondition: must be singleton and must be resolved.
	*/
	ResolvedService* GetResolvedSingleton() const;

	void AddResolved(std::unique_ptr<ResolvedService>&& pInResolved);

private:
	IServiceContainer* pContainer;
	SServiceProps Props;
	IServiceFactory* pFactory;
	std::unique_ptr<ResolvedService> pNullOrResolvedService;
};

class ResolvedService : public IResolvedService
{
public:
	ResolvedService(ResolvedServiceId InId, IService* pInService);

	virtual ResolvedServiceId GetId() const override { return Id; }
	virtual IService* GetService() const override { return pService; }
	virtual void* GetServiceObject() const override { return pServiceObject; }
	virtual void Release() override;
	virtual IResolvedService* GetResolvedDependency(ServiceTypeNameConstRef InServiceType) const override;
	void SetDependencyResolved(ServiceTypeNameConstRef InServiceType, IResolvedService* pInDependency);
	void MarkFinished(void* pInServiceObject);
	bool IsFinished() const { return pServiceObject; }

private:
	ResolvedServiceId Id;
	IService* pService;
	void* pServiceObject;

	using DependencyMap = std::unordered_map<std::string_view, IResolvedService*>;
	DependencyMap DependenciesByServiceTypeName;
};

class ServiceContainer : public IServiceContainer
{
public:
	~ServiceContainer();
	ServiceContainer(const SServiceContainerInitializer& InInitializer);

	virtual IService* Register(const SServiceProps& InProps, IServiceFactory* pInFactory) override;
	virtual IResolvedService* Resolve(ServiceTypeNameConstRef InServiceTypeName) override;
	virtual IResolvedService* ResolveDependency(ServiceTypeNameConstRef InServiceTypeName, IResolvedService* pForService) override;
	virtual void Release(IResolvedService* pInService) override;

private:
	void _PurgeAllServiceObjects();
	void _FinishServiceResolution(ResolvedService* pService, void *pServiceObject);
	
	/**
	* Maps resolved service id to the resolved service
	*/
	using ResolvedServiceMap = TVector<ResolvedService*>;
	ResolvedServiceMap ResolvedServicesById;

	/**
	* Resolved services in the order of finished resolutions
	*/
	using FinishedResolvedServiceCont = TVector<ResolvedService*>;
	FinishedResolvedServiceCont FinishedResolvedServices;

	/**
	* Returns nullptr, if service is NOT registered
	*/
	Service* TryGetServiceByTypeName(ServiceTypeNameConstRef InServiceTypeName);

	using ServiceVector = TVector<Service>;
	ServiceVector Services;

	using ServiceMap = std::unordered_map<ServiceTypeName, int>;
	ServiceMap ServicesByServiceType;
};

} // Eng::Svc::IMPL
} // Eng::Svc
} // Eng