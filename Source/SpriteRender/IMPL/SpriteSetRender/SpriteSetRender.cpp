#include "SpriteSetRender.h"
#include "SpriteRender/IMPL/Sprite/SpriteManager.h"
#include "SpriteSetRenderUtils.h"
#include "SpriteRender/IMPL/RenUtils/RenDeviceContextUtils.h"
#include "SpriteRender/IMPL/RenUtils/RenCommandListUtils.h"
#include "SpriteRender/IMPL/RenUtils/RenResources.h" // to get the d3d11 device
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

bool SpriteSetRender::IsRenderResultValid() const
{
	// WARNING!!! Testing for the render result relevance must be done 
	// - see the comments on the variables in the expression.
	return _bRenderStateCanBeUseAsIs_IfRelevant && _renderResult.IsRelevant();
}

SpriteSetRender::SpriteSetRender(const SSpriteSetRenderInitializer& InInitializer) :
	_pSpriteManager(InInitializer.pSpriteManager)
,	_pRenResources(InInitializer.pRenResources)
,	_bRenderStateCanBeUseAsIs_IfRelevant(false)
{
	BOOST_ASSERT(_pSpriteManager);
	BOOST_ASSERT(_pRenResources);
}

void SpriteSetRender::RenderAndCache()
{
	if (IsRenderResultValid()) { return; }
	
	D3D::DeviceContextHandle pDeferredDevCon = D3D::CreateDeferredContext(GetDev());
	Render(pDeferredDevCon.get());
	D3D::CommandListHandle pCommandList = D3D::FinishCommandList(pDeferredDevCon.get());
	_CacheResult(std::move(pCommandList));
}

/**
* Update the result of the cache and mark the update committed.
*/
void SpriteSetRender::_CacheResult(D3D::CommandListHandle&& pInCommandList)
{
	_renderResult = std::move(SpriteSetRenderResult{ std::move(pInCommandList) });
	_MarkUpdateCached();
}

void SpriteSetRender::Render(ID3D11DeviceContext* pInDevCon)
{
	if (_renderResult.IsRelevant())
	{
		// We must restore the render state to the state right before the command list executed,
		// because we have a common pipeline state already set for all sprites in this sprite set.
		const bool bRestoreCommandState = true;
		pInDevCon->ExecuteCommandList(_renderResult.GetCommandList(), bRestoreCommandState);

		_RenderNewZOrderTopSprites(pInDevCon);
	}
	else
	{
		_RenderAllVisibleSprites(pInDevCon);
	}
}

void SpriteSetRender::MarkSpriteCreated(const Sprite* pInSprite)
{
	if (pInSprite->IsVisible())
	{
		MarkSpriteShown(pInSprite);
	}
}

void SpriteSetRender::_MarkUpdateCached()
{
	_newZOrderTopSprites.clear();
	_bRenderStateCanBeUseAsIs_IfRelevant = true;
}

void SpriteSetRender::MarkSpritePendingDelete(const Sprite* pInSprite)
{
	if (pInSprite->IsVisible())
	{
		MarkSpriteHidden(pInSprite);
	}
}

void SpriteSetRender::MarkSpriteRenderStateChanged(const Sprite* pInSprite)
{
	_InvalidateRenderResult();
}

void SpriteSetRender::MarkSpriteZOrderChanged(const Sprite* pInSprite)
{
	_InvalidateRenderResult();
}

void SpriteSetRender::MarkSpriteShown(const Sprite* pInSprite)
{
	_bRenderStateCanBeUseAsIs_IfRelevant = false;
	bool bSpriteOnTopOfZOrder = IsOnTopOfVisible(pInSprite);
	if (bSpriteOnTopOfZOrder)
	{
		_newZOrderTopSprites.push_back(pInSprite);
	}
	else
	{
		_renderResult.Invalidate();
	}
}

void SpriteSetRender::MarkSpriteHidden(const Sprite* pInSprite)
{
	_InvalidateRenderResult();
}

void SpriteSetRender::MarkVertexBufferResetted(ID3D11Buffer* pInOldBuffer, ID3D11Buffer* pInNewBuffer)
{
	_InvalidateRenderResult();
}

void SpriteSetRender::_InvalidateRenderResult()
{
	_renderResult.Invalidate();
	// HINT: Really resetting the "render-state-can-be-used-as-is" flag is redundant
	// because we always check this flag together with checking the render result relevancy. 
	_bRenderStateCanBeUseAsIs_IfRelevant = false;
}

/**
* Renders every sprite that is registered in the set of the new Z Order top  sprites.
* Performs rendering in the Z Order (from back to front).
*/
void SpriteSetRender::_RenderNewZOrderTopSprites(ID3D11DeviceContext* pInDevCon)
{
	for (const Sprite* pSprite : _newZOrderTopSprites)
	{
		RenderSprite(pInDevCon, pSprite, _pRenResources);
	}
}

/**
* Renders every visible sprite registered in the sprite manager.
* Performs rendering in the Z Order (from back to front).
*/
void SpriteSetRender::_RenderAllVisibleSprites(ID3D11DeviceContext* pInDevCon)
{
	for (auto it = _pSpriteManager->GetVisibleSpriteIterator_Begin(); it != _pSpriteManager->GetVisibleSpriteIterator_End(); ++it)
	{
		RenderSprite(pInDevCon, *it, _pRenResources);
	}
}

ID3D11Device* SpriteSetRender::GetDev() const
{
	return _pRenResources->GetDev();
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv