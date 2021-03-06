#include "App.h"
#include "Util/ServiceContainer/ServiceContainerAccess.h"
#include "Util/ServiceContainer/ServiceContainerTypedInterface.h"
#include "AppDeployer.h"

// Services
#include "../HelperSubsys/Log/LogServiceRegister.h"
#include "../HelperSubsys/ConfigManager/ConfigManagerServiceRegister.h"
#include "../Render/QSpriteRender/QSpriteRenderServiceRegister.h"
#include "../D3DSubsys/D3DSubsysServiceRegister.h"

namespace Tet
{
	Eng::Svc::ServiceContainerHandle CreatedInitializedServiceContainer(HINSTANCE hInstance, LPSTR lpCmdLine);
	void DeploySubsystemFactories(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont);
	void DeployD3DSubsys(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont);
	//void DeploySpriteRender(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont);
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
		RegSvc_ConfigManager("ConfigManager", pInSvcCont);
		DeployD3DSubsys(hInstance, lpCmdLine, pInSvcCont);
		// @TODO:
		//DeploySpriteRender(hInstance, lpCmdLine, pInSvcCont);
	}

	void DeployD3DSubsys(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont)
	{
		D3DSubsysInitializer Initializer; // @TODO
		RegSvc_D3DSubsys(Initializer, pInSvcCont);
	}

	//void DeploySpriteRender(HINSTANCE hInstance, LPSTR lpCmdLine, Eng::Svc::IServiceContainer* pInSvcCont)
	//{
	//	QSpriteRenderSubsysInitializer Initializer; // @TODO
	//	RegSvc_QSpriteRender(Initializer, pInSvcCont);
	//}
} // Tet