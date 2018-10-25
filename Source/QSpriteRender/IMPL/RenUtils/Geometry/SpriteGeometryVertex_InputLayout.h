#pragma once

#include "QSpriteRender/IMPL/RenUtils/RenInputLayoutTypes.h"

namespace Dv
{
namespace Spr
{

struct SShadersConfig;
namespace QRen
{
namespace IMPL
{
namespace D3D
{
	InputLayoutHandle RegisterInputLayout_SpriteGeometryVertex(ID3D11Device* pDev, const SShadersConfig& InConfig);

} // Dv::Spr::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv
