#pragma once

#include "ISpriteRenderFwd.h"
#include <fstream>
#include <windows.h> // HWND

namespace Test
{
	struct TesterConfig;

	namespace IMPL
	{
		class D3DDevice;
	} // IMPL
	using D3DDevice = IMPL::D3DDevice;

	/**
	* Abstract interface to the underlying sprite render subsystem.
	*/
	class ISpriteRenderSubsystemManager
	{
	public:
		virtual ~ISpriteRenderSubsystemManager() = default;

		/**
		* Name of the manager.
		*/
		virtual const char* GetName() const = 0;

		/**
		* Gets the deployed sprite render interface (or nullptr if NOT yet deployed).
		*/
		virtual ISpriteRender* GetSpriteRender() const = 0;

		/**
		* Initializes the subsystem with current settings
		* (WARNING!!! The subsystem must be shutted down already).
		*/
		virtual void Initialize() = 0;

		/**
		* Shutdown the subsystem completely
		*/
		virtual void Shutdown() = 0;

		/**
		* Tick function (To be called each frame).
		*/
		virtual void Tick(float InDeltaSeconds) = 0;

		/**
		* Returns true if any settings of the sprite render are updated,
		* but the current sprite render instance does NOT account the new settings
		* (does NOT make sense if there's no existing sprite render instance at the moment).
		*/
		virtual bool AreSettingsUpdated() const = 0;

		/**
		* To be called each time DefaultTesterConfig is updated
		* To be called from the Fixtures automatically.
		*/
		virtual void OnDefaultTesterConfigUpdated(const TesterConfig& InDefautConfig) = 0;

		/**
		* To be called each time D3DDevice or any of its COM objects (render target etc.) created or destroyed.
		*
		* WARNING!!! The device pointer may be passed as nullptr.
		*/
		virtual void OnD3DUpdated(const D3DDevice* pInDev) = 0;

		/**
		* To be called each time the viewport window is changed.
		*/
		virtual void OnViewportWindowChanged(HWND hWnd) = 0;
		
		/**
		* To be called each time the new log is to be used.
		*/
		virtual void SetLog(std::ofstream* pInLog) = 0;
	};
} // Test