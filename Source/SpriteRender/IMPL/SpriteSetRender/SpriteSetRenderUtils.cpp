#include "SpriteSetRenderUtils.h"
#include "SpriteRender/IMPL/Sprite/Sprite.h"
#include "SpriteRender/IMPL/Geometry/ISpriteGeometry.h"
#include "SpriteRender/IMPL/RenUtils/RenResources.h"
#include "SpriteRender/IMPL/RenUtils/RenDrawCallUtils.h"
#include "SpriteRender/Material/SpriteMaterial.h"
#include "SpriteRender/Material/MaterialInstanceRenderState.h"
#include <boost/assert.hpp>

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
	class RenResources;
}

ID3D11BlendState* GetSpriteTransparencyBlendState(ESpriteTransparency InTransparency, const D3D::RenResources* pInRenResources)
{
	switch (InTransparency)
	{
	case ESpriteTransparency::Transparent:
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
	ID3D11BlendState* const pBlendState = GetSpriteTransparencyBlendState(pInSprite->GetTransparency(), pInRenResources);
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
* Sets up vertex buffer, that this sprite is loaded into.
*/
void SetRS_SpriteVertexBuffer(ID3D11DeviceContext* pInDevCon, const Sprite* pInSprite)
{
	auto const pGeometry = pInSprite->GetGeometry();
	ID3D11Buffer* const pBuffer = pGeometry->GetBuffer();
	const UINT stride = pGeometry->GetBufferStride();
	const UINT offset = pGeometry->GetBufferOffset();
	const UINT VB_SLOT = 0;
	pInDevCon->IASetVertexBuffers(VB_SLOT, 1, &pBuffer, &stride, &offset);
}

/**
* Setups entire render state of the sprite and performs rendering of the sprite.
*/
void RenderSprite(ID3D11DeviceContext* pInDevCon, const Sprite* pInSprite, const D3D::RenResources* pInRenResources)
{
	SetRS_SpriteMaterial(pInDevCon, pInSprite->GetRenderState());
	SetRS_SpriteTransparency(pInDevCon, pInSprite, pInRenResources);

	SetRS_SpriteVertexBuffer(pInDevCon, pInSprite);
	D3D::PerformDrawCall(pInDevCon, pInSprite->GetGeometry()->GetD3DStripDrawCall());
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv