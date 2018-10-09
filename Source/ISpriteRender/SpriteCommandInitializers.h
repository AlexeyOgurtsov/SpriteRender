#pragma once

#include "ISprite/SpriteTypes.h"
#include "ISprite/SpriteProps.h"
#include "SpriteMaterialInstanceRenderState.h"
#include <memory>

namespace Dv
{
namespace Spr
{
namespace Ren
{

using MaterialInstanceRenderStateInitializerPtr = std::shared_ptr<SpriteMaterialInstanceRenderState>;

/**
* Base class that all sprite command initializers should derive from.
*/
struct SSpriteCommandInitializerBase
{
	/**
	* Target sprite that this command is related to.
	*/
	SpriteId TargetSpriteId = NULL_SPRITE_ID;

	SSpriteCommandInitializerBase() :
		TargetSpriteId(NULL_SPRITE_ID) {}
	SSpriteCommandInitializerBase(SpriteId InTargetSpriteId) :
		TargetSpriteId(InTargetSpriteId) {}
};

/**
* Initializer of the create command
*/
struct SSpriteCreateCommandInitializer : public SSpriteCommandInitializerBase
{
	/**
	* Fully describes all properties of the sprite.
	*/
	SSpriteProps Props;

	/**
	* Material render state.
	*/
	MaterialInstanceRenderStateInitializerPtr pRenderState;

	/**
	* Z Order of the sprite.
	* If null, then the sprite is added at the end.
	*/
	SpriteId ZBeforeSpriteId = NULL_SPRITE_ID;

	/**
	* Should the sprite to be initially visible.
	*/
	bool bShow = false;

	SSpriteCreateCommandInitializer() :
		ZBeforeSpriteId(NULL_SPRITE_ID)
	,	bShow(false) {}
	SSpriteCreateCommandInitializer(SpriteId InTargetSpriteId, const SSpriteProps& InProps, MaterialInstanceRenderStateInitializerPtr pInRenderState) :
		SSpriteCommandInitializerBase(InTargetSpriteId)
	,	Props(InProps)
	,	pRenderState(pInRenderState)
	,	bShow(false) {}
};

struct SSpriteDeleteCommandInitializer : public SSpriteCommandInitializerBase 
{
	SSpriteDeleteCommandInitializer() = default;
	SSpriteDeleteCommandInitializer(SpriteId InTargetSpriteId) :
		SSpriteCommandInitializerBase(InTargetSpriteId) {}
};

struct SSpriteShowCommandInitializer : public SSpriteCommandInitializerBase 
{
	SSpriteShowCommandInitializer() = default;
	SSpriteShowCommandInitializer(SpriteId InTargetSpriteId) :
		SSpriteCommandInitializerBase(InTargetSpriteId) {}
};

struct SSpriteHideCommandInitializer : public SSpriteCommandInitializerBase 
{
	SSpriteHideCommandInitializer() = default;
	SSpriteHideCommandInitializer(SpriteId InTargetSpriteId) :
		SSpriteCommandInitializerBase(InTargetSpriteId) {}
};

struct SSpriteSetTransparencyCommandInitializer : public SSpriteCommandInitializerBase
{
	/**
	* New transparency state.
	*/
	ESpriteTransparency Transparency;
	
	SSpriteSetTransparencyCommandInitializer() = default;
	SSpriteSetTransparencyCommandInitializer(SpriteId InTargetSpriteId, ESpriteTransparency InTransparency) :
		SSpriteCommandInitializerBase(InTargetSpriteId)
	,	Transparency(InTransparency) {}
};

struct SSpriteSetGeometryCommandInitializer : public SSpriteCommandInitializerBase
{
	/**
	* New geometry
	*/
	SSpriteGeometryProps Geometry;

	SSpriteSetGeometryCommandInitializer() = default;
	SSpriteSetGeometryCommandInitializer(SpriteId InTargetSpriteId, const SSpriteGeometryProps& InGeometry) :
		SSpriteCommandInitializerBase(InTargetSpriteId)
	,	Geometry(InGeometry) {}
};

struct SSpriteSetMaterialCommandInitializer : public SSpriteCommandInitializerBase
{
	/**
	* New render state for the material associated with the sprite.
	*/
	MaterialInstanceRenderStateInitializerPtr pRenderState;

	SSpriteSetMaterialCommandInitializer() = default;
	SSpriteSetMaterialCommandInitializer(SpriteId InTargetSpriteId, MaterialInstanceRenderStateInitializerPtr pInRenderState) :
		SSpriteCommandInitializerBase(InTargetSpriteId)
	,	pRenderState(pInRenderState) {}
};

struct SSpriteSetZOrderCommandInitializer : public SSpriteCommandInitializerBase
{
	/**
	* Sprite to be right before the given in the Z order, after the command is issued.
	* If NULL sprite id is provided, then the sprite is to be moved on top of the Z order.
	*/
	SpriteId ZBeforeSpriteId = NULL_SPRITE_ID;

	SSpriteSetZOrderCommandInitializer() :
		ZBeforeSpriteId(NULL_SPRITE_ID) {}
	SSpriteSetZOrderCommandInitializer(SpriteId InTargetSpriteId, SpriteId InZBeforeSpriteId) :
		SSpriteCommandInitializerBase(InTargetSpriteId)
	,	ZBeforeSpriteId(InZBeforeSpriteId) {}
};

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv