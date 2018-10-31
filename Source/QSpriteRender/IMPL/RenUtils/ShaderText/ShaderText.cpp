#pragma once

#include "ShaderText.h"
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
	std::string GetHLSLTypeFromFormat(DXGI_FORMAT InFormat)
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
			return std::string("float4");

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return std::string("float4");

		case DXGI_FORMAT_R32G32B32_FLOAT:
			return std::string("float3");

		case DXGI_FORMAT_R32G32_FLOAT:
			return std::string("float2");

		case DXGI_FORMAT_R32_FLOAT:
			return std::string("float");

		case DXGI_FORMAT_R8G8B8A8_UINT:
			return std::string("uint4");

		case DXGI_FORMAT_R32_UINT:
			return std::string("uint");

		case DXGI_FORMAT_R16_UINT:
			return std::string("short");

		default:
			BOOST_ASSERT_MSG(false, "GetHLSLTypeFromFormat: case for this literal is not supported or NOT yet implemented");
		}
		BOOST_ASSERT_MSG(false, "GetHLSLTypeFromFormat: should not get here");
		return "{FATAL_ERROR}";
	}

	std::string GetTextureHLSLTypeFromFormat(DXGI_FORMAT InFormat, UINT InSamples)
	{
		std::string const TextureKeyword = InSamples > 1 ? "Texture2DMS" : "Texture2D";

		return TextureKeyword + std::string("<") + GetHLSLTypeFromFormat(InFormat) + std::string(">");
	}

	void ShaderText::AppendSampler(const std::string& InString)
	{
		samplers.emplace_back(InString);
	}

	void ShaderText::AppendSampler(const char* pInName, UINT InRegister)
	{
		AppendSampler
		(
			std::string("SamplerState")
			+ std::string(" ")
			+ std::string(pInName)
			+ std::string(" : register(s")
			+ std::to_string(InRegister)
			+ std::string(");")
		);
	}

	void ShaderText::AppendCB(const std::string& InString)
	{
		constantBuffers.emplace_back(InString);
	}

	void AppendText(std::string* pOutText, const std::vector<std::string>& InText)
	{
		BOOST_ASSERT(pOutText);
		for (int i = 0; i < static_cast<int>(InText.size()); i++)
		{
			pOutText->append(InText[i]);
			pOutText->append("\n");
		}
	}

	void ShaderText::AppendCB(const char* pInName, UINT InRegister, const std::vector<std::string>& InFields)
	{
		std::string result;
		AppendShaderText_CBHeader_Begin(&result, InRegister, pInName);
		AppendText(&result, InFields);		
		AppendShaderText_CBHeader_End(&result);	

		AppendCB(result);
	}

	void ShaderText::AppendTexture(const std::string& InString)
	{
		textures.emplace_back(InString);
	}

	void ShaderText::AppendTexture(const char* pInName, UINT InRegister, DXGI_FORMAT InFormat, UINT InSamples)
	{
		AppendTexture
		(
			GetTextureHLSLTypeFromFormat(InFormat, InSamples)
			+ std::string(" ")
			+ std::string(pInName) 
			+ std::string(" : register(t") 
			+ std::to_string(InRegister) 
			+ std::string(");")
		);
	}

	void ShaderText::AppendStruct(const std::string& InString)
	{
		structs.emplace_back(InString);
	}

	void ShaderText::AppendStruct(const char* pInName, const std::vector<std::string>& InFields)
	{
		std::string result;
		result.append("struct ");
		result.append(pInName);
		result.append("{\n");
		AppendText(&result, InFields);
		result.append("};\n");

		AppendStruct(result);
	}

	void ShaderText::AppendVertexShaderText(const std::vector<std::string>& InStatements)
	{
		AppendText(&vs_main_text, InStatements);
	}

	void ShaderText::AppendPixelShaderText(const std::vector<std::string>& InStatements)
	{
		AppendText(&ps_main_text, InStatements);
	}

	void ShaderText::SetVertexShaderHeader(const std::string& InHeader)
	{
		vs_header = InHeader;
	}

	void ShaderText::SetPixShaderHeader(const std::string& InHeader)
	{
		ps_header = InHeader;
	}

	void AppendStructs(std::string* pOutText, const ShaderText& InText)
	{
		for (int i = 0; i < static_cast<int>(InText.GetNumStructs()); i++)
		{
			pOutText->append(InText.GetStructByIndex(i));
			pOutText->append("\n");
		}
	}

	void AppendSamplers(std::string* pOutText, const ShaderText& InText)
	{
		for (int i = 0; i < static_cast<int>(InText.GetNumSamplers()); i++)
		{
			pOutText->append(InText.GetSamplerByIndex(i));
			pOutText->append("\n");
		}
	}

	void AppendTextures(std::string* pOutText, const ShaderText& InText)
	{
		for (int i = 0; i < static_cast<int>(InText.GetNumTextures()); i++)
		{
			pOutText->append(InText.GetTextureByIndex(i));
			pOutText->append("\n");
		}
	}

	void AppendConstantBuffers(std::string* pOutText, const ShaderText& InText)
	{
		for (int i = 0; i < static_cast<int>(InText.GetNumConstantBuffers()); i++)
		{
			pOutText->append(InText.GetConstantBufferByIndex(i));
			pOutText->append("\n");
		}
	}

	void AppendShader(std::string* pOutText, const std::string& InHeader, const std::string& InText)
	{
		pOutText->append(InHeader);
		pOutText->append("{\n");
		pOutText->append(InText);
		pOutText->append("}\n");
	}

	void AppendVertexShader(std::string* pOutText, const ShaderText& InText)
	{
		AppendShader(pOutText, InText.GetVertexShaderHeader(), InText.GetVertexMainText());
	}


	void AppendPixShader(std::string* pOutText, const ShaderText& InText)
	{
		AppendShader(pOutText, InText.GetPixShaderHeader(), InText.GetPixMainText());
	}

	std::string ConvertShaderTextToString(const ShaderText& InText)
	{
		std::string result;
		AppendStructs(&result, InText);
		AppendSamplers(&result, InText);
		AppendTextures(&result, InText);
		AppendConstantBuffers(&result, InText);
		AppendVertexShader(&result, InText);
		AppendPixShader(&result, InText);
		return result;
	}
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv