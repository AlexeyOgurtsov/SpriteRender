#pragma once

#include <boost/test/included/unit_test.hpp>
#include "GlobFixture.h"

// WARNING!!! WARNING!! WARNING!!!
// Here we setup global fixture BEFORE including any other test headers!!!
namespace Test::ISpr::Spr
{
	BOOST_GLOBAL_FIXTURE(GlobFixture);
}

#include "ISpriteRender/AllTests.h"


namespace Test::ISpr::Spr
{
	// This set of tests is only presented to check that the tester itself works
	BOOST_AUTO_TEST_SUITE(SuiteMetaTest)
	BOOST_FIXTURE_TEST_CASE(MetaTestSmoke, ISprPerTestFixture_Custom)
	{
		SetupTest("SmokeMetaTest");
	}
	BOOST_FIXTURE_TEST_CASE(MetaTestSmoke2, ISprPerTestFixture_Custom)
	{
		SetupTest("SmokeMetaTest2");
	}
	BOOST_AUTO_TEST_SUITE_END() // SuiteMetaTest
} // Test::ISpr::Spr