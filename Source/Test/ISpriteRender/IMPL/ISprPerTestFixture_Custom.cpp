#pragma once

#include "../ISprPerTestFixture_Custom.h"

namespace Test::ISpr
{
	ISprPerTestFixture_Custom::ISprPerTestFixture_Custom()
	{
		T_LOG("Custom per test fixture");
	}

	ISprPerTestFixture_Custom::~ISprPerTestFixture_Custom()
	{
	}
} // Test::ISpr