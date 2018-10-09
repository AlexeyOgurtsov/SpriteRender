#include "RenBufferUtils.h"
#include <cstdlib>

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

BufferHandle CreateVertexBuffer(ID3D11Device* pDev, int Capacity, bool bInDynamic)
{
	return CreateBuffer(pDev, Capacity, D3D11_BIND_VERTEX_BUFFER, bInDynamic);
}

BufferHandle CreateBuffer(ID3D11Device* pDev, int Capacity, UINT BindFlags, bool bInDynamic)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = Capacity;
	desc.BindFlags = BindFlags;
	if(bInDynamic)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
	}
	ID3D11Buffer* pBuffer = nullptr;
	HRESULT hr_create = pDev->CreateBuffer(&desc, nullptr, &pBuffer);
	if(FAILED(hr_create))
	{
		throw SpriteRenderHRFailure("Failed to create buffer", hr_create);
	}
	return BufferHandle(pBuffer);
}

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv