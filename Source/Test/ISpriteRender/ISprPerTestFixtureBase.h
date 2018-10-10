#pragma once

#include "Test/ISpriteRenderTestUtils/Fixture/PerTestFixtureBase.h"

namespace Test::ISpr
{
	class ISprPerTestFixtureBase : public PerTestFixtureBase
	{
	public:
		ISprPerTestFixtureBase();
		~ISprPerTestFixtureBase();

		void SetupTest(const char *InTestName, UINT InResetFlags = 0);
		
		// @TODO: Return check as result
		void CommitFrame();

	protected:

	private:		
	};
} // Test::ISpr