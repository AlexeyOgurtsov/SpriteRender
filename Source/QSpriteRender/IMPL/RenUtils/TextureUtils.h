#pragma once

#include <d3d11.h>
#include <memory>
#include "RenUtilsSystem.h"
#include "TextureTypes.h"

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
	TextureHandle CreateTexture2D
	(
		ID3D11Device* pDev, 
		UINT InWidth, UINT InHeight, DXGI_FORMAT InFormat,
		UINT InBindFlags,
		D3D11_USAGE InUsage = D3D11_USAGE_DEFAULT,
		UINT InCpuAccessFlags = 0,
		UINT NumMips = 0
	);
} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL 
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv