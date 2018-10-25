#pragma once

#include <windows.h> // UINT
#include <string>

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
	using BufferSlotIndex = UINT;


	struct BufferAlloc
	{
		/**
		* Index of the first datum stored in the buffer.
		* Really offset in slots from the start of the buffer.
		*/
		BufferSlotIndex OffsetInSlots = 0;

		/**
		* Number of slots that this allocation occupies.
		*/
		UINT NumSlots = 0;
		
		BufferAlloc() = default;
		BufferAlloc(BufferSlotIndex InOffsetInSlots, UINT InNumSlots) :
			OffsetInSlots{InOffsetInSlots}
		,	NumSlots{InNumSlots} {}

		void Invalidate();
		bool IsValid() const;

		static const BufferAlloc Invalid;
	};
	std::string ToString(const BufferAlloc& InAlloc);
} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv