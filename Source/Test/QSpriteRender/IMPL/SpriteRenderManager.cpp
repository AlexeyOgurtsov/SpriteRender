#include "../SpriteRenderManager.h"
#include "ISpriteRender/ISpriteRender.h"
#include "QSpriteRender/INTERFACE/SpriteRenderSubsystemCreate.h"
#include "ISpriteRenderTestUtils/TesterConfig.h"
#include "ISpriteRenderTestUtils/IMPL/D3DDevice/D3DDevice.h"
#include "ISpriteRenderTestUtils/Utils/TestUtils.h"
#include "QSpriteRender/INTERFACE/Material/MaterialInstanceFactory.h"
#include "../CONFIG/SpriteRenderCustomSetuper.h"

namespace Test::ISpr::QSpr
{
	namespace SubsysInitializerUtils
	{
		namespace Ren = Dv::Spr::QRen;

		void Configure_RenderTargetAndDepthStencil(std::ofstream& InLog, Ren::SRenTargetConfig& OutConfig, const D3DDevice* pInDevice)
		{
			T_LOG_TO(InLog, "Configure_RenderTargetAndDepthStencil...");
			if (pInDevice)
			{
				OutConfig.RTWidth = pInDevice->GetRTWidth();
				OutConfig.RTHeight = pInDevice->GetRTHeight();
				T_LOG_TO(InLog, "Configure_RenderTargetAndDepthStencil: RTWidth=" << OutConfig.RTWidth << " RTHeight=" << OutConfig.RTHeight);
				OutConfig.pRenderTargetView = pInDevice->GetSwapChainBufferRTView();
				T_LOG_TO(InLog, "Configure_RenderTargetAndDepthStencil: pRenderTarget: " << (OutConfig.pRenderTargetView ? "Valid" : "nullptr"));
				OutConfig.pDepthStencilView = pInDevice->GetDepthStencilView();
				T_LOG_TO(InLog, "Configure_RenderTargetAndDepthStencil: pDepthStencilView: " << (OutConfig.pDepthStencilView ? "Valid" : "nullptr"));
			}
			else
			{
				T_LOG_TO(InLog, "pInDevice is nullptr, resetting values");
				OutConfig.RTWidth = 0;
				OutConfig.RTHeight = 0;
				OutConfig.pRenderTargetView = nullptr;
				OutConfig.pDepthStencilView = nullptr;
			}
			T_LOG_TO(InLog, "Configure_RenderTargetAndDepthStencil DONE");
		}

		void Configure_RenderDeviceConfig(std::ofstream& InLog, Ren::SRenderDeviceConfig& OutConfig, const D3DDevice* pInDevice)
		{
			T_LOG_TO(InLog, "Configure_RenderDeviceConfig...");
			if (pInDevice)
			{
				T_LOG_TO(InLog, "Configure_RenderDeviceConfig: D3DDevice valid");
				OutConfig.pDev = pInDevice->GetDev();
				OutConfig.pImmDevCon = pInDevice->GetDevCon();
			}
			else
			{
				T_LOG_TO(InLog, "Configure_RenderDeviceConfig: Resetting - D3D Device is nullptr");
				OutConfig.pDev = nullptr;
				OutConfig.pImmDevCon = nullptr;
			}
			T_LOG_TO(InLog, "Configure_RenderDeviceConfig DONE");
		}
	} // SubsysInitializerUtils

	ISpriteRender* SpriteRenderSubsystemManager::GetSpriteRender() const
	{
		if(pSubsys)
		{
			return pSubsys->GetRender();
		}

		return nullptr;
	}

	void SpriteRenderSubsystemManager::Initialize()
	{
		BOOST_ASSERT(pLog);
		T_LOG("SpriteRenderSubsystemManager::Initialize...");
		BOOST_ASSERT_MSG(nullptr == pSubsys.get(), "SpriteRenderSubsystemManager::Initialize: WARNING!!! Subsystem must be shutted down");
		pSubsys = Dv::Spr::QRen::CreateSpriteRenderSubsystem(SubsysInitializer);
		bSettingsUpdated = false;

		LogSubsystemState();

		T_LOG("SpriteRenderSubsystemManager::Initialize DONE");
	}

	void SpriteRenderSubsystemManager::LogSubsystemState()
	{
		T_LOG("SpriteRenderSubsystemManager::LogSubsystemState...");
		if (nullptr == GetSpriteRender())
		{
			T_LOG("SpriteRenderSubsystemManager::LogSubsystemState: SpriteRender is nullptr");
			return;
		}

		// WARNING!!! We should NOT log most properties of the sprite render here,
		// because it's already logged by the render itself.
		T_LOG("ScreenCoordSystem:");
		T_LOG(GetScreenCoordSystem());

		T_LOG("SpriteRenderSubsystemManager::LogSubsystemState DONE");
	}

	void SpriteRenderSubsystemManager::Shutdown()
	{
		BOOST_ASSERT(pLog);
		T_LOG("SpriteRenderSubsystemManager::Shutdown...");
		if(pSubsys)
		{
			pSubsys.reset();
		}
		bSettingsUpdated = true;
		T_LOG("SpriteRenderSubsystemManager::Shutdown DONE");
	}

	void SpriteRenderSubsystemManager::Tick(float InDeltaSeconds)
	{
		// Nothing is to do here yet
		// (if no tick function is the current sprite render manager subsystem)
	}

	void SpriteRenderSubsystemManager::BeginFrame(float InDeltaSeconds)
	{
		// Nothing is to do here yet
		// (if no tick function is the current sprite render manager subsystem)
	}

	void SpriteRenderSubsystemManager::EndFrame(float InDeltaSeconds)
	{
		// Nothing is to do here yet
		// (if no tick function is the current sprite render manager subsystem)
	}

	void SpriteRenderSubsystemManager::OnDefaultTesterConfigUpdated(const TesterConfig& InDefaultConfig)
	{
		BOOST_ASSERT(pLog);
		T_LOG("SpriteRenderSubsystemManager::OnDefaultTesterConfigUpdated...");
		// Nothing is to be done here yet
		bSettingsUpdated = true;
		T_LOG("SpriteRenderSubsystemManager::OnDefaultTesterConfigUpdated DONE");
	}

	void SpriteRenderSubsystemManager::OnD3DUpdated(const D3DDevice* pInDev)
	{
		BOOST_ASSERT(pLog);
		T_LOG("SpriteRenderSubsystemManager::OnD3DUpdated...");
		SubsysInitializerUtils::Configure_RenderDeviceConfig(GetLog(), SubsysInitializer.Render.Device, pInDev);
		SubsysInitializerUtils::Configure_RenderTargetAndDepthStencil(GetLog(), SubsysInitializer.Render.RenderTarget, pInDev);
		bSettingsUpdated = true;
		T_LOG("SpriteRenderSubsystemManager::OnD3DUpdated DONE");
	}

	void SpriteRenderSubsystemManager::OnViewportWindowChanged(HWND hWnd)
	{
		T_LOG("SpriteRenderSubsystemManager::OnViewportWindowChanged...");
		// Nothing is to be done when the hWnd is changed for this SpriteRender implementation.
		T_LOG("SpriteRenderSubsystemManager::OnViewportWindowChanged DONE");
	}

	void SpriteRenderSubsystemManager::SetLog(std::ofstream* pInLog)
	{		
		if (pLog)
		{
			T_LOG_TO(*pLog, "SpriteRenderSubsystemManager::SetLog: setting up a new log");
			if (nullptr == pInLog)
			{
				T_LOG_TO(*pLog, "SpriteRenderSubsystemManager::SetLog: new log is nullptr, switching to default");
				pLog = &DefaultLog;
			}
			else
			{
				pLog = pInLog;
			}
		}
		SubsysInitializer.Render.Log.pLog = pLog;
		bSettingsUpdated = true;
		if (pLog)
		{
			T_LOG_TO(*pLog, "SpriteRenderSubsystemManager::SetLog: This log will be used as a new log");
		}
	}

	void SpriteRenderSubsystemManager::OnCustomSetup(const SpriteRenderCustomSetuper& InSetuper)
	{
		BOOST_ASSERT(pLog);
		T_LOG("SpriteRenderSubsystemManager::OnCustomSetup...");
		InSetuper.SetupCustomSettings(SubsysInitializer);
		bSettingsUpdated = true;
		T_LOG("SpriteRenderSubsystemManager::OnCustomSetup END");
	}

	/**
	* WARNING!!! We decided to put the SpirteRenderCustomSetuper implementation
	* in this file (SpriteRenderManager.cpp) because often change together.
	*/
	void SpriteRenderCustomSetuper::SetupCustomSettings(Dv::Spr::QRen::SSpriteRenderSubsystemInitializer& OutInitializer) const
	{
		OutInitializer.Render.InitialSpriteBufferCapacity = InitialSpriteBufferCapacity;
		OutInitializer.Render.RenderTarget.ZFar = RenderTarget_ZFar;
		OutInitializer.Render.Shaders = ShadersConfig;
	}
	
	ScreenCoordSystemDesc SpriteRenderSubsystemManager::GetScreenCoordSystem() const
	{
		ScreenCoordSystemDesc Desc
		{
			/*Center*/Vec2{0.0F, 0.0F}, 
			/*HalfWidth*/1.0F, 
			/*HalfHeight*/1.0F 
		};
		return Desc;
	}
	
	ScreenCoordSystemDesc SpriteRenderSubsystemManager::GetCanvasCoordSystem(CanvasId InCanvasId) const
	{
		BOOST_ASSERT_MSG(GetSpriteRender(), "SpriteRenderSubsystemManager::GetCanvasCoordSystem: the sprite render must be running");
		BOOST_ASSERT(MySpr::IsValidCanvasId(InCanvasId));
		// @TODO: WARNING!!! This implementation only works if the entire screen is used as a canvas.
		T_LOG("WARNING!!! SpriteRenderSubsystemManager::GetCanvasCoordSystem: this implementation only works if the entire screen is used as a canvas!");
		return GetScreenCoordSystem();
	}

	DXGI_FORMAT SpriteRenderSubsystemManager::GetDefaultTextureFormat_Diffuse() const
	{
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	Handle_SprMaterialInstance SpriteRenderSubsystemManager::CreateMatInst_Default(const char* pInName, ID3D11ShaderResourceView* pInTexture2D) const
	{
		BOOST_ASSERT(pInName);
		T_LOG("SpriteRenderSubsystemManager::CreateMatInst_Default, Name=" << pInName << "...");
		BOOST_ASSERT(pInTexture2D);
		BOOST_ASSERT_MSG(pSubsys.get(), "SpriteRenderSubsystemManager::CreateMatInst_Default: sprite render subsystem must be created");

		Handle_SprMaterialInstance MatInstHandle = Dv::Spr::QRen::CreateMatInst_Default(pSubsys.get(), pInTexture2D);
		BOOST_ASSERT_MSG(MatInstHandle.get(), "SpriteRenderSubsystemManager::CreateMatInst_Default: returned material instance handle must be valid");

		T_LOG("SpriteRenderSubsystemManager::CreateMatInst_Default Returning");
		return MatInstHandle;
	}
} // Test::ISpr::QSpr