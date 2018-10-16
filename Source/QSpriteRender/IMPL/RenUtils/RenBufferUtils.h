#pragma once

#include "RenBufferTypes.h"

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

BufferHandle CreateVertexBuffer(ID3D11Device* pDev, int Capacity, bool bInDynamic);
BufferHandle CreateBuffer(ID3D11Device* pDev, int Capacity, UINT BindFlags, bool bInDynamic);

} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv