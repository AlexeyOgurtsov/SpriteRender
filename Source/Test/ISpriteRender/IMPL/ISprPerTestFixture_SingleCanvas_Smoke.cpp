#include "../ISprPerTestFixture_SingleCanvas_Smoke.h"

namespace Test::ISpr
{
	ISprPerTestFixture_SingleCanvas_Smoke::ISprPerTestFixture_SingleCanvas_Smoke()
	{
		T_LOG("ISprPerTestFixture_SingleCanvas_Smoke: CTOR...");
		SetTextureFormat(GetSprRen_DefaultTextureFormat_Diffuse());
		T_LOG("ISprPerTestFixture_SingleCanvas_Smoke: CTOR DONE");
	}
	ISprPerTestFixture_SingleCanvas_Smoke::~ISprPerTestFixture_SingleCanvas_Smoke()
	{
		T_LOG("ISprPerTestFixture_SingleCanvas_Smoke: ~DTOR...");
		T_LOG("ISprPerTestFixture_SingleCanvas_Smoke: ~DTOR DONE");
	}
} // Test::ISpr