#pragma once

#include "RenDrawCall.h"

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

void PerformDrawCall(ID3D11DeviceContext* pDevCon, const SDrawCall& InDrawCall);

} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv