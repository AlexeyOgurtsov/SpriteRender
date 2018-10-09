#pragma once

#include "RenUtilsSystem.h"

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

using InputLayoutHandle = std::unique_ptr<ID3D11InputLayout, ComReleaser>;

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv