#include "UniformBuffer.h"
#include "RenBufferUtils.h"

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
	namespace
	{
		D3D11_BUFFER_DESC GetDescForCapacity(const UniformBuffer* InBuffer, UINT InNumSlots)
		{
			D3D11_BUFFER_DESC Desc;
			ZeroMemory(&Desc, sizeof(Desc));
			Desc.ByteWidth = InBuffer->GetSlotSize() * InNumSlots;
			Desc.Usage = InBuffer->GetUsage();
			Desc.CPUAccessFlags = InBuffer->GetCpuAccessFlags();
			Desc.BindFlags = InBuffer->GetBindFlags();
			return Desc;
		}
	}
} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv