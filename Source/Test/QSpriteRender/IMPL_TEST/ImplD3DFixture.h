#pragma once

#include "Test/ISpriteRenderTestUtils/Fixture/D3DFixtureBase.h"

namespace Test::ISpr::QSpr::IMPL
{
	/**
	* To be used for any tests that use d3d.
	*/
	class D3DFixture : public D3DFixtureBase
	{
	public:
		D3DFixture();
		~D3DFixture();
	};
} // Test::ISpr::QSpr::IMPL