#include "SpriteRender.h"
#include "SpriteUpdater.h"
#include "SpriteRender/IMPL/Canvas/Canvas.h"
#include "SpriteRender/IMPL/Subsystem/SpriteRenderSubsystemEnvironment.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
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

	ID3D11SamplerState* const pSamplerState = pRenResources->GetDefaultSampler();
	pDevCon->VSSetSamplers(0, 1, &pSamplerState);
	pDevCon->PSSetSamplers(0, 1, &pSamplerState);

	ID3D11RenderTargetView* const pRTView = pRenResources->GetConfig().RenderTarget.pRenderTargetView;
	ID3D11DepthStencilView* const pDepthStencilView = pRenResources->GetConfig().RenderTarget.pDepthStencilView;
	pDevCon->OMSetRenderTargets(1, &pRTView, pDepthStencilView);

	//BOOST_ASSERT_MSG(false, "SetupRS: We must setup viewport here");
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.Width = static_cast<float>(pRenResources->GetRTWidth());
	viewport.Height = static_cast<float>(pRenResources->GetRTHeight());
	viewport.TopLeftX = 0.0F;
	viewport.TopLeftY = 0.0F;
	viewport.MinDepth = 0.0F;
	viewport.MaxDepth = pRenResources->GetConfig().RenderTarget.ZFar;
	pDevCon->RSSetViewports(1, &viewport);
}

/**
* Perform caching or non-caching rendering of the canvas based on the config.
*/
void RenderCanvasWithAppropriateMode(Canvas* pCanvas, ID3D11DeviceContext* pDevCon, const SConfig& InConfig)
{
	if (InConfig.RenderCaching.bUseCommandListToCacheRendering)
	{
		pCanvas->RenderAndCache();
		BOOST_ASSERT(pCanvas->IsRenderResultValid());
		pDevCon->ExecuteCommandList(pCanvas->GetResult().GetCommandList(), true);
	}
	else
	{
		pCanvas->Render(pDevCon);
	}
}

/**
* Commits changes to the sprite geometry VB
* and notifies the canvas if buffer the updated.
*/
void CommitChangesToSpriteGeometryVB_AndNotifyCanvas(ID3D11DeviceContext* pInDevCon, SpriteGeometryBuffer* pSpriteGeometryVB, Canvas* pCanvas)
{
	pSpriteGeometryVB->CommitDeletesAndUpdates(pInDevCon);
	pSpriteGeometryVB->CommitAdded(pInDevCon);

	ID3D11Buffer* pOldVB = pSpriteGeometryVB->GetOldBuffer();
	ID3D11Buffer* pVB = pSpriteGeometryVB->GetBuffer();
	if (pOldVB != pVB)
	{
		pCanvas->MarkVertexBufferResetted(pOldVB, pVB);
	}
}

SpriteRender::~SpriteRender()
{
	// We use default destructor implementation
}

SpriteRender::SpriteRender(Environment* pInEnv) :
	_pEnv(pInEnv)
,   _bRenderingStarted(false)
,	_bUpdating(false)
{
	BOOST_ASSERT(_pEnv);
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

	/**
	* To be called after all rendering calls are done.
	*/
	//BOOST_ASSERT_MSG(false, "SpriteRender::BeginFrame: NOT yet implemented");
}

void SpriteRender::RenderAllVisibleCanvasses()
{
	BOOST_ASSERT_MSG(_bRenderingStarted, "SpriteRender::RenderAllVisibleCanvasses: must be called inside BeginFrame()/EndFrame pair only.");

	if (_pCanvas && _pCanvas->IsVisible())
	{
		RenderCanvasWithAppropriateMode(_pCanvas.get(), GetDevCon(), GetConfig());
	}
}

void SpriteRender::RenderCanvas(SpriteCanvasId InCanvasId)
{
	BOOST_ASSERT_MSG(_bRenderingStarted, "SpriteRender::RenderCanvas: must be called inside BeginFrame()/EndFrame pair only.");

	BOOST_ASSERT_MSG(_pCanvas && _pCanvas->GetId() == InCanvasId, "SpriteRender::RenderCanvas: The canvas with the given Id must be created");

	RenderCanvasWithAppropriateMode(_pCanvas.get(), GetDevCon(), GetConfig());
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

	CommitChangesToSpriteGeometryVB_AndNotifyCanvas(GetDevCon(), GetSpriteGeometryBuffer(), _pCanvas.get());
}

ISpriteUpdater* SpriteRender::BeginCanvasUpdate(SpriteCanvasId InCanvasId)
{
	BOOST_ASSERT_MSG(IsUpdating(), "SpriteRender::BeginCanvasUpdate: must be called inside the BeginUpdates()/EndUpdates() pair");
	BOOST_ASSERT_MSG( ! IsUpdatingCanvas(), "SpriteRender::BeginCanvasUpdate: only one canvas may be updated at a time and not nested BeginCanvasUpdate()/EndCanvasUpdate() calls allowed." );
	BOOST_ASSERT_MSG( _pCanvas && (InCanvasId == _pCanvas->GetId()), "SpriteRender::BeginCanvasUpdate: canvas with the given id must be created" );

	SSpriteUpdaterInitializer const canvasUpdaterInitializer { _pCanvas.get() };
	auto const pCanvasUpdater = new SpriteUpdater(canvasUpdaterInitializer);
	_pCanvasUpdater.reset(pCanvasUpdater);
	return pCanvasUpdater;
}


void SpriteRender::EndCanvasUpdate(ISpriteUpdater* pInUpdater)
{
	BOOST_ASSERT_MSG( GetCanvasUpdater() == pInUpdater, "SpriteRender::EndCanvasUpdate: BeginCanvasUpdate() must be called for the given canvas before calling EndCanvasUpdate()" );

	_pCanvasUpdater.reset();
}

void SpriteRender::ShowCanvas(SpriteCanvasId InCanvasId)
{
	BOOST_ASSERT_MSG(_pCanvas && _pCanvas->GetId() == InCanvasId, "SpriteRender::ShowCanvas: The canvas with the given Id must be created");
	_pCanvas->Show();
}

void SpriteRender::HideCanvas(SpriteCanvasId InCanvasId)
{
	BOOST_ASSERT_MSG(_pCanvas && _pCanvas->GetId() == InCanvasId, "SpriteRender::HideCanvas: The canvas with the given Id must be created");
	_pCanvas->Hide();
}


void SpriteRender::CreateCanvas(const SSpriteCanvasCreateCommandInitializer& InInitializer)
{
	BOOST_ASSERT_MSG( ! _bRenderingStarted, "SpriteRender::CreateCanvas: cannot be called during the rendering" );
	BOOST_ASSERT_MSG( _pCanvas.get() == nullptr, "SpriteRender::CreateCanvas: only one canvas is supported now" );

	SCreateCanvasArgs const createCanvasArgs { InInitializer };
	SCanvasInitializer const canvasInitializer { GetAmbientContext(), createCanvasArgs, GetSpriteGeometryBuffer(), GetRenResources() };
	auto pNewCanvas = new Canvas(canvasInitializer);
	_pCanvas.reset(pNewCanvas);
}

void SpriteRender::DeleteCanvas(SpriteCanvasId InCanvasId)
{
	BOOST_ASSERT_MSG( ! _bRenderingStarted, "SpriteRender::DeleteCanvas: cannot be called during the rendering");
	BOOST_ASSERT_MSG( ! GetCanvasUpdater() || GetCanvasUpdater()->GetCanvasId() != InCanvasId, "SpriteRender::DeleteCanvas: we cannot delete canvas because it's in the state of updating" );
	BOOST_ASSERT_MSG(_pCanvas && _pCanvas->GetId() == InCanvasId, "SpriteRender::DeleteCanvas: the canvas with the given id must be created");

	_pCanvas.reset();
}


void SpriteRender::MoveCanvasZOrderAfter(SpriteCanvasId InCanvasId, SpriteCanvasId InZBeforeCanvasId)
{
	BOOST_ASSERT_MSG( ! _bRenderingStarted, "SpriteRender::MoveCanvasZOrderAfter: cannot be called during the rendering");
	BOOST_ASSERT_MSG(_pCanvas && _pCanvas->GetId() == InCanvasId, "SpriteRender::MoveCanvasZOrderAfter: canvas with the given id must be created");
	BOOST_ASSERT_MSG(InZBeforeCanvasId == NULL_SPRITE_CANVAS_ID, "SpriteRender::MoveCanvasZOrderAfter: only once canvas is supported, so this method never should be called except with the \"move on top\" arguments");

	// Nothing is to be done here.
}

std::ofstream& SpriteRender::GetLog()
{
	return _pEnv->GetAmbientContext()->GetLog();
}

AmbientContext* SpriteRender::GetAmbientContext()
{
	return _pEnv->GetAmbientContext();
}

SpriteGeometryBuffer* SpriteRender::GetSpriteGeometryBuffer()
{
	return _pEnv->GetSpriteGeometryBuffer();
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

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv