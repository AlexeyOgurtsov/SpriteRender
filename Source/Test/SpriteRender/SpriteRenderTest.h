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
} // Test::ISpr::Spr