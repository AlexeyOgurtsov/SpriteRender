#include "../ISprPerTestFixture_CustomPick.h"

namespace Test::ISpr
{
	ISprPerTestFixture_CustomPick::ISprPerTestFixture_CustomPick()
	{
		T_LOG("ISprPerTestFixture_CustomPick::ctor...");
		EnablePick();
		T_LOG("ISprPerTestFixture_CustomPick::ctor DONE");
	}

	ISprPerTestFixture_CustomPick::~ISprPerTestFixture_CustomPick()
	{
		T_LOG("ISprPerTestFixture_CustomPick::~DTOR...");
		DisablePick();
		T_LOG("ISprPerTestFixture_CustomPick::~DTOR DONE");
	}
} // Test::ISpr