#pragma once

#include "ISprPerTestFixtureBase.h"

namespace Test::ISpr
{
	/**
	* This fixture performs minimal setup only, and should be used 
	* for very special tests.
	*/
	class ISprPerTestFixture_Custom : public ISprPerTestFixtureBase
	{
	public:
		ISprPerTestFixture_Custom();
		~ISprPerTestFixture_Custom();

	protected:
	private:
	};
} // Test::ISpr