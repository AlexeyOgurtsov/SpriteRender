#pragma once

#include "ISprite/SpriteCanvasProps.h"
#include "ISprite/SpriteCanvasTypedefs.h"

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
	* Should the sprite canvas to be initially visible.
	*/
	bool bShow = false;

	SSpriteCanvasCreateCommandInitializer() :
		ZBeforeSpriteCanvasId(NULL_SPRITE_CANVAS_ID)
	,	bShow(false) {}
	SSpriteCanvasCreateCommandInitializer(SpriteCanvasId InTargetSpriteCanvasId, const SSpriteCanvasProps& InProps) :
		SSpriteCanvasCommandInitializerBase(InTargetSpriteCanvasId)
	,	Props(InProps)
	,	bShow(false) {}

};

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv