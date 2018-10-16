#include "DefaultSpriteMaterial.h"
#include "DefaultSpriteMaterialShader.h"
#include <algorithm>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

SShaderConfig_DefaultSpriteMaterial GetShaderConfig_ForDefaultMaterial(ID3D11ShaderResourceView* pInColorTextureView)
{
	BOOST_ASSERT(pInColorTextureView);

	D3D11_SHADER_RESOURCE_VIEW_DESC ColorTextureViewDesc;
	pInColorTextureView->GetDesc(&ColorTextureViewDesc);
	BOOST_ASSERT_MSG(ColorTextureViewDesc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D, "Color texture must be 2D texture");

	ID3D11Resource* pColorTextureResource = nullptr;
	pInColorTextureView->GetResource(&pColorTextureResource);

	ID3D11Texture2D * const pTexture = static_cast<ID3D11Texture2D*>(pColorTextureResource);
	D3D11_TEXTURE2D_DESC ColorTextureDesc;
	pTexture->GetDesc(&ColorTextureDesc);

	SShaderConfig_DefaultSpriteMaterial ShaderConfig{ ColorTextureDesc.Format };
	return ShaderConfig;
}
	
DefaultShaderConfigId PrepareShader_ForDefaultMaterialInstance(DefaultSpriteMaterial* pInMaterial, ID3D11ShaderResourceView* pInColorTextureView)
{
	SShaderConfig_DefaultSpriteMaterial const ShaderConfig = GetShaderConfig_ForDefaultMaterial(pInColorTextureView);
	return pInMaterial->PrepareShaderConfig(ShaderConfig);
}

DefaultSpriteMaterial::DefaultSpriteMaterial(const SDefaultMaterialInitializer& InInitializer) :
	_Id(InInitializer.Id)
,	_pShaderCompileEnv(InInitializer.pShaderCompileEnv)
{
	BOOST_ASSERT(_pShaderCompileEnv);

}

DefaultShaderConfigId DefaultSpriteMaterial::PrepareShaderConfig(const SShaderConfig_DefaultSpriteMaterial& InShaderConfig)
{
	auto it_config = std::find_if
	(
		_shadersConfigs.begin(), _shadersConfigs.end(), 
		[&InShaderConfig](const ShaderConfigMapById::value_type& InValueType)
		{
			return InValueType.second.Desc == InShaderConfig;
		}
	);
	if (it_config != _shadersConfigs.end())
	{
		return it_config->first;
	}


	D3D::ShaderProgOwnerHandle Prog = CreateShaderProg_DefaultSpriteMaterial("DefaultSpriteMaterial", InShaderConfig, *_pShaderCompileEnv);
	DefaultShaderConfigId const id = GenerateNextId();
	_shadersConfigs.insert(std::make_pair(id, ShaderConfig{ InShaderConfig, std::move(Prog) }));
	return id;
}

void DefaultSpriteMaterial::SetupRS(ID3D11DeviceContext* pDevCon, DefaultShaderConfigId InConfigId)
{
	D3D::ShaderProg* pShaderProg = FindShaderConfigById(InConfigId)->ShaderProg.get();
	pDevCon->VSSetShader(pShaderProg->GetVertex()->GetShader(), nullptr, 0);
	pDevCon->PSSetShader(pShaderProg->GetPix()->GetShader(), nullptr, 0);
}

const DefaultSpriteMaterial::ShaderConfig* DefaultSpriteMaterial::FindShaderConfigById(DefaultShaderConfigId InId) const
{
	auto it = _shadersConfigs.find(InId);
	BOOST_ASSERT_MSG(it != _shadersConfigs.end(), "DefaultSpriteMaterial::FindShaderConfigById: unable to find shader config with the given id");
	return &it->second;
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv