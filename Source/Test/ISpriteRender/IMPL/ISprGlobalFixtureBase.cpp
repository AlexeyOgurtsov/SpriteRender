#pragma once

#include "../ISprGlobalFixtureBase.h"

namespace Test::ISpr
{
	ISprGlobFixtureBase::ISprGlobFixtureBase()
	{
	}

	ISprGlobFixtureBase::~ISprGlobFixtureBase()
	{
	}

	void ISprGlobFixtureBase::InitGlobalISpr(const TesterConfig& InDefaultConfig)
	{
		InitGlobal(InDefaultConfig);
	}
} // Test::ISpr