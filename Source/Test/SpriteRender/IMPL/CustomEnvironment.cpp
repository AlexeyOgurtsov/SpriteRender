#pragma once

#include "../CustomEnvironment.h"
#include "../CONFIG/SpriteRenderCustomSetuper.h"
#include "ISpriteRenderTestUtils/IMPL/Environment.h"
#include "ISpriteRenderTestUtils/Utils/TestUtils.h"
#include "../SpriteRenderManager.h"

namespace Test::ISpr::Spr::IMPL
{
	namespace
	{
		Test::IMPL::Environment* GetEnv() { return Test::IMPL::Environment::Get(); }
		std::ofstream& GetMainLog() { return GetEnv()->GetMainLog(); }
		SpriteRenderSubsystemManager* GetSpriteRenderManager() { return dynamic_cast<SpriteRenderSubsystemManager*>(GetEnv()->GetSpriteRenderManager()); }
	}

	void CustomEnvironment::ResetDefaultCustomSettings(const SpriteRenderCustomSetuper& InSetuper)
	{
		T_LOG_TO(GetMainLog(), "CustomEnvironment::ResetDefaultCustomSettings...");
		if(GetSpriteRenderManager())
		{
			T_LOG_TO(GetMainLog(), "CustomEnvironment::ResetDefaultCustomSettings: notifying sprite render manager");
			GetSpriteRenderManager()->OnCustomSetup(InSetuper);
		}
		RenCustomSetuper = InSetuper;
		T_LOG_TO(GetMainLog(), "CustomEnvironment::ResetDefaultCustomSettings END");
	}
} // Test::ISpr::Spr::IMPL