#include "Environment.h"
#include "ISpriteRenderTestUtils/IMPL/Utils/Window.h"
#include "Utils/Exception.h"
#include "../Utils/TestUtils.h"

namespace Test::IMPL
{ 
	namespace
	{
		Environment* GetEnv() { return Environment::Get(); }
	} // anonymous

	std::ofstream OpenLog(const char* pInLog)
	{
		std::ofstream log{ pInLog, std::ios::out };
		if (!log.is_open())
		{
			throw Exception("OpenLog failed");
		}
		return log;
	}

	void Environment::BeginFrame_FixDeltaSeconds(std::ofstream& InLog)
	{
		BeginFrameQPC.Fix(InLog);
	}

	void Environment::BeginFrame_PauseDeltaCounter(std::ofstream& InLog)
	{
		if ( ! BeginFrameQPC.IsPaused() )
		{
			BeginFrameQPC.Pause(InLog);
		}
	}

	void Environment::BeginFrame_ResumeDeltaCounter(std::ofstream& InLog)	
	{
		if (BeginFrameQPC.IsPaused())
		{
			BeginFrameQPC.Resume(InLog);
		}
	}

	void Environment::BeginFrame_ResetDeltaCounter(std::ofstream& InLog)
	{
		BeginFrameQPC.Reset(InLog);
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

	void Environment::ReInit_D3DResources(const TesterConfig_Resources& InResources)
	{
		T_LOG_TO(MainLog, "Environment::ReInit_D3DResources...");

		BOOST_ASSERT_MSG(pD3DDevice.get(), "Environment::ReInit_D3DResources: the d3d device must be initialized!");
		Shutdown_D3DResources_IfInitialized();

		T_LOG_TO(MainLog, "ReInit_D3DResources...");
		pResources.reset(new Resources(&MainLog, pD3DDevice.get(), InResources));
		T_LOG_TO(MainLog, "ReInit_D3DResources DONE");

		T_LOG_TO(MainLog, "Environment::ReInit_D3DResources DONE");
	}

	void Environment::Shutdown_D3DResources_IfInitialized()
	{
		if(pResources)
		{
			T_LOG_TO(MainLog, "Environment: Shutting down resources...");
			pResources.reset();
			T_LOG_TO(MainLog, "Environment: Shutting down resources DONE");
		}
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
				T_LOG_TO(MainLog, "Environment::ReInit_D3DDevice: shutting existing SpriteRender instance  (dependent service)");
				pSpriteRenderManager->Shutdown();
			}
			if (pResources.get())
			{
				T_LOG_TO(MainLog, "Environment::ReInit_D3DDevice: shutting down resources (dependent service)");
				Shutdown_D3DResources_IfInitialized();
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

	void Environment::ResetWindow(UINT InWidth, UINT InHeight, bool bWindowAlreadyResized)
	{
		T_LOG_TO(MainLog, "Environment::ResetWindow...");
		T_LOG_TO(MainLog, "NewResolution: (width*height)=" << InWidth << "*" << InHeight);
		BOOST_ASSERT_MSG(hWndViewport == nullptr, "Environment::ResetWindow: Cannot initialize window twice");
		BOOST_ASSERT_MSG(InWidth <= 3000, "Environment::ResetWindow: window width is too high");
		BOOST_ASSERT_MSG(InHeight <= 2500, "Environment::ResetWindow: window height is too high");
		if (hWndViewport == nullptr)
		{
			T_LOG_TO(MainLog, "Creating new window..");
			hWndViewport = RegisterClassAndCreateWindow(GetModuleHandle(NULL), "SpriteRenderTester_WINDOW_CLASS", StaticWndProc, "SpriteRenderTester", InWidth, InHeight);
			T_LOG_TO(MainLog, "Creating new window DONE");
		}
		else
		{
			if (!bWindowAlreadyResized)
			{
				T_LOG_TO(MainLog, "Window already created, resizing");
				BOOL bResized = SetWindowPos(hWndViewport, nullptr, 0, 0, InWidth, InHeight,  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
				BOOST_ASSERT_MSG(bResized, "Environment::ResetWindow: SetWindowPos must succeed");
			}
		}		
		if (pSpriteRenderManager.get())
		{
			T_LOG_TO(MainLog, "Environment::InitWindow: Setting up new window for SpriteRenderManager...");
			pSpriteRenderManager->OnViewportWindowChanged(hWndViewport);
			T_LOG_TO(MainLog, "Environment::InitWindow: Setting up new window for SpriteRenderManager DONE");
		}

		if (pD3DDevice)
		{
			T_LOG_TO(MainLog, "Resetting resolution of D3D device...");
			pD3DDevice->ResetResolution(InWidth, InHeight);
			T_LOG_TO(MainLog, "Resetting resolution of D3D device DONE");
		}

		T_LOG_TO(MainLog, "Environment::ResetWindow DONE");
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
		ResetAllCounters();
	}

	void Environment::ResetAllCounters()
	{
		T_LOG_TO(MainLog, "Environment::ResetAllCounters...");
		BeginFrame_ResetDeltaCounter(MainLog);
		T_LOG_TO(MainLog, "Environment::ResetAllCounters DONE");
	}

	void Environment::EndTesting(const char* pInReason)
	{
		BOOST_ASSERT(pInReason);
		T_LOG_TO(MainLog, "Environment::EndTesting: Reason=" << pInReason);
		bReadyForTesting = false;
	}

	void Environment::ProcessWindowsMessages_AndSetupQuitFlag(std::ofstream& InLog)
	{
		T_LOG_TO(InLog, "Environment::ProcessWindowsMessages_AndSetupQuitFlag...");
		bMessagePoke = false;
		if (hWndViewport)
		{
			bMessagePoke = PeekMessage(&Msg, hWndViewport, 0, 0, PM_REMOVE);
			if (bMessagePoke)
			{
				T_LOG_TO(InLog, "bMessagePoke = TRUE");
				if (Msg.message == WM_QUIT)
				{
					T_LOG_TO(InLog, "WM_QUIT Message, setting up bIsQuitRequested flag");
					bIsQuitRequested = true;
				}
			}
		}
		T_LOG_TO(InLog, "Environment::ProcessWindowsMessages_AndSetupQuitFlag DONE");
	}

	Environment::~Environment() = default;	

	namespace {
		void TickEnvironmentTestResources(std::ofstream& InLog, float InDeltaSeconds)
		{
			if(Resources* pResources = GetEnv()->GetResources())
			{
				pResources->Tick(InLog, InDeltaSeconds);
			}
		}

		/**
		* Here we tick test resources only (but NOT primary resources like copy buffers).
		*/
		void ClearDynamicTestResources(std::ofstream& InLog)
		{
			// Nothing is to be done here
		}
	} // anonymous

	void ResetEnvironment(UINT InResetFlags)
	{		
		// @TODO: Log reset flags
		if (InResetFlags != 0)
		{
			T_LOG_TO(GetEnv()->GetMainLog(), "ResetEnvironment: WARNING!!! Any reset flags are IGNORED (NOT YET IMPL)!");
		}
		// @TODO }

		// WARNING!!! We must shutdown the sprite render BEFORE destroying resources,
		// to eliminate all dependencies of the SpriteRender subsystem on this resources.
		GetEnv()->Shutdown_SpriteRender_IfInitialized();

		ClearAndPurgeDynamic(GetEnv()->GetMainLog(), GetEnv()->GetD3DDevice());
		// Normally we do NOT destroy test resources, only clear them.
		ClearDynamicTestResources(GetEnv()->GetMainLog());

		GetEnv()->ReInit_SpriteRender();

		// We reset all counters as the last line
		GetEnv()->ResetAllCounters();
	}


	void TickEnvironment(std::ofstream& InLog, float InDeltaSeconds)
	{
		T_LOG_TO(InLog, "TickEnvironment, InDeltaSeconds=" << InDeltaSeconds << "...");

		GetEnv()->ProcessWindowsMessages_AndSetupQuitFlag(InLog);		

		if(D3DDevice* pD3D = GetEnv()->GetD3DDevice())
		{
			pD3D->Tick(InDeltaSeconds);
		}

		TickEnvironmentTestResources(InLog, InDeltaSeconds);

		// Tick sprite render
		if(ISpriteRenderSubsystemManager* pSprManager = GetEnv()->GetSpriteRenderManager())
		{
			T_LOG_TO(InLog, "Ticking SpriteRenderManagerSubsystem...");
			pSprManager->Tick(InDeltaSeconds);	
			T_LOG_TO(InLog, "Ticking SpriteRenderManagerSubsystem DONE");
		}

		T_LOG_TO(InLog, "TickEnvironment DONE");
	}

	void Environment_BeginFrame(int InLocalFrameIndex, std::ofstream& InLog)
	{
		T_LOG_TO(InLog, "Environment_BeginFrame...");

		const float DeltaSeconds = GetEnv()->GetDeltaSeconds_SinceLastBeginFrame();

		GetEnv()->GetD3DDevice()->BeginFrame(InLocalFrameIndex, InLog);
		if(ISpriteRenderSubsystemManager* pSprManager = GetEnv()->GetSpriteRenderManager())
		{
			T_LOG_TO(InLog, "Calling BeginFrame on SpriteRenderManagerSubsystem...");
			pSprManager->BeginFrame(DeltaSeconds);
		}

		if(Resources* pResources = GetEnv()->GetResources())
		{
			pResources->BeginFrame(InLog, DeltaSeconds);
		}

		T_LOG_TO(InLog, "Environment_BeginFrame DONE");
	}

	void Environment_EndFrame(int InLocalFrameIndex, std::ofstream& InLog)
	{
		T_LOG_TO(InLog, "Environment_EndFrame...");

		const float DeltaSeconds = GetEnv()->GetDeltaSeconds_SinceLastBeginFrame();

		if (ISpriteRenderSubsystemManager* pSprManager = GetEnv()->GetSpriteRenderManager())
		{
			T_LOG_TO(InLog, "Calling EndFrame on SpriteRenderManagerSubsystem...");
			pSprManager->EndFrame(DeltaSeconds);
		}

		GetEnv()->GetD3DDevice()->EndFrame(InLocalFrameIndex, InLog);

		if(Resources* pResources = GetEnv()->GetResources())
		{
			pResources->EndFrame(InLog);
		}

		T_LOG_TO(InLog, "Environment_EndFrame DONE");
	}
} // Test::IMPL