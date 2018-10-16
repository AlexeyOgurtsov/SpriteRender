#include "DefaultSpriteMaterialShader.h"
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

std::string GenerateShaderText_DefaultSpriteMaterial
(
	const SShaderConfig_DefaultSpriteMaterial& InShaderConfig
)
{
	BOOST_ASSERT_MSG(InShaderConfig.TextureFormat == DXGI_FORMAT_R8G8B8A8_UNORM, "Unsupported Default Sprite Material texture format");

	const char * const Source = 
	"SamplerState defaultSampler : register(s0);\n"
	"Texture2D<float4> diffuseTexture : register(t0);\n"
	"\n"
	"struct VS_INPUT {\n"
	"	float3 position : POSITION;\n"
	"	float2 texCoord : TEXCOORD;\n"
	"};\n"
	"struct VS_OUTPUT {\n"
	"	float4 position : SV_POSITION;\n"
	"	float2 texCoord : TEXCOORD;\n"
	"};\n"
	"\n"
	"VS_OUTPUT vs_main(VS_INPUT InVert) {\n"
	"	VS_OUTPUT result;\n"
	"	result.position = float4(InVert.position, 1.0F);\n"
	"	result.texCoord = InVert.texCoord;\n"
	"	return result;\n"
	"}\n"
	"float4 ps_main(VS_OUTPUT InFrag) : SV_TARGET {\n"
	"	return diffuseTexture.Sample(defaultSampler, InFrag.texCoord);\n"
	"}\n";
	return std::string(Source);
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