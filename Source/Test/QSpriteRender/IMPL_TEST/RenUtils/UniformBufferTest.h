#pragma once

#include "../ImplTestHelper.h"
#include <boost/test/included/unit_test.hpp>

#include "../ImplD3DFixture.h"
#include "QSpriteRender/IMPL/RenUtils/UniformBuffer.h"

namespace Test
{
namespace ISpr
{
namespace QSpr
{
namespace IMPL
{
	BOOST_AUTO_TEST_SUITE(TestSuite_QRen)
	BOOST_AUTO_TEST_SUITE(TestSuite_Impl)
	BOOST_FIXTURE_TEST_SUITE(TestSuite_RenUtils, D3DFixture)
	BOOST_AUTO_TEST_SUITE(TestSuite_UniformBuffer)
	BOOST_AUTO_TEST_CASE(Test_First)
	{
		SetupTest("First");

		// @TODO
	}
	BOOST_AUTO_TEST_SUITE_END() // TestSuite_UniformBuffer
	BOOST_AUTO_TEST_SUITE_END() // TestSuite_RenUtils
	BOOST_AUTO_TEST_SUITE_END() // TestSuite_Impl
	BOOST_AUTO_TEST_SUITE_END() // TestSuite_QRen
} // Test::ISpr::QSpr::IMPL
} // Test::ISpr::QSpr
} // Test::ISpr
} // Test