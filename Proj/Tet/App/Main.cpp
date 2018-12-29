#include "App.h"
#include "Util/ServiceContainer/ServiceContainerAccess.h"
#include "Util/ServiceContainer/ServiceContainerTypedInterface.h"
#include "../HelperSubsys/Log/LogServiceRegister.h"
#include "../HelperSubsys/Log/LogServiceTypeTraits.h"

namespace Tet
{
	Eng::Svc::ServiceContainerHandle CreatedInitializedServiceContainer(HINSTANCE hInstance, LPSTR lpCmdLine);
	void DeploySubsystemFactories(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont);
	Tet::AppEnvironment DeployEnvironment(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont);
	Tet::AmbientContext DeployAmbientEnvironment(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont);
} // Tet

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Eng::Svc::ServiceContainerHandle SvcCont = Tet::CreatedInitializedServiceContainer(hInstance, lpCmdLine);
	Tet::DeploySubsystemFactories(hInstance, lpCmdLine, SvcCont.get());

	Tet::AppEnvironment Env = Tet::DeployEnvironment(hInstance, lpCmdLine, SvcCont.get());

	Tet::App::Get().Run(lpCmdLine, Env);
	return 0;
}

namespace Tet
{
	Eng::Svc::ServiceContainerHandle CreatedInitializedServiceContainer(HINSTANCE hInstance, LPSTR lpCmdLine)
	{
		Eng::Svc::SServiceContainerInitializer Initializer{};
		Eng::Svc::ServiceContainerHandle Svc = Eng::Svc::CreateServiceContainer(Initializer);
		return Svc;
	}

	void DeploySubsystemFactories(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont)
	{
		RegSvc_Log(std::string("C:\\SpriteRender\\Out\\Log\\Tet\\TetLog.txt"), pInSvcCont);
	}

	Tet::AppEnvironment DeployEnvironment(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont)
	{
		Tet::AppEnvironment Env;
		Env.SetAmbient(DeployAmbientEnvironment(hInstance, lpCmdLine, pInSvcCont));
		return Env;
	}

	Tet::AmbientContext DeployAmbientEnvironment(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont)
	{
		Eng::Svc::TResolvedService<Tet::Log> Log = Eng::Svc::Resolve(pInSvcCont, Tet::LogServiceInstanceType{});

		Tet::AmbientContext Ambient { Log.GetServiceObject() };
		return Ambient;
	}
} // Tet