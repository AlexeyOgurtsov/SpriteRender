#include "ISpriteRenderTestUtils/Fixture/FixtureBase.h"
#include "ISpriteRenderTestUtils/IMPL/Environment.h"
#include <optional>
#include <boost/assert.hpp>

namespace Test
{
	namespace IMPL
	{
		class Environment;
	} // IMPL

	/**
	* Global tester config variable.
	*/
	std::optional<TesterConfig> gTesterDefaultConfig;

	namespace {
		/**
		* Helper to get the environment within the FixtureBase
		*/
		IMPL::Environment* GetEnv() { return IMPL::Environment::Get(); }
	}

	UINT FixtureBase::GetRTWidth() const
	{
		return GetEnv()->GetD3DDevice()->GetRTWidth();
	}

	UINT FixtureBase::GetRTHeight() const
	{
		return GetEnv()->GetD3DDevice()->GetRTHeight();
	}

	bool FixtureBase::IsGloballyReadyForTesting() const
	{
		return GetEnv()->IsReadyForTesting();
	}

	std::ofstream& FixtureBase::GetLog() const
	{
		return GetEnv()->GetMainLog();
	}

	FixtureBase::FixtureBase()
	{
		// @TODO
	}

	FixtureBase::~FixtureBase()
	{
		// @TODO
	}

	const TesterConfig& FixtureBase::GetDefaultConfig()const
	{
		BOOST_ASSERT_MSG(IsDefaultConfigSet(), "FixtureBase::GetDefaultConfig: default config is NOT set yet");
		return gTesterDefaultConfig.value();
	}

	bool FixtureBase::IsDefaultConfigSet() const
	{
		return gTesterDefaultConfig.has_value();
	}

	void FixtureBase::ResetDefaultConfig(const TesterConfig& InConfig)
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ResetDefaultConfig...");
		gTesterDefaultConfig = InConfig;		
		if (GetEnv()->GetSpriteRenderManager())
		{
			T_LOG("ISpriteRenderTestUtils: FixtureBase::ResetDefaultConfig: Sprite render manager exists, notifying it that default config is updated");
			GetEnv()->GetSpriteRenderManager()->OnDefaultTesterConfigUpdated(InConfig);
			GetEnv()->NotifySpriteRenderManager_D3DDeviceUpdated(/*Reason:*/"FixtureBase::ResetDefaultConfig");
		}
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ResetDefaultConfig DONE");
	}

	void FixtureBase::InitMinimalCore()
	{
		// WARNING!!! Logging should NOT be performed here, because the log is NOT yet initialized!!!
		std::string MainLogFilename = GetMainLogFullFilename(GetDefaultConfig().Log);
		GetEnv()->ReInitMainLog(MainLogFilename.c_str());
	}

	void FixtureBase::ReInitViewport()
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitViewport...");
		BOOST_ASSERT_MSG(nullptr == GetEnv()->GetWnd(), "FixtureBase::ReInitViewport: viewport window RE initialization is NOT yet implemented");
		UINT NewWidth = GetDefaultConfig().Viewport.Width;
		UINT NewHeight = GetDefaultConfig().Viewport.Height;
		T_LOG("ISpriteRenderTestUtils: NewWidth = " << NewWidth << "; NewHeight=" << NewHeight);
		GetEnv()->InitWindow(NewWidth, NewHeight);
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitViewport DONE");
	}

	void FixtureBase::ReInitD3DDevice()
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitD3DDevice...");
		GetEnv()->ReInit_D3DDevice(GetDefaultConfig().Viewport.Width, GetDefaultConfig().Viewport.Height, GetDefaultConfig().D3D);
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitD3DDevice DONE");
	}

	void FixtureBase::ReInitResources()
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitResources...");
		GetEnv()->ReInit_D3DResources(GetDefaultConfig().Resources);
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInitResources DONE");
	}

	void FixtureBase::ReInit_SpriteRender()
	{
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInit_SpriteRender...");
		GetEnv()->ReInit_SpriteRender();
		T_LOG("ISpriteRenderTestUtils: FixtureBase::ReInit_SpriteRender DONE");
	}
} // Test