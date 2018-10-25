#pragma once

#include "ISprite/SpriteTypes.h"
#include "QSpriteRender/IMPL/RenUtils/RenUtilsSystem.h"

/**
* Utils for internal usage of the sprite set render.
*/

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

namespace D3D
{
	class RenResources;
}

class Sprite;

/**
* Setups all per-sprite render state, excluding the common state.
*/
void SetSpriteRS(ID3D11DeviceContext* pInDevCon, const Sprite* pInSprite, const D3D::RenResources* pInRenResources);

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv