#pragma once

#include "SpriteSetRender.h"
#include "SpriteManager.h"
#include "ISprite/SpriteCanvasTypedefs.h"
#include "ISpriteRender/SpriteCommandInitializers.h"
#include "CanvasCommands.h"
#include <memory>
#include <d3d11.h>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

class AmbientContext;
class SpriteManager;

namespace D3D
{
	class RenResources;
}

struct SCanvasInitializer
{
	SCreateCanvasArgs CreateArgs;
	AmbientContext* pAmbientContext;
	const D3D::RenResources* pRenResources;

	bool bDebug = false;
	UINT MaxSprites = 100;
	bool bAutoResizeable = false;

	SCanvasInitializer(AmbientContext* pInAmbientContext, const SCreateCanvasArgs& InCreateArgs, const D3D::RenResources* pInRenResources) :
		CreateArgs(InCreateArgs)
	,	pAmbientContext(pInAmbientContext)
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
	int GetNumVisibleSprites() const { return pSprites->GetNumVisibleSprites(); }

	/**
	* Returns total count of sprites (both visible and hidden).
	*/
	int GetNumSprites() const { return pSprites->GetNumSprites(); }

	/**
	* Flushes D3D buffers to perform rendering.
	*/
	void FlushD3D();

	/**
	* Issues DrawCall and RS-change commands to render the given canvas.
	*
	* WARNING!!! The common render state must already be set
	*/
	void Render(ID3D11DeviceContext* pInDevCon, UINT InVBSlot);


	/**
	* Creates a new sprite.
	*
	* The sprite with the given id must NOT be registered yet.
	*/
	void CreateSprite(const SCreateSpriteArgs& InArgs);

	/**
	* Immediately deletes all data, related to the given sprite and invalidates the sprite id.
	*
	* The given id must be valid.
	*/
	void DeleteSprite(SpriteId InId);

	/**
	* Makes the given sprite visible.
	*
	* ATTENTION! The sprite must be invisible right before the call.
	*/
	void ShowSprite(SpriteId InId);

	/**
	* Makes the given sprite hidden.
	*
	* ATTENTION! The sprite must be visible right before the call.
	*/
	void HideSprite(SpriteId InId);

	/**
	* Changes ZOrder of the sprite to immediately follow right after the other one.
	*
	* @argument InId: id of the sprite (must be valid);
	* @argument InZBeforeSpriteId:
	* Valid id of the sprite that is to preceed the given sprite in the ZOrder
	* (if NULL_SPRITE_ID, then the sprite is moved to the top of the ZOrder); 
	*/
	void MoveSpriteZOrderAfter(SpriteId InId, SpriteId InZBeforeSpriteId);

	/**
	* Sets a new value of the transparency for the sprite.
	*/
	void SetSpriteTransparency(SpriteId InId, ESpriteTransparency InTransparency);

	/**
	* Sets a new material instance for the sprite.
	*/
	void SetMaterialInstance(SpriteId InId, MaterialInstanceRenderStateHandle pInRenderState);

	/**
	* Sets a new geometry props for the sprite.
	*/
	void SetSpriteGeometry(SpriteId InId, const SSpriteGeometryData& InGeometry);

private:
	AmbientContext* _pAmbientContext;
	SpriteCanvasId _id;
	bool _bVisible;
	SRenderLayerCanvasProps _props;
	std::unique_ptr<SpriteManager> pSprites;
	std::unique_ptr<SpriteSetRender> pRender;
};

} // Dv::Spr::QRen::IMPL

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv