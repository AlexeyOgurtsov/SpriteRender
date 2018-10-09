#pragma once

#include "RenDeviceContextTypes.h"
#include "ISprite/Core/SpriteRenderException.h"

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

/**
* Tries to create deferred device context and throws an exception if error has been occured.
*/
DeviceContextHandle CreateDeferredContext(ID3D11Device* pInDev);

} // Dv::Spr::Ren::IMPL::D3D

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv