#pragma once

#include "ISprite/SpriteTypes.h"
#include "SpriteRender/IMPL/RenUtils/RenUtilsSystem.h"

/**
* Utils for internal usage of the sprite set render.
*/

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

namespace D3D
{
	class RenResources;
}

class Sprite;

/**
* Renders all passes that can be done consecutively when rendering a single sprite.
* Setups all per-sprite render state (however, the common render state is NOT set).
*/
void RenderSprite(ID3D11DeviceContext* pInDevCon, const Sprite* pInSprite, const D3D::RenResources* pInRenResources);

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv