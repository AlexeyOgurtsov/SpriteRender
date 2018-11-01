#pragma once

#include <d3d11.h>
#include <memory>
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
	using SRVHandle = std::unique_ptr<ID3D11ShaderResourceView, ComReleaser>;
	using UAVHandle = std::unique_ptr<ID3D11UnorderedAccessView, ComReleaser>;
} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL 
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv