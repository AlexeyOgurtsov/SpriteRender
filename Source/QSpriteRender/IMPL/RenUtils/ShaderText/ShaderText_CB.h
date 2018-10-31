#pragma once

#include <string>
#include <vector>

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
	void AppendShaderText_CBHeader_Begin(std::string* pOutString, unsigned int InSlotIndex, const char* pInName);
	void AppendShaderText_CBHeader_End(std::string* pOutString);
	void AppendShaderText_Field(std::vector<std::string>* pOutText, const std::string& InHLSLTypeName, const std::string& InFieldName);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv