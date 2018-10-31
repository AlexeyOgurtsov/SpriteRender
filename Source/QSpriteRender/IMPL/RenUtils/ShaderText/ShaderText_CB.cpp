#pragma once

#include <string>
#include <vector>
#include <boost/assert.hpp>
#include "ShaderText_CB.h"

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
	void AppendShaderText_CBHeader_Begin(std::string* pOutString, unsigned int InSlotIndex, const char* pInName)
	{
		BOOST_ASSERT(pOutString);
		BOOST_ASSERT(pInName);

		pOutString->append("cbuffer ");
		pOutString->append(pInName);
		pOutString->append(" : register(b");
		pOutString->append(std::to_string(InSlotIndex));
		pOutString->append(")\n{\n");
	}
	
	void AppendShaderText_CBHeader_End(std::string* pOutString)
	{
		BOOST_ASSERT(pOutString);
		pOutString->append("}\n");
	}

	void AppendShaderText_Field(std::vector<std::string>* pOutText, const std::string& InHLSLTypeName, const std::string& InFieldName)
	{
		std::string result;
		result.append(InHLSLTypeName);
		result.append(" ");
		result.append(InFieldName);
		result.append(";");
		pOutText->emplace_back(result);
	}
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv