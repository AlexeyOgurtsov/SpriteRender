#pragma once

#include "SpriteRender/IMPL/SpriteSetRender/SpriteSetRender.h"
#include "SpriteRender/IMPL/Sprite/SpriteManager.h"
#include "ISprite/SpriteCanvasTypedefs.h"
#include "CanvasCommands.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class AmbientContext;
class ISpriteGeometryBufferUpdateInterface;
class SpriteManager;

namespace D3D
{
	class RenResources;
}

struct SCanvasInitializer
{
	SCreateCanvasArgs CreateArgs;
	AmbientContext* pAmbientContext;
	ISpriteGeometryBufferUpdateInterface* pSpriteBuffer;
	const D3D::RenResources* pRenResources;

	SCanvasInitializer(AmbientContext* pInAmbientContext, const SCreateCanvasArgs& InCreateArgs, ISpriteGeometryBufferUpdateInterface* pInSpriteBuffer, const D3D::RenResources* pInRenResources) :
		CreateArgs(InCreateArgs)
	,	pAmbientContext(pInAmbientContext)
	,	pSpriteBuffer(pInSpriteBuffer)
	,	pRenResources(pInRenResources) {}
};

/**
* Manages the sprites canvas entirely:
* - Controls canvas properties;
* - Manages sprites;
* - Performs rendering of the set of the sprites;
*/
class Canvas
{
public:
	Canvas(const SCanvasInitializer& InInitializer);

	/**
	* Unique id of the canvas.
	*/
	SpriteCanvasId GetId() const { return _id; }

	/**
	* Should the canvas be rendered by default.
	*/
	bool IsVisible() const { return _bVisible; }

	/**
	* Returns all properties of the canvas as one struct.
	*/
	const SRenderLayerCanvasProps& GetProps() const { return _props; }

	/**
	* Helper getter that returns the render target rect that canvas occupies.
	*/
	const SRenderLayerCanvasRect& GetRect() const { return _props.RTRect; }

	/**
	* Updates the rect on the render target that the canvas occupies.
	*/
	void UpdateRect(const SRenderLayerCanvasRect& InNewRect);

	/**
	* Makes the given canvas visible.
	*/
	void Show();

	/**
	* Makes the given canvas hidden.
	*/
	void Hide();

	/**
	* Returns count of sprites in the visible state.
	*/
	int GetNumVisibleSprites() const { return _sprites.GetNumVisibleSprites(); }

	/**
	* Returns total count of sprites (both visible and hidden).
	*/
	int GetNumSprites() const { return _sprites.GetNumSprites(); }

	/**
	* Is old cached render result valid and be used as-is the next call to the render function.
	*/
	bool IsRenderResultValid() const { return _render.IsRenderResultValid(); }

	/**
	* Result of the last rendering.
	* WARNING!!! Only caching rendering operations update the last stored result.
	*/
	const SpriteSetRenderResult& GetResult() const { return _render.GetRenderResult(); }

	/**
	* Performs rendering of all visible sprites in the set and store the result.
	* To be called to make the render result reflect the changes made 
	* by any of the sprite set update functions.
	*
	* ATTENTION! Performs rendering ever if canvas is in the invisible state.  
	*/
	void RenderAndCache();

	/**
	* Renders the same as the caching rendering function, but uses the given device context 
	* and never caches the rendering result.
	* However, the existing cache must be either valid or invalidated (i.e. contain empty data),
	* because the render function may use the existing cache to build the new result incrementally.
	*
	* @see: RenderAndCache
	*/
	void Render(ID3D11DeviceContext* pInDevCon);

	/**
	* Must be called when a d3d11 buffer is destroyed, and a new buffer is to be used instead of it.
	* Sprite set state update function (@see RenderAndCache).
	*/
	void MarkVertexBufferResetted(ID3D11Buffer* pInOldBuffer, ID3D11Buffer* pInNewBuffer);

	/**
	* Creates a new sprite.
	* Sprite set state update function (@see RenderAndCache).
	*
	* The sprite with the given id must NOT be registered yet.
	*/
	void CreateSprite(const SCreateSpriteArgs& InArgs);

	/**
	* Immediately deletes all data, related to the given sprite and invalidates the sprite id.
	* Sprite set state update function (@see RenderAndCache).
	*
	* The given id must be valid.
	*/
	void DeleteSprite(SpriteId InId);

	/**
	* Makes the given sprite visible.
	* Sprite set state update function (@see RenderAndCache).
	*
	* ATTENTION! The sprite must be invisible right before the call.
	*/
	void ShowSprite(SpriteId InId);

	/**
	* Makes the given sprite hidden.
	* Sprite set state update function (@see RenderAndCache).
	*
	* ATTENTION! The sprite must be visible right before the call.
	*/
	void HideSprite(SpriteId InId);	

	/**
	* Changes ZOrder of the sprite to immediately follow right after the other one.
	* Sprite set state update function (@see RenderAndCache).
	*
	* @argument InId: id of the sprite (must be valid);
	* @argument InZBeforeSpriteId:
	* Valid id of the sprite that is to preceed the given sprite in the ZOrder
	* (if NULL_SPRITE_ID, then the sprite is moved to the top of the ZOrder); 
	*/
	void MoveSpriteZOrderAfter(SpriteId InId, SpriteId InZBeforeSpriteId);

	/**
	* Sets a new value of the transparency for the sprite.
	* Sprite set state update function (@see RenderAndCache).
	*/
	void SetSpriteTransparency(SpriteId InId, ESpriteTransparency InTransparency);

	/**
	* Sets a new material instance for the sprite.
	* Sprite set state update function (@see RenderAndCache).
	*/
	void SetMaterialInstance(SpriteId InId, MaterialInstanceRenderStateHandle pInRenderState);

	/**
	* Sets a new geometry props for the sprite.
	* Sprite set state update function (@see RenderAndCache).
	*/
	void SetSpriteGeometry(SpriteId InId, const SSpriteGeometryData& InGeometry);

private:
	AmbientContext* _pAmbientContext;
	SpriteCanvasId _id;
	bool _bVisible;
	SRenderLayerCanvasProps _props;
	SpriteManager _sprites;
	SpriteSetRender _render;
};

} // Dv::Spr::Ren::IMPL

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv