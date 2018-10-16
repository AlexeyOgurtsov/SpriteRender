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
namespace QRen
{

class ISpriteRenderSubsystem;

std::shared_ptr<Ren::SpriteMaterialInstanceRenderState> CreateMatInst_Default(ISpriteRenderSubsystem* pInRenderSubsystem, ID3D11ShaderResourceView* pInTexture2D);

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv