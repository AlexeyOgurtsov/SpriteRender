#pragma once

#include "SpriteMaterial.h"
#include <d3d11.h>
#include "DefaultSpriteMaterialShader.h"
#include "SpriteRender/IMPL/RenUtils/ShaderProg.h"
#include <unordered_map>

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
	struct SShaderCompileEnv;
}

struct SDefaultMaterialInitializer
{
	SpriteMaterialId Id;
	const D3D::SShaderCompileEnv* pShaderCompileEnv = nullptr;

	SDefaultMaterialInitializer(SpriteMaterialId InId, const D3D::SShaderCompileEnv* pInShaderCompileEnv) :
		Id(InId)
	,	pShaderCompileEnv(pInShaderCompileEnv) {}
};

using DefaultShaderConfigId = int; 


/**
* Implementation of the default material.
*/

class DefaultSpriteMaterial : public IMaterial
{
public:
	DefaultSpriteMaterial(const SDefaultMaterialInitializer& InInitializer);

	virtual SpriteMaterialId GetId() const override { return _Id; }

	/**
	* To be called each time a new material instance with a new shader config is added
	*
	* Returns the same config if the same config is registered more than once.
	*/
	DefaultShaderConfigId PrepareShaderConfig(const SShaderConfig_DefaultSpriteMaterial& InShaderConfig);

	/**
	* To be called to setup the render state of the material.
	*/
	void SetupRS(ID3D11DeviceContext* pDevCon, DefaultShaderConfigId InConfigId);

private:
	SpriteMaterialId _Id;

	const D3D::SShaderCompileEnv* _pShaderCompileEnv = nullptr;
	struct ShaderConfig
	{
		SShaderConfig_DefaultSpriteMaterial Desc;
		D3D::ShaderProgOwnerHandle ShaderProg;

		ShaderConfig(const SShaderConfig_DefaultSpriteMaterial& InDesc, D3D::ShaderProgOwnerHandle&& InProg) :
			Desc(InDesc)
		,	ShaderProg(std::move(InProg)) {}
	};
	const ShaderConfig* FindShaderConfigById(DefaultShaderConfigId InId) const;
	using ShaderConfigMapById = std::unordered_map<DefaultShaderConfigId, ShaderConfig>;
	ShaderConfigMapById _shadersConfigs;
	int _nextId = 0;
	int GenerateNextId() { return _nextId++; }
};
DefaultShaderConfigId PrepareShader_ForDefaultMaterialInstance(DefaultSpriteMaterial* pInMaterial, ID3D11ShaderResourceView* pInColorTextureView);


} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv