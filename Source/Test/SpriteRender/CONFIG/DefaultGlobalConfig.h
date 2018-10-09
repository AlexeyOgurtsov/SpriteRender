#pragma once


namespace Test
{
	struct TesterConfig;
} // Test

namespace Test::ISpr::Spr
{
	class SpriteRenderCustomSetuper;

	/**
	* Initializes some or all parameters with default values for all SpriteRender tester modules.
	*/
	void InitDefaultGlobalConfig(TesterConfig& OutDefaultConfig);
	void InitDefaultCustomConfig(SpriteRenderCustomSetuper& OutDefaultConfig, const TesterConfig& InDefaultConfig);
} // Test::ISpr::Spr