#pragma once

#include "SpriteSetRender.h"
#include "SpriteManager.h"
#include "ISprite/SpriteCanvasTypedefs.h"
#include "ISpriteRender/SpriteCommandInitializers.h"
#include "CanvasCommands.h"
#include "../Render/CanvasManagerContainerTypes.h"
#include <memory>
#include <d3d11.h>
#include <optional>

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
* Caches precalculated values connected with the canvas coord system (like, matrix, for example), recalculates them.
*/
class CanvasCoordSystemManager
{
public:	
	CanvasCoordSystemManager(const SCanvasCoordSystem& InCoordSystem, float InAspectWidthOverHeight);	

	const SCanvasCoordSystem& GetCoordSystem() const { return CoordSystem; }
	void UpdateCoordSystem(const SCanvasCoordSystem& InCoordSystem);	

	float GetAspectWidthOverHeight() const { return AspectWidthOverHeight; }
	void UpdateAspectWidthOverHeight(float InAspectWidthOverHeight);

	float GetWidth() const;
	float GetHeight() const;	
	float GetLeftX() const;
	float GetRightX() const;
	float GetBottomY() const;
	float GetTopY() const;

	float GetZNear() const;
	float GetZFar() const;

	const float* GetMatrix() const { return &Matrix[0][0]; }

private:
	void RecalculateMatrix();

	float AspectWidthOverHeight;
	SCanvasCoordSystem CoordSystem;
	float Matrix[4][4];
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
	* Iterator in the canvas container.
	*/
	const CanvasIterator& GetIterator() const { return Iterator; }

	/**
	* Binds new canvas storage iterator
	*/
	void BindStorageIterator(CanvasStorage::iterator InIt);

	/**
	* Binds iterator to the list by Z-order.
	*/
	void BindZOrderIterator(CanvasList::iterator InIt);

	/**
	* Unique id of the canvas.
	*/
	SpriteCanvasId GetId() const { return _id; }

	/**
	* Should the canvas be rendered by default.
	*/
	bool IsVisible() const { return _bVisible; }

	/**
	* Opposite to IsVisible.
	*/
	bool IsHidden() const;

	/**
	* Sets visibility variable (to be called only from the CanvasManager!)
	*/
	void SetVisibility(bool bInVisible);

	/**
	* Returns all properties of the canvas as one struct.
	*/
	const SRenderLayerCanvasProps& GetProps() const { return _props; }

	/**
	* Helper getter that returns the render target rect that the canvas occupies.
	*/
	const SRenderLayerCanvasRect& GetRect() const { return _props.RTRect; }

	/**
	* Updates the rect on the render target that the canvas occupies.
	*/
	void UpdateRect(const SRenderLayerCanvasRect& InNewRect);

	/**
	* Canvas coord system.
	*/
	const SCanvasCoordSystem& GetCoordSystem() const { return _coordSystem.GetCoordSystem(); }

	/**
	* Updates the coord system of the canvas
	*/
	void UpdateCoordSystem(const SCanvasCoordSystem& InCoordSystem);

	/**
	* Makes the given canvas visible.
	*/
	void Show();

	/**
	* Makes the given canvas hidden.
	*/
	void Hide();

	/**
	* Capacity in sprites.
	*/
	int GetCapacityInSprites() const;

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
	void UpdateRenderCoordSystem();

	AmbientContext* _pAmbientContext;
	SpriteCanvasId _id;
	bool _bVisible;
	SRenderLayerCanvasProps _props;
	CanvasCoordSystemManager _coordSystem;
	std::unique_ptr<SpriteManager> pSprites;
	std::unique_ptr<SpriteSetRender> pRender;
	CanvasIterator Iterator;
};

} // Dv::Spr::QRen::IMPL

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv