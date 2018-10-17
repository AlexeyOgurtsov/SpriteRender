#pragma once

#include <windows.h>
#include <fstream>
#include <memory>
#include <chrono>
#include "D3DDevice/D3DDevice.h"
#include "Resources/Resources.h"
#include "../ISpriteRenderSubsystemManager.h"
#include "Utils/QPC.h"

namespace Test
{
	class ISpriteRenderSubsystemManager;
	struct TesterConfig_D3DDevice;
	struct TesterConfig_Resources;
} // Test

namespace Test::IMPL
{
	/**
	* Environment.
	*/
	class Environment
	{
	public:
		/**
		* Always must be default-constructible.
		*/
		Environment() = default;
		~Environment();

		/**
		* Get delta seconds since the last frame
		*/
		float GetDeltaSeconds_SinceLastBeginFrame() const { return BeginFrameQPC.GetDeltaSeconds(); }
		const MSG& GetMsg() const { return Msg; }
		bool IsMessagePoke() const { return bMessagePoke; }
		void ProcessWindowsMessages_AndSetupQuitFlag(std::ofstream& InLog);

		void ResetAllCounters();

		void BeginFrame_FixDeltaSeconds(std::ofstream& InLog);
		void BeginFrame_PauseDeltaCounter(std::ofstream& InLog);
		void BeginFrame_ResumeDeltaCounter(std::ofstream& InLog);
		void BeginFrame_ResetDeltaCounter(std::ofstream& InLog);		

		/**
		* Returns true if application environment is ready to run BOOST unit tests.
		*/
		bool IsReadyForTesting() const { return bReadyForTesting; }
		
		/**
		* Marks that tests can be executed.
		* (Typically to be called from global fixture).
		*/
		void BeginTesting(const char* pInReason);

		/**
		* Marks that tests cannot be executed.
		* (Typically to be called from global fixture).
		*/
		void EndTesting(const char* pInReason);

		void SetSpriteRenderManager(ISpriteRenderSubsystemManager* InManager);

		ISpriteRenderSubsystemManager* GetSpriteRenderManager() const { return pSpriteRenderManager.get(); }
		void ReInit_SpriteRender();
		/**
		* Shutdowns sprite render. 
		* If no sprite render instance now exists, then just does nothing.
		*
		* After the shutdown the sprite render subsystem is no longer 
		* dependent on any of the environment resources.
		*/
		void Shutdown_SpriteRender_IfInitialized();
		void NotifySpriteRenderManager_D3DDeviceUpdated(const char* pInReason);

		void ReInit_D3DResources(const TesterConfig_Resources& InResources);	
		void Shutdown_D3DResources_IfInitialized();	
		Textures* GetTextures() const;
		Resources* GetResources() const { return pResources.get(); }

		void ReInit_D3DDevice(UINT InRTWidth, UINT InRTHeight, const TesterConfig_D3DDevice& InConfig);
		D3DDevice* GetD3DDevice() const { return pD3DDevice.get(); }

		void ReInitMainLog(const char* InLogName);
		void ResetWindow(UINT InWidth, UINT InHeight, bool bWindowAlreadyResized = false);

		HWND GetWnd() const { return hWndViewport; }
		std::ofstream& GetMainLog() const { return MainLog; }

		bool IsQuitRequested() const { return bIsQuitRequested; }

		static Environment* Get() 
		{
			static Environment Obj;
			return &Obj;
		}

	private:
		std::unique_ptr<Resources> pResources;
		std::unique_ptr<D3DDevice> pD3DDevice;
		std::unique_ptr<ISpriteRenderSubsystemManager> pSpriteRenderManager;

		static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HWND hWndViewport = nullptr;
		mutable std::ofstream MainLog;
		bool bReadyForTesting = false;
		bool bIsQuitRequested = false;
		MSG Msg;
		BOOL bMessagePoke = FALSE;
		/**
		* Query performance counter that counts time since BeginFrame.
		*/
		QPC BeginFrameQPC;
	};
	/**
	* Resets entire environment or some of its components, dependent on the flags.
	*/
	void ResetEnvironment(UINT InResetFlags);

	/**
	* Calls all Tick functions of the environment.
	*
	* WARNING!!! After this call the IsQuitRequested() state may change.
	*/
	void TickEnvironment(std::ofstream& InLog, float InDeltaSeconds);

	/**
	* Starts frame in all subsystems.
	*/
	void Environment_BeginFrame(int InLocalFrameIndex, std::ofstream& InLog);

	/**
	* Ends frame in all subsystems.
	*/
	void Environment_EndFrame(int InLocalFrameIndex, std::ofstream& InLog);
} // Test::IMPL