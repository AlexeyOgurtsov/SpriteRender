#pragma once

#include <d3d11.h>
#include <memory>
#include "RenUtilsSystem.h"
#include "SRVTypes.h"

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
	using TextureHandle = std::unique_ptr<ID3D11Texture2D, ComReleaser>;
} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL 
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv