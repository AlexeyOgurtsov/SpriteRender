#pragma once

#include "ISpriteRenderTestUtils/ISpriteRenderSubsystemManager.h"
#include "SpriteRender/INTERFACE/SpriteRenderSubsystemHandle.h"
#include "SpriteRender/INTERFACE/SpriteRenderSubsystemInitializer.h"
#include <fstream>

namespace Test::ISpr::Spr
{
	using SpriteRenderSubsystemHandle = Dv::Spr::Ren::SpriteRenderSubsystemHandle;

	class SpriteRenderSubsystemManager : public ISpriteRenderSubsystemManager
	{
	public:
		// ~ ISpriteRenderSubsystemManager Begin
		virtual const char* GetName() const override { return "SpriteRender"; }
		virtual ISpriteRender* GetSpriteRender() const override;
		virtual void Initialize() override;
		virtual void Shutdown() override;
		virtual bool AreSettingsUpdated() const override { return bSettingsUpdated; }
		virtual void OnDefaultTesterConfigUpdated(const TesterConfig& InDefautConfig) override;
		virtual void OnD3DUpdated(const D3DDevice* pInDev) override;
		virtual void OnViewportWindowChanged(HWND hWnd) override;
		virtual void SetLog(std::ofstream* pInLog) override;
		// ~ ISpriteRenderSubsystemManager End

		// ~ SpriteRenderSubsystem implementation-specific Begin
		void OnCustomSetup(const class SpriteRenderCustomSetuper& InSetuper);

		// @TODO: Access materials
	
		// ~ SpriteRenderSubsystem implementation-specific End

	private:
		SpriteRenderSubsystemHandle pSubsys;
		Dv::Spr::Ren::SSpriteRenderSubsystemInitializer SubsysInitializer;	
		bool bSettingsUpdated = true;
		std::ofstream& GetLog() const { return *pLog; }
		// ~ Logging begin
		// WARNING!!! We provided the default log, to make the T_LOG work correctly
		// ever when there was no SetLog call yet.
		std::ofstream DefaultLog;
		std::ofstream* pLog = &DefaultLog;
		// ~ Logging end
	};
} // Test::ISpr::Spr