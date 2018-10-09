#pragma once

#include "RenDrawCall.h"

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

void PerformDrawCall(ID3D11DeviceContext* pDevCon, const SDrawCall& InDrawCall);

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv