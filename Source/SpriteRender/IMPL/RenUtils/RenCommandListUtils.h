#pragma once

#include "RenCommandListTypes.h"
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

CommandListHandle FinishCommandList(ID3D11DeviceContext* pDevCon);

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv