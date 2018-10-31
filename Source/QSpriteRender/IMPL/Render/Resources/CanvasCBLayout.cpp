#include "CanvasCBLayout.h"
#include "../../RenUtils/ShaderText/ShaderText_CB.h"
#include "../../RenUtils/ShaderText/ShaderText.h"
#include "RSLayout.h"

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
	void AppendShaderText_CanvasCBLayout(class ShaderText* pOutText)
	{
		std::vector<std::string> Fields;
		
		AppendShaderText_Field(&Fields, CanvasCBLayout::MATRIX_FIELD_HLSL_TYPE, CanvasCBLayout::MATRIX_FIELD_NAME);
		AppendShaderText_Field(&Fields, CanvasCBLayout::CANVAS_ID_FIELD_HLSL_TYPE, CanvasCBLayout::CANVAS_ID_FIELD_NAME);
		AppendShaderText_Field(&Fields, CanvasCBLayout::VIEWPORT_WIDTH_FIELD_HLSL_TYPE, CanvasCBLayout::VIEWPORT_WIDTH_FIELD_NAME);
		AppendShaderText_Field(&Fields, CanvasCBLayout::VIEWPORT_HEIGHT_FIELD_HLSL_TYPE, CanvasCBLayout::VIEWPORT_HEIGHT_FIELD_NAME);
		AppendShaderText_Field(&Fields, CanvasCBLayout::CANVAS_MINIMUM_Z_FIELD_HLSL_TYPE, CanvasCBLayout::CANVAS_MINIMUM_Z_FIELD_NAME);

		pOutText->AppendCB(CBLayout::CB_Canvas_Name, CBLayout::Slot_Canvas, Fields);
	}
} // D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv