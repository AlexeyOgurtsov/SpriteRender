#pragma once

#include "ISpriteRender/SpriteCanvasCommandInitializers.h"
#include <string>

namespace Dv
{
namespace Spr
{
namespace QRen
{

namespace IMPL
{

/**
* Render-layer respresentation of the canvas props structure.
*/
using SRenderLayerCanvasProps = SSpriteCanvasProps;

/**
* Render-layer representation of the canvas render target rect structure.
*/
using SRenderLayerCanvasRect = SCanvasRect;


/**
* Arguments to be passed when creating a canvas.
*/
struct SCreateCanvasArgs
{
	/**
	* Id of the target sprite canvas id.
	*/
	SpriteCanvasId GetTargetCanvasId() const { return _InterfaceInitializer.TargetSpriteCanvasId; }

	/**
	* Name of the canvas
	*/
	const std::string& GetName() const { return _InterfaceInitializer.Name; }

	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	bool ShouldShow() const { return _InterfaceInitializer.bShow; }

	/**
	* Coord system of the canvas.
	*/
	const SCanvasCoordSystem& GetCoordSystem() const { return _InterfaceInitializer.Props.CoordSystem; }

	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	SpriteCanvasId GetZBeforeCanvasId() const { return _InterfaceInitializer.ZBeforeSpriteCanvasId; }

	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	const SRenderLayerCanvasProps& GetProps() const { return _InterfaceInitializer.Props; }

	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	int GetInitialCapacityInSprites() const { return _InterfaceInitializer.InitialCapacityInSprites; }

	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	bool IsAutoResizeable() const { return _InterfaceInitializer.bAutoResize; }

	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	bool ShouldDebug() const { return _InterfaceInitializer.bDebug; }
	/**
	* @param InInterfaceInitializer: Arguments those were passed dirrectly from the ISpriteRender interface,
	*/
	SCreateCanvasArgs(const Ren::SSpriteCanvasCreateCommandInitializer& InInterfaceInitializer) :
		_InterfaceInitializer(InInterfaceInitializer) {}

private:
	/**
	* See help in the constructor parameters section.
	*/
	Ren::SSpriteCanvasCreateCommandInitializer _InterfaceInitializer;
};

} // Dv::Spr::QRen::IMPL

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv