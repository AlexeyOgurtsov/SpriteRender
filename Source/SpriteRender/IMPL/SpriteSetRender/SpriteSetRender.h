#pragma once

#include "SpriteSetRenderResult.h"
#include <vector>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class Sprite;
class SpriteManager;

namespace D3D
{
	class RenResources;
}

struct SSpriteSetRenderInitializer
{
	const SpriteManager* pSpriteManager;
	const D3D::RenResources* pRenResources;

	SSpriteSetRenderInitializer(const SpriteManager* pInSpriteManager, const D3D::RenResources* pInRenResources) :
		pSpriteManager(pInSpriteManager)
	,	pRenResources(pInRenResources) {}
};

/**
* Renders a set of sprites using the given d3d11 device or by caching into the internal command list.
*/
class SpriteSetRender
{
public:
	SpriteSetRender(const SSpriteSetRenderInitializer& InInitializer);

	/**
	* Is old cached render result valid and be used as-is the next call to the render function.
	*/
	bool IsRenderResultValid() const;

	/**
	* Last stored result.
	* WARNING!!! Only caching rendering operations update the last stored result.
	*/
	const SpriteSetRenderResult& GetRenderResult() const { return _renderResult; }

	/**
	* Performs rendering of all visible sprites in the set and store the result.
	* After renders, sets of sprites regarded as updated, created, deleted are cleared. 
	*/
	void RenderAndCache();

	/**
	* Performs rendering using the given device context.
	* Never updates cache.
	* However, the existing cache must be either valid or invalidated (i.e. contain empty data),
	* because the render function may use the existing cache to build the new result incrementally.
	*/
	void Render(ID3D11DeviceContext* pInDevCon);

	/**
	* Notifies that a new sprite is created.
	*/
	void MarkSpriteCreated(const Sprite* pInSprite);

	/**
	* Notifies that a given sprite is to be deleted soon.
	*/
	void MarkSpritePendingDelete(const Sprite* pInSprite);

	/**
	* Notifies that a given sprite changed its rendering state (i.e. material state or transparency settings).
	* Should NOT be called when geometry of the sprite is changed.
	*/
	void MarkSpriteRenderStateChanged(const Sprite* pInSprite);

	/**
	* Notifies that ZOrder of the sprite has been changed.
	*/
	void MarkSpriteZOrderChanged(const Sprite* pInSprite);

	/**
	* Notifies that the given sprite has been shown.
	* WARNING!!! Must be called only if the sprite was invisible before.
	*/
	void MarkSpriteShown(const Sprite* pInSprite);

	/**
	* Notifies that the given sprite has been hidden.
	* WARNING!!! Must be called only if the sprite was visible before.
	*/
	void MarkSpriteHidden(const Sprite* pInSprite);

	/**
	* Notifies that a given sprite buffer has been substituted with a new one.
	*/	
	void MarkVertexBufferResetted(ID3D11Buffer* pInOldBuffer, ID3D11Buffer* pInNewBuffer);

private:
	void _CacheResult(D3D::CommandListHandle&& pInCommandList);
	void _MarkUpdateCached();
	void _InvalidateRenderResult();

	void _RenderNewZOrderTopSprites(ID3D11DeviceContext* pInDevCon);

	void _RenderAllVisibleSprites(ID3D11DeviceContext* pInDevCon);

	using OrderSpriteUpdateList = std::vector<const Sprite*>;

	const SpriteManager* _pSpriteManager;
	const D3D::RenResources* _pRenResources;

	ID3D11Device* GetDev() const;

	/**
	* List of sprites, that are added on the top of the Z Order, but NOT yet cached.
	* This sprites must be sorted according to the Z Order.
	*/
	OrderSpriteUpdateList _newZOrderTopSprites;

	/**
	* True if the render result can be used as-is.
	* WARNING!!! This variable is relevant ONLY if the result cache IS relevant (i.e. NOT invalidated).
	* Never should be checked directly, the IsRenderResultValid() getter is to be used instead.
	*/
	bool _bRenderStateCanBeUseAsIs_IfRelevant;

	/**
	* Result of the last rendering.
	* Ever if it's relevant, it's still may need to be calculated up.
	* But relevant result can be used to render up incrementally, using the old result,
	* while the irrelevant result cannot be used to build a new result.
	*/
	SpriteSetRenderResult _renderResult;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv