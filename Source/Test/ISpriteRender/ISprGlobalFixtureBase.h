#pragma once

#include "Test/ISpriteRenderTestUtils/Fixture/GlobFixtureBase.h"

/**
* Global fixture base class for fixtures of all ISpriteRender implementation testers.
*/

namespace Test::ISpr
{
	class ISprGlobFixtureBase : public GlobFixtureBase
	{
	public:
		ISprGlobFixtureBase();
		~ISprGlobFixtureBase();

	protected:
		void InitGlobalISpr(const TesterConfig& InDefaultConfig);

	private:
	};
} // Test::ISpr