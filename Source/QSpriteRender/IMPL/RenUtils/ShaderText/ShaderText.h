#pragma once

#include <string>
#include <vector>
#include <d3d11.h> // UINT

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
	class ShaderText
	{
	public:
		UINT GetNumSamplers() const { return samplers.size(); }
		const std::string& GetSamplerByIndex(UINT InIndex) const { return samplers[InIndex]; }
		void AppendSampler(const std::string& InString);
		void AppendSampler(const char* pInName, UINT InRegister);

		UINT GetNumConstantBuffers() const { return constantBuffers.size(); }
		const std::string& GetConstantBufferByIndex(UINT InIndex) const { return constantBuffers[InIndex]; }
		void AppendCB(const std::string& InString);
		void AppendCB(const char* pInName, UINT InRegister, const std::vector<std::string>& InFields);

		UINT GetNumTextures() const { return textures.size(); }
		const std::string& GetTextureByIndex(UINT InIndex) const { return textures[InIndex]; }
		void AppendTexture(const std::string& InString);
		void AppendTexture(const char* pInName, UINT InRegister, DXGI_FORMAT InFormat, UINT InSamples);

		UINT GetNumStructs() const { return structs.size(); }
		const std::string& GetStructByIndex(UINT InIndex) const { return structs[InIndex]; }
		void AppendStruct(const std::string& InString);
		void AppendStruct(const char* pInName, const std::vector<std::string>& InFields);

		const std::string& GetVertexShaderHeader() const { return vs_header; }
		void SetVertexShaderHeader(const std::string& InHeader);

		const std::string& GetPixShaderHeader() const { return ps_header; }
		void SetPixShaderHeader(const std::string& InHeader);

		const std::string& GetVertexMainText() const { return vs_main_text; }
		const std::string& GetPixMainText() const { return ps_main_text; }

		void AppendVertexShaderText(const std::vector<std::string>& InStatements);
		void AppendPixelShaderText(const std::vector<std::string>& InStatements);

	private:
		std::vector<std::string> structs;
		std::vector<std::string> samplers;
		std::vector<std::string> constantBuffers; 
		std::vector<std::string> textures;
		std::string vs_header;
		std::string ps_header;
		std::string vs_main_text;
		std::string ps_main_text;
	};
	std::string ConvertShaderTextToString(const ShaderText& InText);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv