#pragma once

#include "DefaultSpriteMaterialTypedefs.h"
#include "MaterialInstanceRenderState.h"
#include <d3d11.h>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class DefaultSpriteMaterial;

struct SDefaultMaterialInstanceRSInitializer
{
	DefaultSpriteMaterial* pMaterial;
	ID3D11ShaderResourceView* pTexture;
	DefaultShaderConfigId shaderConfigId;
	
	SDefaultMaterialInstanceRSInitializer(DefaultSpriteMaterial* pInMaterial, ID3D11ShaderResourceView* pInTexture, DefaultShaderConfigId InShaderConfigId) :
		pMaterial(pInMaterial)
	,   pTexture(pInTexture)
	,   shaderConfigId(InShaderConfigId) {}
};

class DefaultMaterialInstanceRS : public MaterialInstanceRenderState
{
public:
	DefaultMaterialInstanceRS(const SDefaultMaterialInstanceRSInitializer& InInitializer);

	virtual void SetRenderState(ID3D11DeviceContext* pDevCon) const override;

private:
	DefaultShaderConfigId _shaderConfigId;
	DefaultSpriteMaterial* _pTypedMaterial;
	ID3D11ShaderResourceView* _pTexture;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv