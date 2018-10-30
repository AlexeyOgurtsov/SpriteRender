#pragma once

#include "ISprite/SpriteCanvasProps.h"
#include "ISprite/SpriteCanvasTypedefs.h"
#include <string>

namespace Dv
{
namespace Spr
{
namespace Ren
{

/**
* Base class that all sprite canvas command initializers should derive from.
*/
struct SSpriteCanvasCommandInitializerBase
{
	/**
	* Target sprite canvas that this command is related to.
	*/
	SpriteCanvasId TargetSpriteCanvasId = NULL_SPRITE_CANVAS_ID;

	SSpriteCanvasCommandInitializerBase() :
		TargetSpriteCanvasId(NULL_SPRITE_CANVAS_ID) {}
	explicit SSpriteCanvasCommandInitializerBase(SpriteCanvasId InTargetSpriteCanvasId) :
		TargetSpriteCanvasId(InTargetSpriteCanvasId) {}
};

/**
* @see: SSpriteCanvasCreateCommandInitializer
*/
constexpr int DEFAULT_CAPACITY_IN_SPRITES = 0;

/**
* @see: SSpriteCanvasCreateCommandInitializer
*/
const std::string DEFAULT_CANVAS_NAME = "{UNNAMED}";

/**
* Initializer of the create command
*/
struct SSpriteCanvasCreateCommandInitializer : public SSpriteCanvasCommandInitializerBase
{
	/**
	* Fully describes all properties of the canvas.
	*/
	SSpriteCanvasProps Props;

	/**
	* Z Order of the sprite canvas.
	* If null, then the sprite canvas is added on the top.
	*/
	SpriteCanvasId ZBeforeSpriteCanvasId = NULL_SPRITE_CANVAS_ID;

	/**
	* Name of the canvas
	*/
	std::string Name = DEFAULT_CANVAS_NAME;

	/**
	* Initial capacity of this canvas in sprites.
	*
	* If zero (DEFAULT), then corresponds to the system default.
	*/
	int InitialCapacityInSprites = DEFAULT_CAPACITY_IN_SPRITES;

	/**
	* Can be automatically resized?
	*/
	bool bAutoResize = false;

	/**
	* Should the sprite canvas to be initially visible.
	*/
	bool bShow = false;

	/**
	* Should we perform any debugging on this canvas (should we log its state etc.)
	*/
	bool bDebug = false;

	SSpriteCanvasCreateCommandInitializer() :
		ZBeforeSpriteCanvasId(NULL_SPRITE_CANVAS_ID)
	,	Name(DEFAULT_CANVAS_NAME)
	,	bShow(false)
	,	InitialCapacityInSprites(DEFAULT_CAPACITY_IN_SPRITES)
	,	bAutoResize(false)
	,	bDebug(false) {}
	SSpriteCanvasCreateCommandInitializer(SpriteCanvasId InTargetSpriteCanvasId, const std::string& InName, const SSpriteCanvasProps& InProps) :
		SSpriteCanvasCommandInitializerBase(InTargetSpriteCanvasId)
	,	Name(InName)
	,	Props(InProps)
	,	InitialCapacityInSprites(DEFAULT_CAPACITY_IN_SPRITES)
	,	bAutoResize(false)
	,	bShow(false) 
	,	bDebug(false) {}

};

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv