#include "SpriteRender.h"
#include "SpriteUpdater.h"
#include "QSpriteRender/IMPL/Canvas/Canvas.h"
#include "QSpriteRender/IMPL/Subsystem/SpriteRenderSubsystemEnvironment.h"
#include "QSpriteRender/IMPL/Render/Resources/RSLayout.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

/**
* Setup the common render state.
*/
void SetupCommonRS(D3D::RenResources* pRenResources)
{
	ID3D11DeviceContext* const pDevCon = pRenResources->GetDevCon();

	pDevCon->IASetInputLayout(pRenResources->GetSpriteInputLayout());
	pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pDevCon->RSSetState(pRenResources->GetRasterizerState());

	// Sampler
	ID3D11SamplerState* const pSamplerState = pRenResources->GetDefaultSampler();
	pDevCon->VSSetSamplers(0, 1, &pSamplerState);
	pDevCon->PSSetSamplers(0, 1, &pSamplerState);

	ID3D11RenderTargetView* const pRTView = pRenResources->GetConfig().RenderTarget.pRenderTargetView;
	ID3D11DepthStencilView* const pDepthStencilView = pRenResources->GetConfig().RenderTarget.pDepthStencilView;
	pDevCon->OMSetRenderTargets(1, &pRTView, pDepthStencilView);
	pDevCon->OMSetDepthStencilState(pRenResources->GetDepthStencilState(), /*StencilRef*/0);
}

void SetRS_CanvasViewport(ID3D11DeviceContext* pDevCon, Canvas* pCanvas, D3D::RenResources* pRenResources)
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.Width = static_cast<float>(pCanvas->GetRect().Width);
	viewport.Height = static_cast<float>(pCanvas->GetRect().Height);
	viewport.TopLeftX = static_cast<float>(pCanvas->GetRect().Left);
	viewport.TopLeftY = static_cast<float>(pCanvas->GetRect().Top);
	viewport.MinDepth = 0.0F;
	viewport.MaxDepth = pRenResources->GetConfig().RenderTarget.ZFar;
	pDevCon->RSSetViewports(1, &viewport);
}

void SetRS_AndRenderCanvas(ID3D11DeviceContext* pDevCon, Canvas* pCanvas, UINT InVBSlot, D3D::RenResources* pRenResources)
{
	BOOST_ASSERT(pDevCon);
	BOOST_ASSERT(pCanvas);
	pCanvas->FlushD3D();
	SetRS_CanvasViewport(pDevCon, pCanvas, pRenResources);
	pCanvas->Render(pDevCon, InVBSlot);
}

void SetRS_AndRenderCanvas_IfVisible(ID3D11DeviceContext* pDevCon, Canvas* pCanvas, UINT InVBSlot, D3D::RenResources* pRenResources)
{
	if (pCanvas->IsVisible())
	{
		SetRS_AndRenderCanvas(pDevCon, pCanvas, InVBSlot, pRenResources);
	}
}


SpriteRender::~SpriteRender()
{
	// We use default destructor implementation
}

SpriteRender::SpriteRender(Environment* pInEnv) :
	_pEnv(pInEnv)
,	_bRenderingStarted(false)
,	_bUpdating(false)
{
	BOOST_ASSERT(_pEnv);

	SCanvasManagerInitializer CanvasManagerInitializer { pInEnv };
	pCanvasManager.reset(new CanvasManager(CanvasManagerInitializer));
}

/**
* Returns true, if we are inside the BeginUpdates/EndUpdates() pair.
*/
bool SpriteRender::IsUpdating() const
{
	return _bUpdating;
}

bool SpriteRender::IsUpdatingCanvas() const
{
	return GetCanvasUpdater() != nullptr;
}

SpriteUpdater* SpriteRender::GetCanvasUpdater() const
{
	return _pCanvasUpdater.get();
}

void SpriteRender::BeginFrame()
{
	BOOST_ASSERT_MSG( ! IsUpdating(), "SpriteRender::BeginFrame: NOT allowed to be called during the updating" );
	BOOST_ASSERT_MSG( ! _bRenderingStarted, "SpriteRender::BeginFrame: Nested BeginFrame/EndFrame calls are not supported" );

	_bRenderingStarted = true;
	SetupCommonRS(GetRenResources());
}

void SpriteRender::EndFrame()
{
	BOOST_ASSERT_MSG( ! IsUpdating(), "SpriteRender::EndFrame: NOT allowed to be called during the updating");
	BOOST_ASSERT_MSG(_bRenderingStarted, "SpriteRender::EndFrame: BeginFrame must be called.");

	_bRenderingStarted = false;
}

void SpriteRender::RenderAllVisibleCanvasses()
{
	BOOST_ASSERT_MSG(_bRenderingStarted, "SpriteRender::RenderAllVisibleCanvasses: must be called inside BeginFrame()/EndFrame pair only.");

	for (auto It = GetCanvasManager()->GetIterator_VisibleCanvasByZOrder(); It; ++It)
	{		
		SetRS_AndRenderCanvas(GetDevCon(), It.GetPtr(), GetConfig().VBSlot, GetRenResources());
	}
}

void SpriteRender::RenderCanvas(SpriteCanvasId InCanvasId)
{
	BOOST_ASSERT_MSG(_bRenderingStarted, "SpriteRender::RenderCanvas: must be called inside BeginFrame()/EndFrame pair only.");

	Canvas* pCanvas = GetCanvasManager()->FindById(InCanvasId);
	BOOST_ASSERT_MSG(pCanvas, "SpriteRender::RenderCanvas: The canvas with the given Id must be created");

	SetRS_AndRenderCanvas_IfVisible(GetDevCon(), pCanvas, GetConfig().VBSlot, GetRenResources());
}

void SpriteRender::BeginUpdates()
{
	BOOST_ASSERT_MSG( ! IsUpdating(), "SpriteRender::BeginUpdates: BeginUpdates()/EndUpdates() calls cannot be nested");
	BOOST_ASSERT_MSG( ! _bRenderingStarted, "SpriteRender::BeginUpdates: updating cannot be done while rendering is performed" );

	_bUpdating = true;
}

void SpriteRender::EndUpdates()
{
	BOOST_ASSERT_MSG(IsUpdating(), "SpriteRender::EndUpdates: BeginUpdates() has NOT been called yet");
	BOOST_ASSERT_MSG( ! IsUpdatingCanvas(), "SpriteRender::EndUpdates: none of the canvasses must be in the updating state");

	_bUpdating = false;
}

Ren::ISpriteUpdater* SpriteRender::BeginCanvasUpdate(SpriteCanvasId InCanvasId)
{
	BOOST_ASSERT_MSG(IsUpdating(), "SpriteRender::BeginCanvasUpdate: must be called inside the BeginUpdates()/EndUpdates() pair");
	BOOST_ASSERT_MSG( ! IsUpdatingCanvas(), "SpriteRender::BeginCanvasUpdate: only one canvas may be updated at a time and not nested BeginCanvasUpdate()/EndCanvasUpdate() calls allowed." );

	Canvas* pCanvas = GetCanvasManager()->FindById(InCanvasId);
	BOOST_ASSERT_MSG(pCanvas, "SpriteRender::BeginCanvasUpdate: canvas with the given id must be created");
	
	SSpriteUpdaterInitializer const canvasUpdaterInitializer{ pCanvas };
	auto const pCanvasUpdater = new SpriteUpdater(canvasUpdaterInitializer);
	_pCanvasUpdater.reset(pCanvasUpdater);
	return pCanvasUpdater;
}


void SpriteRender::EndCanvasUpdate(Ren::ISpriteUpdater* pInUpdater)
{
	BOOST_ASSERT_MSG( GetCanvasUpdater() == pInUpdater, "SpriteRender::EndCanvasUpdate: BeginCanvasUpdate() must be called for the given canvas before calling EndCanvasUpdate()" );

	_pCanvasUpdater.reset();
}

void SpriteRender::ShowCanvas(SpriteCanvasId InCanvasId)
{
	GetCanvasManager()->ShowCanvas(InCanvasId);
}

void SpriteRender::HideCanvas(SpriteCanvasId InCanvasId)
{
	GetCanvasManager()->HideCanvas(InCanvasId);
}


void SpriteRender::CreateCanvas(const Ren::SSpriteCanvasCreateCommandInitializer& InInitializer)
{
	BOOST_ASSERT_MSG( ! _bRenderingStarted, "SpriteRender::CreateCanvas: cannot be called during the rendering" );

	SCreateCanvasArgs const CreateCanvasArgs { InInitializer };
	GetCanvasManager()->CreateCanvas(CreateCanvasArgs);
}

void SpriteRender::DeleteCanvas(SpriteCanvasId InCanvasId)
{
	BOOST_ASSERT_MSG( ! _bRenderingStarted, "SpriteRender::DeleteCanvas: cannot be called during the rendering");

	BOOST_ASSERT_MSG( ! GetCanvasUpdater() || GetCanvasUpdater()->GetCanvasId() != InCanvasId, "SpriteRender::DeleteCanvas: we cannot delete canvas because it's in the state of updating" );
	GetCanvasManager()->DeleteCanvas(InCanvasId);
}


void SpriteRender::MoveCanvasZOrderAfter(SpriteCanvasId InCanvasId, SpriteCanvasId InZBeforeCanvasId)
{
	BOOST_ASSERT_MSG( ! _bRenderingStarted, "SpriteRender::MoveCanvasZOrderAfter: cannot be called during the rendering");
	GetCanvasManager()->MoveCanvasZOrderAfter(InCanvasId, InZBeforeCanvasId);
}

std::ofstream& SpriteRender::GetLog()
{
	return _pEnv->GetAmbientContext()->GetLog();
}

AmbientContext* SpriteRender::GetAmbientContext()
{
	return _pEnv->GetAmbientContext();
}

D3D::RenResources* SpriteRender::GetRenResources()
{
	return _pEnv->GetRenResources();
}

ID3D11DeviceContext* SpriteRender::GetDevCon()
{
	return _pEnv->GetRenResources()->GetDevCon();
}

const SConfig& SpriteRender::GetConfig() const
{
	return _pEnv->GetRenResources()->GetConfig();
}


int SpriteRender::GetCapacityInSprites_ForCanvas(SpriteCanvasId InCanvasId) const
{
	Canvas* const pCanvas = GetCanvasManager()->FindById(InCanvasId);
	BOOST_ASSERT_MSG(pCanvas, "SpriteRender::GetCapacityInSprites_ForCanvas: Canvas id must exist");
	return pCanvas->GetCapacityInSprites();
}

int SpriteRender::GetNumSprites_ForCanvas(SpriteCanvasId InCanvasId) const
{
	Canvas* const pCanvas = GetCanvasManager()->FindById(InCanvasId);
	BOOST_ASSERT_MSG(pCanvas, "SpriteRender::GetNumSprites_ForCanvas: Canvas id must exist");
	return pCanvas->GetNumSprites();
}

int SpriteRender::GetNumVisibleSprites_ForCanvas(SpriteCanvasId InCanvasId) const
{
	Canvas* const pCanvas = GetCanvasManager()->FindById(InCanvasId);
	BOOST_ASSERT_MSG(pCanvas, "SpriteRender::GetNumVisibleSprites_ForCanvas: Canvas id must exist");
	return pCanvas->GetNumVisibleSprites();
}


int SpriteRender::GetMaxCanvasLimit() const
{
	return GetCanvasManager()->GetMaxCanvasLimit();
}

int SpriteRender::GetNumCanvasses() const
{
	return GetCanvasManager()->GetNumCanvasses();
}

int SpriteRender::GetNumVisibleCanvasses() const
{
	return GetCanvasManager()->GetNumVisibleCanvasses();
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv