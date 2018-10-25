#pragma once

#include "BufferAlloc.h"

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
	void BufferAlloc::Invalidate()
	{
		OffsetInSlots = 0;
		NumSlots = 0;
	}
	bool BufferAlloc::IsValid() const
	{
		return NumSlots > 0;
	}

	const BufferAlloc BufferAlloc::Invalid{ 0, 0 };

	std::string ToString(const BufferAlloc& InAlloc)
	{
		std::string result;
		result.append("{");

		result.append("Offset=");
		result.append(std::to_string(InAlloc.OffsetInSlots));

		result.append("; NumSlots=");
		result.append(std::to_string(InAlloc.NumSlots));

		if (false == InAlloc.IsValid())
		{
			result.append("; INVALID!");
		}

		result.append("}");
		return result;
	}
} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv