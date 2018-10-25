#include "SpriteSetRender.h"
#include "../Environment/AmbientContext.h"
#include "../Utils/CommonSystem.h"
#include "../RenUtils/RenDrawCallUtils.h"
#include "SpriteSetRenderUtils.h"
#include "SpriteManager.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{
	SpriteSetRender::SpriteSetRender(const SpriteSetRenderInitializer& InInitializer) :
		pAmbientContext{InInitializer.pAmbientContext}
	,	pSprites{InInitializer.pSpriteManager}
	,	pRenResources{InInitializer.pRenResources}
	{
		BOOST_ASSERT(pAmbientContext);
		BOOST_ASSERT(pSprites);
		BOOST_ASSERT(pRenResources);
	}

	void SpriteSetRender::Render(ID3D11DeviceContext* pInDevCon, UINT InVBSlot)
	{		
		pSprites->SetVB(pInDevCon, InVBSlot);
		for (auto SpriteIt = pSprites->Iterator_VisibleSpriteByZOrder(); SpriteIt; ++SpriteIt)
		{
			RenderSprite(pInDevCon, *SpriteIt);
		}
	}

	void SpriteSetRender::RenderSprite(ID3D11DeviceContext* pInDevCon, const Sprite* pInSprite)
	{
		SetSpriteRS(pInDevCon, pInSprite, pRenResources);
		
		D3D::SDrawCall DrawCall { 4 * pInSprite->GetVBAlloc().OffsetInSlots, 4 };
		D3D::PerformDrawCall(pInDevCon, DrawCall);
	}

	std::ofstream& SpriteSetRender::GetLog() const
	{
		return pAmbientContext->GetLog();
	}
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv