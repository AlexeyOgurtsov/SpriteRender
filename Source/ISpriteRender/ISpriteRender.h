#pragma once

#include "ISpriteRender/ISpriteUpdater.h"
#include "ISprite/SpriteCanvasTypedefs.h"
#include "SpriteCanvasCommandInitializers.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{

SSpriteCanvasCreateCommandInitializer GetCanvasInit
(
	bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName,
	const SSpriteCanvasProps& InCanvasProps, 
	int InInitialCapacityInSprites = DEFAULT_CAPACITY_IN_SPRITES, bool bInAutoResize  = false
);
SSpriteCanvasCreateCommandInitializer GetCanvasInit
(
	bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName, 
	const SCanvasRect& InRect, const SCanvasCoordSystem& InCoordSystem = SCanvasCoordSystem(), 
	int InInitialCapacityInSprites = DEFAULT_CAPACITY_IN_SPRITES, bool bInAutoResize = false
);
SSpriteCanvasCreateCommandInitializer GetCanvasInit
(
	bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName,
	int InWidth, int InHeight, int InLeft = 0, int InTop = 0, 
	const SCanvasCoordSystem& InCoordSystem = SCanvasCoordSystem(), 
	int InInitialCapacityInSprites = DEFAULT_CAPACITY_IN_SPRITES, bool bInAutoResize = false
);

class ISpriteRender
{
public:
	virtual ~ISpriteRender() = default;

	// ~ Helper interface Begin
	/**
	* Registers a canvas (helper).
	* Calling during rendering exhibits an undefined behaviour.
	*/
	void CreateCanvas(bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName, const SSpriteCanvasProps& InCanvasProps, int InInitialCapacityInSprites = DEFAULT_CAPACITY_IN_SPRITES, bool bInAutoResize = false);

	/**
	* Registers a canvas (helper).
	* Calling during rendering exhibits an undefined behaviour.
	*/
	void CreateCanvas(bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName,  const SCanvasRect& InRect, const SCanvasCoordSystem& InCoordSystem = SCanvasCoordSystem(), int InInitialCapacityInSprites = DEFAULT_CAPACITY_IN_SPRITES, bool bInAutoResize = false);

	/**
	* Registers a canvas (helper).
	* Calling during rendering exhibits an undefined behaviour.
	*/
	void CreateCanvas(bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName, int InWidth, int InHeight, int InLeft = 0, int InTop = 0, const SCanvasCoordSystem& InCoordSystem = SCanvasCoordSystem(), int InInitialCapacityInSprites = DEFAULT_CAPACITY_IN_SPRITES, bool bInAutoResize = false);
	// ~ Helper interface End
	

	// ~ Virtual interface Begin
	/**
	* To be called before any calls to rendering operations,
	* but after the update operations.
	*/
	virtual void BeginFrame() = 0;

	/**
	* To be called after all rendering calls are done.
	*/
	virtual void EndFrame() = 0;

	/**
	* Renders all canvasses that are visible now.
	*/
	virtual void RenderAllVisibleCanvasses() = 0;

	/**
	* Always Renders the particular canvas, independent of whether it's visible or not.
	*/
	virtual void RenderCanvas(SpriteCanvasId InCanvasId) = 0;

	/**
	* To be called before performing any update calls.
	* WARNING!!! The render should NOT be in the render state.
	*/
	virtual void BeginUpdates() = 0;

	/**
	* To be called after all updates have been done.
	*/
	virtual void EndUpdates() = 0;

	/**
	* Starts to update the given canvas.
	* WARNING!!! Only one canvas can be updated at a time 
	* (i.e. no overlapping BeginCanvasUpdate/EndCanvasUpdate for different canvasses allowed).
	*
	* @returns: pointer to the updater interface for the given canvas.
	*/
	virtual ISpriteUpdater* BeginCanvasUpdate(SpriteCanvasId InCanvasId) = 0;

	/**
	* Ends canvas update.
	* Invalidates the canvas updater pointer.
	*
	* @param pInUpdater: updater to stop updating the canvass.
	*/
	virtual void EndCanvasUpdate(ISpriteUpdater* pInUpdater) = 0;

	/**
	* Makes the given canvas visible.
	* Can be called ever during the update and during the rendering.
	*/
	virtual void ShowCanvas(SpriteCanvasId InCanvasId) = 0;

	/**
	* Makes the given canvas hidden.
	* Can be called ever during the update and during the rendering.
	*/
	virtual void HideCanvas(SpriteCanvasId InCanvasId) = 0;

	/**
	* Registers a canvas.
	* Calling during rendering exhibits an undefined behaviour.
	*/
	virtual void CreateCanvas(const SSpriteCanvasCreateCommandInitializer& InInitializer) = 0;

	/**
	* Deletes a canvas.
	* Calling during rendering exhibits an undefined behaviour.
	*/
	virtual void DeleteCanvas(SpriteCanvasId InCanvasId) = 0;

	/**
	* Moves the canvas right after the given one in the ZOrder.
	* Calling during rendering the rendering exhibits an undefined behaviour.
	*
	* @argument InCanvasId: already registered canvas id of the canvas to be moved.
	* @argument InZBeforeSpriteId: already registered canvas id of the sprite, that must preceed the given canvas in the ZOrder
	* (if null, the canvas is to be moved on top of the ZOrder).
	*/
	virtual void MoveCanvasZOrderAfter(SpriteCanvasId InCanvasId, SpriteCanvasId InZBeforeCanvasId) = 0;

	/**
	* Capacity of the canvas in sprites.
	*/
	virtual int GetCapacityInSprites_ForCanvas(SpriteCanvasId InCanvasId) const = 0;

	/**
	* Total number of sprites (both visible and hidden) for the given canvas.
	*/
	virtual int GetNumSprites_ForCanvas(SpriteCanvasId InCanvasId) const = 0;

	/**
	* Number of visible sprites for the given canvas
	*/
	virtual int GetNumVisibleSprites_ForCanvas(SpriteCanvasId InCanvasId) const = 0;

	/**
	* Max count of canvasses.
	*/
	virtual int GetMaxCanvasLimit() const = 0;

	/**
	* Count of visible canvasses
	*/
	virtual int GetNumCanvasses() const = 0;

	/**
	* Count of visible canvasses
	*/
	virtual int GetNumVisibleCanvasses() const = 0;
	// ~ Virtual interface End

	/**
	* Count of hidden canvasses
	*/
	int GetNumHiddenCanvasses() const;

	/**
	* Number of hidden sprites for the given canvas
	*/
	int GetNumHiddenSprites_ForCanvas(SpriteCanvasId InCanvasId) const;	
};

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv