#pragma once

#include <string>
#include <d3d11.h>
#include "SpriteRender/IMPL/RenUtils/ShaderProg.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

/**
* Based on the given sprite material shader config the shader is to be generated.
*/
struct SShaderConfig_DefaultSpriteMaterial
{
	DXGI_FORMAT TextureFormat;

	SShaderConfig_DefaultSpriteMaterial(DXGI_FORMAT InTextureFormat) :
		TextureFormat{ InTextureFormat } {}
};
inline bool operator==(const SShaderConfig_DefaultSpriteMaterial& lhs, const SShaderConfig_DefaultSpriteMaterial& rhs)
{
	return lhs.TextureFormat == rhs.TextureFormat;
}
inline bool operator!=(const SShaderConfig_DefaultSpriteMaterial& lhs, const SShaderConfig_DefaultSpriteMaterial& rhs)
{
	return ! (operator==(lhs, rhs));
}

namespace ShaderText
{

std::string GenerateShaderText_DefaultSpriteMaterial
(
	const SShaderConfig_DefaultSpriteMaterial& InShaderConfig
);

} // Dv::Spr::Ren::IMPL::ShaderText

/**
* Creates compiled shader program for the default sprite material.
*/

namespace D3D
{
	struct SShaderCompileEnv;
}

D3D::ShaderProgOwnerHandle CreateShaderProg_DefaultSpriteMaterial(const char *pInShaderInstaceName, const SShaderConfig_DefaultSpriteMaterial& InShaderConfig, const D3D::SShaderCompileEnv& InEnv);

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv