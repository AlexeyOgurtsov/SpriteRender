#include "DefaultMaterialInstanceRS.h"
#include "DefaultSpriteMaterial.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

DefaultMaterialInstanceRS::DefaultMaterialInstanceRS(const SDefaultMaterialInstanceRSInitializer& InInitializer) :
	MaterialInstanceRenderState{InInitializer.pMaterial}
,	_pTypedMaterial{ InInitializer.pMaterial }
,	_pTexture{ InInitializer.pTexture }
,	_shaderConfigId{ InInitializer.shaderConfigId }
{
	BOOST_ASSERT(_pTypedMaterial);
	BOOST_ASSERT(_pTexture);
}

void DefaultMaterialInstanceRS::SetRenderState(ID3D11DeviceContext* pDevCon) const
{
	_pTypedMaterial->SetupRS(pDevCon, _shaderConfigId);

	ID3D11ShaderResourceView* const pTextureSRV = _pTexture;
	pDevCon->VSSetShaderResources(0, 1, &pTextureSRV);
	pDevCon->PSSetShaderResources(0, 1, &pTextureSRV);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv