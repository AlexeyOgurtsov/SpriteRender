#include "Environment.h"
#include "ISpriteRenderTestUtils/IMPL/Utils/Window.h"
#include "ISpriteRenderTestUtils/IMPL/D3DDevice/D3DDevice.h"
#include "Utils/Exception.h"
#include "../Utils/TestUtils.h"

namespace Test::IMPL
{ 
	std::ofstream OpenLog(const char* pInLog)
	{
		std::ofstream log{ pInLog, std::ios::out };
		if (!log.is_open())
		{
			throw Exception("OpenLog failed");
		}
		return log;
	}

	void Environment::SetSpriteRenderManager(ISpriteRenderSubsystemManager* InManager)
	{
		BOOST_ASSERT(InManager);

		T_LOG_TO(MainLog, "Environment::SetSpriteRenderManager...");
		T_LOG_TO(MainLog, "ManagerType: " << InManager->GetName());
		if (pSpriteRenderManager.get())
		{
			T_LOG_TO(MainLog, "Old ManagerType: " << pSpriteRenderManager->GetName());
		}

		pSpriteRenderManager.reset(InManager);

		T_LOG_TO(MainLog, "Environment::SetSpriteRenderManager DONE");
	}

	void Environment::ReInit_SpriteRender()
	{
		T_LOG_TO(MainLog, "Environment::ReInit_SpriteRender...");

		BOOST_ASSERT_MSG(pSpriteRenderManager.get(), "Environment::ReInit_SpriteRender: Sprite render must be set");

		Shutdown_SpriteRender_IfInitialized();

		T_LOG_TO(MainLog, "Initializing sprite render instance...");
		pSpriteRenderManager->Initialize();
		T_LOG_TO(MainLog, "Initializing sprite render instance DONE");
		
		T_LOG_TO(MainLog, "Environment::ReInit_SpriteRender DONE");
	}

	void Environment::Shutdown_SpriteRender_IfInitialized()
	{
		if (pSpriteRenderManager->GetSpriteRender())
		{
			T_LOG_TO(MainLog, "SpriteRender instance exists, AreSettingsUpdated=" << pSpriteRenderManager->AreSettingsUpdated());

			T_LOG_TO(MainLog, "Shutting down old sprite render instance...");
			pSpriteRenderManager->Shutdown();
			T_LOG_TO(MainLog, "Shutting down old sprite render instance DONE");
		}
		BOOST_ASSERT_MSG(nullptr == pSpriteRenderManager->GetSpriteRender(), "Environment::ReInit_SpriteRender: sprite render must be nullptr at this point (tried to shutdown it already)");
	}

	void Environment::ReInit_D3DDevice(UINT InRTWidth, UINT InRTHeight, const TesterConfig_D3DDevice& InConfig)
	{
		T_LOG_TO(MainLog, "Environment::ReInit_D3DDevice...");

		BOOST_ASSERT_MSG(MainLog.is_open(), "Environment::ReInit_D3D11Device: main log must be opened");
		BOOST_ASSERT_MSG(hWndViewport, "Environment::ReInit_D3D11Device: hWndViewport window must be created");

		if (pD3DDevice.get())
		{
			T_LOG_TO(MainLog, "Environment::ReInit_D3DDevice: device instance already exists, will be shutted down");
			if (pSpriteRenderManager->GetSpriteRender())
			{
				T_LOG_TO(MainLog, "Environment::ReInit_D3DDevice: shutting existing SpriteRender instance");
				pSpriteRenderManager->Shutdown();
			}
		}

		pD3DDevice.reset
		(
			new D3DDevice(InRTWidth, InRTHeight, &GetMainLog(), hWndViewport, InConfig)
		);
		NotifySpriteRenderManager_D3DDeviceUpdated("Environment::ReInit_D3DDevice");

		T_LOG_TO(MainLog, "Environment::ReInit_D3DDevice DONE");
	}

	void Environment::NotifySpriteRenderManager_D3DDeviceUpdated(const char* pInReason)
	{
		if (pSpriteRenderManager)
		{
			T_LOG_TO(MainLog, "SpriteRenderManager notified that D3DDevice updated; Reason=" << pInReason);
			pSpriteRenderManager->OnD3DUpdated(pD3DDevice.get());
		}
	}

	void Environment::ReInitMainLog(const char* InLogName)
	{
		BOOST_ASSERT(InLogName);
		MainLog = OpenLog(InLogName);
		T_LOG_TO(MainLog, "Environment::ReInitMainLog: Main log initialized");
		if (pSpriteRenderManager.get())
		{
			T_LOG_TO(MainLog, "Environment::ReInitMainLog: Binding new log for SpriteRenderManager...");
			pSpriteRenderManager->SetLog(&MainLog);
			T_LOG_TO(MainLog, "Environment::ReInitMainLog: Binding new log for SpriteRenderManager DONE");
		}
	}

	void Environment::InitWindow(UINT InWidth, UINT InHeight)
	{
		BOOST_ASSERT_MSG(hWndViewport == nullptr, "Environment::InitWindow: Cannot initialize window twice");
		BOOST_ASSERT_MSG(InWidth <= 3000, "Environment::InitWindow: window width is too high");
		BOOST_ASSERT_MSG(InHeight <= 2500, "Environment::InitWindow: window height is too high");
		hWndViewport = RegisterClassAndCreateWindow(GetModuleHandle(NULL), "SpriteRenderTester_WINDOW_CLASS", StaticWndProc, "SpriteRenderTester", InWidth, InHeight);
		if (pSpriteRenderManager.get())
		{
			T_LOG_TO(MainLog, "Environment::InitWindow: Setting up new window for SpriteRenderManager...");
			pSpriteRenderManager->OnViewportWindowChanged(hWndViewport);
			T_LOG_TO(MainLog, "Environment::InitWindow: Setting up new window for SpriteRenderManager DONE");
		}
	}

	LRESULT Environment::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		/**
		* TODO
		*/
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	void Environment::BeginTesting(const char* pInReason)
	{
		BOOST_ASSERT(pInReason);
		T_LOG_TO(MainLog, "Environment::BeginTesting: Reason=" << pInReason);
		bReadyForTesting = true;
	}

	void Environment::EndTesting(const char* pInReason)
	{
		BOOST_ASSERT(pInReason);
		T_LOG_TO(MainLog, "Environment::EndTesting: Reason=" << pInReason);
		bReadyForTesting = false;
	}

	Environment::~Environment() = default;	

	void ResetEnvironment(UINT InResetFlags)
	{
		// @TODO: Log reset flags
		if (InResetFlags != 0)
		{
			T_LOG_TO(Environment::Get()->GetMainLog(), "ResetEnvironment: WARNING!!! Any reset flags are IGNORED (NOT YET IMPL)!");
		}
		// @TODO }

		// WARNING!!! We must shutdown the sprite render BEFORE destroying resources,
		// to eliminate all dependencies of the SpriteRender subsystem on this resources.
		Environment::Get()->Shutdown_SpriteRender_IfInitialized();
		// @TODO: Should we clear the resources (textures etc.)
		ClearAndPurgeDynamic(Environment::Get()->GetMainLog(), Environment::Get()->GetD3DDevice());
		Environment::Get()->ReInit_SpriteRender();
	}
} // Test::IMPL