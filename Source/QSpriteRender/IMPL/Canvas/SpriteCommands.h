#pragma once

#include "ISpriteRender/SpriteCommandInitializers.h"
#include "SpriteGeometryData.h"
#include "QSpriteRender/Material/MaterialInstanceRenderState.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

/**
* Arguments to be passed when creating a sprite.
*/
struct SCreateSpriteArgs
{
	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	bool ShouldShow() const { return _InterfaceInitializer.bShow; }

	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	SpriteId GetZBeforeSpriteId() const { return _InterfaceInitializer.ZBeforeSpriteId; }

	/**
	* Id of the target sprite.
	* See the sprite interface library sprite initializer command help for details.
	*/
	SpriteId GetTargetSpriteId() const { return _InterfaceInitializer.TargetSpriteId; }

	/**
	* See the sprite interface library sprite initializer command help for details.
	*/	
	MaterialInstanceRenderStateHandle GetRenderState() const;

	/**
	* See the sprite interface library sprite initializer command help for details.
	*/
	const SSpriteRenderMode& GetRenderMode() const { return _InterfaceInitializer.Props.RenderMode; }

	/**
	* See the sprite interface library sprite initializer command help for details.
	*/
	const SSpriteGeometryProps& GetGeometryProps() const { return _InterfaceInitializer.Props.Geometry; }

	/**
	* See the sprite interface library sprite initializer command help for details.
	*/
	const SSpriteProps& GetProps() const { return _InterfaceInitializer.Props; }

	/**
	* See help in the constructor parameters section.
	*/
	const SSpriteGeometryData& GetRenderLayerGeometryData() const { return _RenderLayerGeometryData; }

	/**
	* @param InInterfaceInitializer: Arguments those were passed dirrectly from the ISpriteRender interface,
	* @param InRenderLayerGeometryData: Data of the sprite geometry, converted into the internal representation of the render.
	*/
	SCreateSpriteArgs(const Ren::SSpriteCreateCommandInitializer& InInterfaceInitializer, const SSpriteGeometryData& InRenderLayerGeometryData) :
		_InterfaceInitializer(InInterfaceInitializer)
	,	_RenderLayerGeometryData(InRenderLayerGeometryData) {}

private:
	/**
	* See help in the constructor parameters section.
	*/
	Ren::SSpriteCreateCommandInitializer _InterfaceInitializer;

	/**
	* See help in the constructor parameters section.
	*/
	SSpriteGeometryData _RenderLayerGeometryData;
};

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv