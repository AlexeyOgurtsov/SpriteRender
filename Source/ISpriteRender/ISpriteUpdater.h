#pragma once

#include "SpriteCommandInitializers.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{

SSpriteCreateCommandInitializer GetSprInit(SpriteId InSpriteId, const SSpriteProps& InProps, MaterialInstanceRenderStateInitializerPtr InRenderState);
SSpriteCreateCommandInitializer GetSprInit(SpriteId InSpriteId, const SSpriteGeometryProps& InGeometry, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency = ESpriteTransparency::Opaque);
SSpriteCreateCommandInitializer GetSprInit(SpriteId InSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency = ESpriteTransparency::Opaque);
SSpriteCreateCommandInitializer GetSprInit(SpriteId InSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency = ESpriteTransparency::Opaque);

SSpriteSetGeometryCommandInitializer GetSprGeometryInit(SpriteId InTargetSpriteId, const SSpriteGeometryProps& InGeometry);
SSpriteSetGeometryCommandInitializer GetSprGeometryInit(SpriteId InTargetSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize);
SSpriteSetGeometryCommandInitializer GetSprGeometryInit(SpriteId InTargetSpriteId, const Math::SVec2& InPosition, const Math::SSize& InSize);
SSpriteSetGeometryCommandInitializer GetSprGeometryInit(SpriteId InTargetSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight);

class ISpriteUpdater
{
public:
	virtual ~ISpriteUpdater() = default;

	// ~Helper interface Begin
	void CreateSprite(SpriteId InSpriteId, const SSpriteProps& InProps, MaterialInstanceRenderStateInitializerPtr InRenderState);
	void CreateSprite(SpriteId InSpriteId, const SSpriteGeometryProps& InGeometry, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency = ESpriteTransparency::Opaque);

	/**
	* Creates sprite with the given transform and size and default texture coordinates.
	*/
	void CreateSprite(SpriteId InSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency = ESpriteTransparency::Opaque);
	
	/**
	* Creates sprite with the given position and size.
	* The preferred method to create sprites quickly.
	*/
	void CreateSprite(SpriteId InSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency = ESpriteTransparency::Opaque);
	void DeleteSprite(SpriteId InSpriteId);
	void HideSprite(SpriteId InSpriteId);
	void ShowSprite(SpriteId InSpriteId);
	void SetSpriteTransparency(SpriteId InSpriteId, ESpriteTransparency InTransparency);
	void SetSpriteGeometry(SpriteId InSpriteId, const SSpriteGeometryProps& InGeometry);
	void SetSpriteMaterial(SpriteId InSpriteId, MaterialInstanceRenderStateInitializerPtr pInRenderState);
	void SetSpriteZOrder(SpriteId InSpriteId, SpriteId ZBeforeSpriteId);
	void MoveSpriteOnTop(SpriteId InSpriteId);

	void SetSpriteGeometry(SpriteId InTargetSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize);
	void SetSpriteGeometry(SpriteId InTargetSpriteId, const Math::SVec2& InPosition, const Math::SSize& InSize);
	void SetSpriteGeometry(SpriteId InTargetSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight);

	// ~Helper interface End

	// ~Virtual interface Begin
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
	// ~Virtual interface End
};

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv