#pragma once

/**
* Contains all tests to be performed for any ISpriteRender implementation.
*/

#include "MinimalSmokeTests.h"
#include "MinimalMoveZOrderTests.h"
#include "MinimalRotationTests.h"
#include "MinimalTransparencyTests.h"
#include "MinimalHeavyLoadTests.h"
#include "MinimalExtraChecks.h"

#ifdef TEST_FEATURE_MANY_CANVASSES
#include "FeatureManyCanvasSmokeTests.h"
#endif // TEST_FEATURE_MANY_CANVASSES

namespace Test::ISpr
{
} // Test::ISpr