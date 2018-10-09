#pragma once

/**
* Factory methods for creating material instances of all native material types.
*/

#include "ISpriteRender/SpriteMaterialInstanceRenderState.h"
#include <d3d11.h>
#include <memory>

namespace Dv
{
namespace Spr
{
namespace Ren
{

class ISpriteRenderSubsystem;

std::shared_ptr<SpriteMaterialInstanceRenderState> CreateMatInst_Default(ISpriteRenderSubsystem* pInRenderSubsystem, ID3D11ShaderResourceView* pInTexture2D);

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv