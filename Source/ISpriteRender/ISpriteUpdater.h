#pragma once

#include "SpriteCommandInitializers.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{

class ISpriteUpdater
{
public:
	virtual ~ISpriteUpdater() = default;

	/**
	* Commit the create command for the sprite.
	*/
	virtual void CreateSprite(const SSpriteCreateCommandInitializer& InCommand) = 0;

	/**
	* Commit the delete command for the sprite.
	*/
	virtual void DeleteSprite(const SSpriteDeleteCommandInitializer& InCommand) = 0;

	/**
	* Hides the given sprite.
	*/
	virtual void HideSprite(const SSpriteHideCommandInitializer& InCommand) = 0;

	/**
	* Shows the given sprite.
	*/
	virtual void ShowSprite(const SSpriteShowCommandInitializer& InCommand) = 0;

	/**
	* Changes the transparency of the sprite.
	*/
	virtual void SetSpriteTransparency(const SSpriteSetTransparencyCommandInitializer& InCommand) = 0;

	/**
	* Changes the geometry of the sprite.
	*/
	virtual void SetSpriteGeometry(const SSpriteSetGeometryCommandInitializer& InCommand) = 0;

	/**
	* Changes material properties of the sprite.
	*/
	virtual void SetSpriteMaterial(const SSpriteSetMaterialCommandInitializer& InCommand) = 0;

	/**
	* Changes Z order of the sprite.
	*/
	virtual void SetSpriteZOrder(const SSpriteSetZOrderCommandInitializer& InCommand) = 0;
};

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv