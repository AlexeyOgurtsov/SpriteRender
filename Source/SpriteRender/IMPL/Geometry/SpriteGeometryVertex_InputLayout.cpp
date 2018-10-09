#include "SpriteGeometryVertex_InputLayout.h"
#include "SpriteRender/IMPL/RenUtils/ShaderCompileUtils.h"
#include "SpriteRender/INTERFACE/CONFIG/SpriteRenderShadersConfig.h"
#include <fstream>
#include <cstdlib>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{
namespace D3D
{

constexpr const UINT SPRITE_VB_SLOT_INDEX = 0;

constexpr const char* INPUT_LAYOUT_ELEMENT_POSITION = "Position";
constexpr const char* INPUT_LAYOUT_ELEMENT_TEXCOORD = "TexCoord";

InputLayoutHandle RegisterInputLayout_SpriteGeometryVertex(ID3D11Device* pDev, const void* pShaderBytecode, UINT InShaderBytecodeLength)
{
	D3D11_INPUT_ELEMENT_DESC position;
	ZeroMemory(&position, sizeof(D3D11_INPUT_ELEMENT_DESC));
	position.SemanticName = INPUT_LAYOUT_ELEMENT_POSITION;
	position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	position.InputSlot = SPRITE_VB_SLOT_INDEX;
	position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	D3D11_INPUT_ELEMENT_DESC texCoord;
	ZeroMemory(&texCoord, sizeof(D3D11_INPUT_ELEMENT_DESC));
	texCoord.SemanticName = INPUT_LAYOUT_ELEMENT_TEXCOORD;
	texCoord.Format = DXGI_FORMAT_R32G32_FLOAT;
	texCoord.InputSlot = SPRITE_VB_SLOT_INDEX;
	texCoord.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	const UINT NUM_ELEMENTS = 2;
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[NUM_ELEMENTS] =
	{
		position, texCoord
	};

	ID3D11InputLayout* pInputLayout = nullptr;
	HRESULT hr_create = pDev->CreateInputLayout(inputLayoutDesc, NUM_ELEMENTS, pShaderBytecode, InShaderBytecodeLength, &pInputLayout);
	if(FAILED(hr_create))
	{
		throw SpriteRenderHRFailure("Failed to create input layout for sprite geometry", hr_create);
	}
	return InputLayoutHandle(pInputLayout);
}

InputLayoutHandle RegisterInputLayout_SpriteGeometryVertex(ID3D11Device* pDev, const SShadersConfig& InConfig)
{
	const char* ShaderSource =
	"struct VS_INPUT {\n"
	"	float3 position : POSITION;\n"
	"	float2 tex : TEXCOORD; \n"
	"};\n"
	"struct VS_OUTPUT {\n"
	"	float4 position : SV_POSITION;\n"
	"};\n"
	"VS_OUTPUT vs_main(VS_INPUT InVert) {\n"
	"	VS_OUTPUT result;\n"
	"	result.position = float4(InVert.position, 1.0);\n"
	"	return result;\n"
	"}\n";

	std::ofstream shaderLog = OpenShaderLogFile("SpriteInputLayout_TestShader.txt", InConfig.OutputDirectory.c_str());
	VertexShader shader = CreateVertexShaderFromMemory
	(
		pDev,
		shaderLog,
		"SpriteInputLayout_TestShader",
		ShaderSource,
		VERTEX_SHADER_ENTRY_POINT,
		VERTEX_SHADER_PROFILE,
		InConfig.CompileFlags
	);
	
	auto pBlob = shader.GetBlob();
	return RegisterInputLayout_SpriteGeometryVertex(pDev, pBlob->GetBufferPointer(), pBlob->GetBufferSize());
}

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv::Spr
} // Dv