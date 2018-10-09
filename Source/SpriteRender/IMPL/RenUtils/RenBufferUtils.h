#pragma once

#include "RenBufferTypes.h"

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

BufferHandle CreateVertexBuffer(ID3D11Device* pDev, int Capacity, bool bInDynamic);
BufferHandle CreateBuffer(ID3D11Device* pDev, int Capacity, UINT BindFlags, bool bInDynamic);

} // Dv::Spr::Ren::IMPL::D3D

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv