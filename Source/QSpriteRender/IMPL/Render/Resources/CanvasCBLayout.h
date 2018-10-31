#pragma once

#include "../../RenUtils/CBLayoutSystem.h"
#include "ISprite/SpriteCanvasTypedefs.h"
#include <cstdint>
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
	struct CanvasCBLayout
	{
		// 4x4 transformation matrix from canvas coordinate system
		static constexpr size_t MATRIX_OFFSET = 0;
		static constexpr size_t MATRIX_SIZE_IN_BYTES = CB_MATRIX_FOUR_FOUR_SIZE;
		static constexpr const char* MATRIX_FIELD_NAME = "CanvasTransform";
		static constexpr const char* MATRIX_FIELD_HLSL_TYPE = CB_MATRIX_FOUR_FOUR_HLSL_TYPE;

		// Canvas id
		static constexpr size_t CANVAS_ID_OFFSET = MATRIX_SIZE_IN_BYTES;
		static constexpr size_t CANVAS_ID_SIZE_IN_BYTES = CB_UINT32_SIZE;
		static constexpr const char* CANVAS_ID_FIELD_NAME = "CanvasId";
		static constexpr const char* CANVAS_ID_FIELD_HLSL_TYPE = CB_UINT32_HLSL_TYPE;

		// Canvas viewport width in texels
		static constexpr size_t VIEWPORT_WIDTH_OFFSET = CANVAS_ID_OFFSET + CANVAS_ID_SIZE_IN_BYTES;
		static constexpr size_t VIEWPORT_WIDTH_SIZE_IN_BYTES = CB_UINT32_SIZE;
		static constexpr const char* VIEWPORT_WIDTH_FIELD_NAME = "ViewportWidthTexels";
		static constexpr const char* VIEWPORT_WIDTH_FIELD_HLSL_TYPE = CB_UINT32_HLSL_TYPE;

		// Canvas viewport height in texels
		static constexpr size_t VIEWPORT_HEIGHT_OFFSET = VIEWPORT_WIDTH_OFFSET + VIEWPORT_WIDTH_SIZE_IN_BYTES;
		static constexpr size_t VIEWPORT_HEIGHT_SIZE_IN_BYTES = CB_UINT32_SIZE;
		static constexpr const char* VIEWPORT_HEIGHT_FIELD_NAME = "ViewportHeightTexels";
		static constexpr const char* VIEWPORT_HEIGHT_FIELD_HLSL_TYPE = CB_UINT32_HLSL_TYPE;

		// Minimum Z-value of srites of the canvas
		static constexpr size_t CANVAS_MINIMUM_Z_OFFSET = VIEWPORT_HEIGHT_OFFSET + VIEWPORT_HEIGHT_SIZE_IN_BYTES;
		static constexpr size_t CANVAS_MINIMUM_Z_SIZE_IN_BYTES = CB_FLOAT_SIZE;
		static constexpr const char* CANVAS_MINIMUM_Z_FIELD_NAME = "MinimumZ";
		static constexpr const char* CANVAS_MINIMUM_Z_FIELD_HLSL_TYPE = CB_FLOAT_HLSL_TYPE;

		// Total
		static constexpr size_t SIZE_IN_BYTES = CANVAS_MINIMUM_Z_OFFSET + CANVAS_MINIMUM_Z_SIZE_IN_BYTES;
	};
	static_assert((CanvasCBLayout::SIZE_IN_BYTES % 16 == 0), "CanvasCBLayout: size of constant buffer must be always a multiple of 16");

	void AppendShaderText_CanvasCBLayout(class ShaderText* pOutText);
} // D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv