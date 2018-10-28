#include "SpriteSetRenderUtils.h"
#include "QSpriteRender/IMPL/Canvas/Sprite.h"
#include "QSpriteRender/IMPL/RenUtils/RenResources.h"
#include "QSpriteRender/Material/SpriteMaterial.h"
#include "QSpriteRender/Material/MaterialInstanceRenderState.h"
#include <boost/assert.hpp>

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
	class RenResources;
}

ID3D11BlendState* GetSpriteTransparencyBlendState(ESpriteTransparency InTransparency, const D3D::RenResources* pInRenResources)
{
	switch (InTransparency)
	{
	case ESpriteTransparency::BlendAdditive:
		return pInRenResources->GetBlendStateAdditive();

	case ESpriteTransparency::Opaque:
		return pInRenResources->GetBlendStateOpaque();

	default:
		break;
	}
	BOOST_ASSERT_MSG(false, "GetSpriteTransparencyBlendState: wrong transparency code");
	return nullptr;
}

void SetRS_SpriteTransparency(ID3D11DeviceContext* pInDevCon, const Sprite* pInSprite, const FLOAT InBlendFactor[4], UINT InBlendMask, const D3D::RenResources* pInRenResources)
{
	ID3D11BlendState* const pBlendState = GetSpriteTransparencyBlendState(pInSprite->GetRenderMode().Transparency, pInRenResources);
	pInDevCon->OMSetBlendState(pBlendState, InBlendFactor, InBlendMask);
}

/**
* Setups transparency of the sprite, and uses the default settings of the blend factor and mask.
*/
void SetRS_SpriteTransparency(ID3D11DeviceContext* pInDevCon, const Sprite* pInSprite, const D3D::RenResources* pInRenResources)
{
	const FLOAT BlendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };
	const UINT BlendMask = 0xffffffff;
	SetRS_SpriteTransparency(pInDevCon, pInSprite, BlendFactor, BlendMask, pInRenResources);
}

/**
* Sets up both the material and material instance render state.
*/
void SetRS_SpriteMaterial(ID3D11DeviceContext* pInDevCon, const MaterialInstanceRenderState* pInSprite)
{
	pInSprite->SetRenderState(pInDevCon);
}

/**
* Setups entire render state of the sprite and performs rendering of the sprite.
*/
void SetSpriteRS(ID3D11DeviceContext* pInDevCon, const Sprite* pInSprite, const D3D::RenResources* pInRenResources)
{
	SetRS_SpriteMaterial(pInDevCon, pInSprite->GetMaterialRenderState().get());
	SetRS_SpriteTransparency(pInDevCon, pInSprite, pInRenResources);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv