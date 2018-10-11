#include "../DefaultGlobalConfig.h"
#include "ISpriteRenderTestUtils/TesterConfig.h"
#include "../SpriteRenderCustomSetuper.h"

namespace Test::ISpr::Spr
{
	namespace
	{
		const std::string ProjectBaseDir = "C:\\SpriteRender";
		const std::string TestLogPathRelativeProjectBase = "Out\\Test\\SpriteRender";
		const std::string TestShadersDirectoryRelativeLogBase = "shaders";
		namespace Ren = Dv::Spr::Ren;
	} // anonymous

	void InitDefaultGlobalConfig_Tester(TesterConfig_Tester& OutDefaultConfig);
	void InitDefaultGlobalConfig_Log(TesterConfig_Logging& OutDefaultConfig);
	void InitDefaultGlobalConfig_Viewport(TesterConfig_Viewport& OutDefaultConfig);
	void InitDefaultGlobalConfig_D3D(TesterConfig_D3DDevice& OutDefaultConfig);

	void InitDefaultCustomConfig_ShadersConfig(SpriteRenderCustomSetuper& OutDefaultConfig, const TesterConfig& InDefaultConfig);
	void InitDefaultCustomConfig_RenderCachingConfig(SpriteRenderCustomSetuper& OutDefaultConfig, const TesterConfig& InDefaultConfig);

	void InitDefaultCustomConfig(SpriteRenderCustomSetuper& OutDefaultConfig, const TesterConfig& InDefaultConfig)
	{
		OutDefaultConfig.SetInitialSpriteBufferCapacity(100);
		OutDefaultConfig.SetZFar(1.0F);
		InitDefaultCustomConfig_ShadersConfig(OutDefaultConfig, InDefaultConfig);
		InitDefaultCustomConfig_RenderCachingConfig(OutDefaultConfig, InDefaultConfig);
	}

	void InitDefaultCustomConfig_ShadersConfig(SpriteRenderCustomSetuper& OutDefaultConfig, const TesterConfig& InDefaultConfig)
	{
		// WARNING!!! WARNING!!! WARNING!!! Never set _DEBUG and SKIP_OPTIMIZATION flags together:
		// It will bring the "Function call discards pixel" bug.
		constexpr UINT SHADER_COMPILE_FLAGS = D3D10_SHADER_DEBUG;

		Ren::SShadersConfig Config;
		Config.CompileFlags = SHADER_COMPILE_FLAGS;
		Config.OutputDirectory = InDefaultConfig.Log.BaseLogPath + std::string("\\") + TestShadersDirectoryRelativeLogBase;
		OutDefaultConfig.SetShadersConfig(Config);
	}

	void InitDefaultCustomConfig_RenderCachingConfig(SpriteRenderCustomSetuper& OutDefaultConfig, const TesterConfig& InDefaultConfig)
	{
		Ren::SRenderCachingConfig Config;
		Config.bUseCommandListToCacheRendering = false;
		OutDefaultConfig.SetRenderCachingConfig(Config);
	}

	void InitDefaultGlobalConfig(TesterConfig& OutDefaultConfig)
	{
		InitDefaultGlobalConfig_Tester(OutDefaultConfig.Tester);
		InitDefaultGlobalConfig_Log(OutDefaultConfig.Log);
		InitDefaultGlobalConfig_Viewport(OutDefaultConfig.Viewport);
		InitDefaultGlobalConfig_D3D(OutDefaultConfig.D3D);
	}

	void InitDefaultGlobalConfig_Tester(TesterConfig_Tester& OutDefaultConfig)
	{
		OutDefaultConfig.bShowMessageBeforeTest = true;
		OutDefaultConfig.DelaySeconds = 15;
		OutDefaultConfig.Presentation = ETestPresenation::Delay;
	}

	void InitDefaultGlobalConfig_Log(TesterConfig_Logging& OutDefaultConfig)
	{
		OutDefaultConfig.BaseLogPath = ProjectBaseDir + std::string("\\") + TestLogPathRelativeProjectBase;
		OutDefaultConfig.MainLogName = std::string("SpriteRenderTest_MainLog.txt");
		OutDefaultConfig.bLoggingEnabled = true;
	}

	void InitDefaultGlobalConfig_Viewport(TesterConfig_Viewport& OutDefaultConfig)
	{
		OutDefaultConfig.Width = 1024;
		OutDefaultConfig.Height = 768;
	}

	void InitDefaultGlobalConfig_D3D(TesterConfig_D3DDevice& OutDefaultConfig)
	{
		OutDefaultConfig.RenderTarget.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		/**
		* Black with Alpha = 1.0F
		*/
		OutDefaultConfig.RenderTarget.ClearColor[0] = 0.0F;
		OutDefaultConfig.RenderTarget.ClearColor[1] = 0.0F;
		OutDefaultConfig.RenderTarget.ClearColor[2] = 0.0F;
		OutDefaultConfig.RenderTarget.ClearColor[3] = 1.0F;

		OutDefaultConfig.DepthStencil.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		OutDefaultConfig.DepthStencil.ClearZ = 1.0F;
		OutDefaultConfig.DepthStencil.ClearStencil = 0;

		/**
		* Copy buffers:
		*/
		OutDefaultConfig.CopyBuffers.bEnableRT = true;
		OutDefaultConfig.CopyBuffers.bEnableDepthStencil = true;
	}
} // Test::ISpr::Spr