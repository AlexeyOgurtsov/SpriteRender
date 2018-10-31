#include "DefaultSpriteMaterialShader.h"
#include "../IMPL/Render/Resources/RSShaderText.h"
#include "QSpriteRender/IMPL/RenUtils/ShaderCompileUtils.h"

#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

namespace ShaderText
{

void AppendShaderText_VertexShader(D3D::ShaderText* pOutResultText, const SShaderConfig_DefaultSpriteMaterial& InShaderConfig)
{
	pOutResultText->SetVertexShaderHeader("VS_OUTPUT vs_main(VS_INPUT InVert)");
	std::vector<std::string> VertexShaderText;
	VertexShaderText.push_back("	VS_OUTPUT result;");
	VertexShaderText.push_back("	result.sprId = InVert.sprId;");
	D3D::AppendStmt_Assign_MulVec3ByMatrix44(&VertexShaderText, "result.position", "InVert.position", D3D::CanvasCBLayout::MATRIX_FIELD_NAME);
	VertexShaderText.push_back("	result.texCoord = InVert.texCoord;");
	VertexShaderText.push_back("	return result;");
	pOutResultText->AppendVertexShaderText(VertexShaderText);
}

void AppendShaderText_PixelShader(D3D::ShaderText* pOutResultText, const SShaderConfig_DefaultSpriteMaterial& InShaderConfig)
{
	pOutResultText->SetPixShaderHeader("float4 ps_main(VS_OUTPUT InFrag) : SV_TARGET ");
	std::vector<std::string> PixShaderText;
	PixShaderText.push_back("return diffuseTexture.Sample(defaultSampler, InFrag.texCoord);");
	pOutResultText->AppendPixelShaderText(PixShaderText);	
}

void AppendShaderText_VS_INPUT(D3D::ShaderText* pOutResultText, const SShaderConfig_DefaultSpriteMaterial& InShaderConfig)
{
	std::vector<std::string> VS_INPUT_STRUCT;
	VS_INPUT_STRUCT.push_back("	uint sprId : SPR_ID;");
	VS_INPUT_STRUCT.push_back("	float3 position : POSITION;");
	VS_INPUT_STRUCT.push_back("	float2 texCoord : TEXCOORD;");
	pOutResultText->AppendStruct("VS_INPUT", VS_INPUT_STRUCT);
}

void AppendShaderText_PS_OUTPUT(D3D::ShaderText* pOutResultText, const SShaderConfig_DefaultSpriteMaterial& InShaderConfig)
{
	std::vector<std::string> VS_OUTPUT_STRUCT;
	VS_OUTPUT_STRUCT.push_back("	uint sprId : SPR_ID;");
	VS_OUTPUT_STRUCT.push_back("	float4 position : SV_POSITION;");
	VS_OUTPUT_STRUCT.push_back("	float2 texCoord : TEXCOORD;");
	pOutResultText->AppendStruct("VS_OUTPUT", VS_OUTPUT_STRUCT);
}

std::string GenerateShaderText_DefaultSpriteMaterial
(
	const SShaderConfig_DefaultSpriteMaterial& InShaderConfig
)
{
	BOOST_ASSERT_MSG(InShaderConfig.TextureFormat == DXGI_FORMAT_R8G8B8A8_UNORM, "Unsupported Default Sprite Material texture format");

	D3D::ShaderText ResultText;
	ResultText.AppendSampler("defaultSampler", 0);
	ResultText.AppendTexture("diffuseTexture", 0, InShaderConfig.TextureFormat, /*Samples=*/0);

	AppendShaderText_CanvasCBLayout(&ResultText);

	AppendShaderText_VS_INPUT(&ResultText, InShaderConfig);
	AppendShaderText_PS_OUTPUT(&ResultText, InShaderConfig);

	AppendShaderText_VertexShader(&ResultText, InShaderConfig);
	AppendShaderText_PixelShader(&ResultText, InShaderConfig);

	return D3D::ConvertShaderTextToString(ResultText);
}

} // Dv::Spr::QRen::IMPL::ShaderText

D3D::ShaderProgOwnerHandle CreateShaderProg_DefaultSpriteMaterial(const char *pInShaderInstaceName, const SShaderConfig_DefaultSpriteMaterial& InShaderConfig, const D3D::SShaderCompileEnv& InEnv)
{
	std::string Source = ShaderText::GenerateShaderText_DefaultSpriteMaterial(InShaderConfig);
	return D3D::CreateShaderProgFromMemory
	(
		pInShaderInstaceName,
		Source,
		InEnv
	);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv