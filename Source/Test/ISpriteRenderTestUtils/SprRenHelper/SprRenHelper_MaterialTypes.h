#pragma once

#include "SprRenHelper_System.h"

#include "ISpriteRender/SpriteMaterialInstanceRenderState.h"
#include <memory>

namespace Test
{
	using Handle_SprMaterialInstance = std::shared_ptr<MySprRen::SpriteMaterialInstanceRenderState>;
} // Test