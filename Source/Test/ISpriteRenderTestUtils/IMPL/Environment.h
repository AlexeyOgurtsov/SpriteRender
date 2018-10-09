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

		void SetSpriteRenderManager(ISpriteRenderSubsystemManager* InManager);

		ISpriteRenderSubsystemManager* GetSpriteRenderManager() const { return pSpriteRenderManager.get(); }
		void ReInit_SpriteRender();
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
	};
} // Test::IMPL