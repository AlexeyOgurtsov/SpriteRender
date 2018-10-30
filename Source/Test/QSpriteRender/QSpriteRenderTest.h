#pragma once

#include <boost/test/included/unit_test.hpp>
#include "QGlobFixture.h"

// WARNING!!! WARNING!! WARNING!!!
// Here we setup global fixture BEFORE including any other test headers!!!
namespace Test::ISpr::QSpr
{
	BOOST_GLOBAL_FIXTURE(QGlobFixture);
}

// ~ Test control macros Begin
// Our render supports many canvasses simultaneosly
#define TEST_FEATURE_MANY_CANVASSES
// ~ Test control macros End

#include "IMPL_TEST/ImplTests.h"
#include "ISpriteRender/AllTests.h"


namespace Test::ISpr::QSpr
{
} // Test::ISpr::QSpr