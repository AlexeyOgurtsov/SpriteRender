#pragma once

#include "RenUtilsSystem.h"

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

using InputLayoutHandle = std::unique_ptr<ID3D11InputLayout, ComReleaser>;

} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv