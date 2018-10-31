#pragma once

#include "../../RenUtils/CBLayoutSystem.h"
#include <cstdint>
#include <d3d11.h>
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
	struct CBLayout
	{
		/**
		* Main constant buffer slot index.
		*/
		static constexpr UINT Slot_Main = 0;

		/**
		* Main constant buffer name.
		*/
		static constexpr const char* CB_Main_Name = "CBMain";

		/**
		* Canvas constant buffer slot index.
		*/
		static constexpr UINT Slot_Canvas = 1;

		/**
		* Canvas constant buffer name.
		*/
		static constexpr const char* CB_Canvas_Name = "CBCanvas";
	};
} // D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv