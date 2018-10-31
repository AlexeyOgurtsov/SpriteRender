#include "SpriteSetRender.h"
#include "../Environment/AmbientContext.h"
#include "../RenUtils/RenResources.h"
#include "../Utils/CommonSystem.h"
#include "../RenUtils/RenDrawCallUtils.h"
#include "../Render//Resources/RSLayout.h"
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

		// Canvas buffer
		D3D::CanvasCBInitializer CanvasCBInitializer
		{
			InInitializer.bDebug,
			InInitializer.Name + std::string("_CB"),
			&InInitializer.pAmbientContext->GetLog(),
			pRenResources->GetDev(),
			pRenResources->GetDevCon()
		};
		pCB.reset(new D3D::CanvasCB(CanvasCBInitializer));
	}


	void SpriteSetRender::FlushD3D()
	{
		pCB->Flush();
	}

	void SpriteSetRender::Render(ID3D11DeviceContext* pInDevCon, UINT InVBSlot)
	{		
		pSprites->SetVB(pInDevCon, InVBSlot);
		pCB->SetCB(pInDevCon, D3D::CBLayout::Slot_Canvas);
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