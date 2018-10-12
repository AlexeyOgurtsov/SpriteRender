#pragma once

#include "ISprPerTestFixture_SingleCanvas.h"
#include "ISpriteRenderTestUtils/Fixture/ResourceFixture.h"

namespace Test::ISpr
{
	class ISprPerTestFixture_SingleCanvas_Smoke :
		public ISprPerTestFixture_SingleCanvas
	,	public ResourceFixture
	{
	public:
		ISprPerTestFixture_SingleCanvas_Smoke();
		~ISprPerTestFixture_SingleCanvas_Smoke();

	protected:

	private:
	};
} // Test::ISpr