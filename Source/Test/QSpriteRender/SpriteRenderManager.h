#pragma once

#include "ISpriteRenderTestUtils/ISpriteRenderSubsystemManager.h"
#include "QSpriteRender/INTERFACE/SpriteRenderSubsystemHandle.h"
#include "QSpriteRender/INTERFACE/SpriteRenderSubsystemInitializer.h"
#include <fstream>

namespace Test::ISpr::QSpr
{
	using SpriteRenderSubsystemHandle = Dv::Spr::QRen::SpriteRenderSubsystemHandle;

	class SpriteRenderSubsystemManager : public ISpriteRenderSubsystemManager
	{
	public:
		// ~ ISpriteRenderSubsystemManager Begin
		virtual const char* GetName() const override { return "SpriteRender"; }
		virtual ISpriteRender* GetSpriteRender() const override;
		virtual void Initialize() override;
		virtual void Shutdown() override;
		virtual void Tick(float InDeltaSeconds) override;
		virtual void BeginFrame(float InDeltaSeconds) override;
		virtual void EndFrame(float InDeltaSeconds) override;
		virtual bool AreSettingsUpdated() const override { return bSettingsUpdated; }
		virtual void OnDefaultTesterConfigUpdated(const TesterConfig& InDefautConfig) override;
		virtual void OnD3DUpdated(const D3DDevice* pInDev) override;
		virtual void OnViewportWindowChanged(HWND hWnd) override;
		virtual void SetLog(std::ofstream* pInLog) override;
		virtual std::ofstream& GetLog() const override { return *pLog; }
		// ~ ISpriteRenderSubsystemManager End

		// ~ SpriteRenderSubsystem implementation-specific Begin
		void OnCustomSetup(const class SpriteRenderCustomSetuper& InSetuper);

		// ~Info getters Begin
		/**
		* Description of coordinate system of the entire screen.
		*/
		virtual ScreenCoordSystemDesc GetScreenCoordSystem() const override;
		/**
		* Description of the coordinate system of the given canvas.
		*/
		virtual ScreenCoordSystemDesc GetCanvasCoordSystem(CanvasId InCanvasId) const override;

		virtual DXGI_FORMAT GetDefaultTextureFormat_Diffuse() const override;
		// ~Info getters End

		// ~ ISpriteRenderSubsystemManager Materials Begin
		virtual Handle_SprMaterialInstance CreateMatInst_Default(const char* pInName, ID3D11ShaderResourceView* pInTexture2D) const override;
		// ~ ISpriteRenderSubsystemManager Materials End
	
		// ~ SpriteRenderSubsystem implementation-specific End

	private:
		void LogSubsystemState();

		SpriteRenderSubsystemHandle pSubsys;
		Dv::Spr::QRen::SSpriteRenderSubsystemInitializer SubsysInitializer;	
		bool bSettingsUpdated = true;
		// ~ Logging begin
		// WARNING!!! We provided the default log, to make the T_LOG work correctly
		// ever when there was no SetLog call yet.
		std::ofstream DefaultLog;
		std::ofstream* pLog = &DefaultLog;
		// ~ Logging end
	};
} // Test::ISpr::QSpr