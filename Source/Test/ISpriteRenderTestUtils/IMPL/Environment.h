#pragma once

#pragma once

#include <windows.h>
#include <fstream>
#include <memory>
#include "D3DDevice/D3DDevice.h"
#include "../ISpriteRenderSubsystemManager.h"

namespace Test
{
	class ISpriteRenderSubsystemManager;
	struct TesterConfig_D3DDevice;
} // Test

namespace Test::IMPL
{
	class D3DDevice;

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

		void ReInit_D3DDevice(UINT InRTWidth, UINT InRTHeight, const TesterConfig_D3DDevice& InConfig);
		D3DDevice* GetD3DDevice() const { return pD3DDevice.get(); }

		void ReInitMainLog(const char* InLogName);
		void InitWindow(UINT InWidth, UINT InHeight);

		HWND GetWnd() const { return hWndViewport; }
		std::ofstream& GetMainLog() const { return MainLog; }

		static Environment* Get() 
		{
			static Environment Obj;
			return &Obj;
		}

	private:
		std::unique_ptr<ISpriteRenderSubsystemManager> pSpriteRenderManager;
		std::unique_ptr<D3DDevice> pD3DDevice;

		static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HWND hWndViewport = nullptr;
		mutable std::ofstream MainLog;
		bool bReadyForTesting = false;
	};
	/**
	* Resets entire environment or some of its components, dependent on the flags.
	*/
	void ResetEnvironment(UINT InResetFlags);
} // Test::IMPL