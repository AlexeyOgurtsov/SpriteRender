#pragma once

#include "ISprPerTestFixture_Custom.h"
#include "ISprFixture_FeaturePick.h"

namespace Test::ISpr
{
	/**
	* Per-test fixture for custom pick tests.
	*/
	class ISprPerTestFixture_CustomPick : 
		public ISprPerTestFixture_Custom
	{
	public:
		ISprPerTestFixture_CustomPick();
		~ISprPerTestFixture_CustomPick();

	protected:

	private:
	};
} // Test::ISpr