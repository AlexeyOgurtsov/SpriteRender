#include "../ISprPerTestFixture_SingleCanvas_Smoke.h"

namespace Test::ISpr
{
	ISprPerTestFixture_SingleCanvas_Smoke::ISprPerTestFixture_SingleCanvas_Smoke()
	{
		T_LOG("ISprPerTestFixture_SingleCanvas_Smoke: CTOR...");
		// @TODO: Magic constant: Are this format dependent on concrete SpriteRender?
		SetTextureFormat(DXGI_FORMAT_R8G8B8A8_UNORM);
		T_LOG("ISprPerTestFixture_SingleCanvas_Smoke: CTOR DONE");
	}
	ISprPerTestFixture_SingleCanvas_Smoke::~ISprPerTestFixture_SingleCanvas_Smoke()
	{
		T_LOG("ISprPerTestFixture_SingleCanvas_Smoke: ~DTOR...");
		T_LOG("ISprPerTestFixture_SingleCanvas_Smoke: ~DTOR DONE");
	}
} // Test::ISpr