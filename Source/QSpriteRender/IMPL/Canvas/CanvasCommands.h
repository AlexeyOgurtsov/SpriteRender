#pragma once

#include "ISpriteRender/SpriteCanvasCommandInitializers.h"

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
	* See the sprite render interface library sprite initializer command help for details.
	*/
	bool ShouldShow() const { return _InterfaceInitializer.bShow; }

	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	SpriteCanvasId GetZBeforeCanvasId() const { return _InterfaceInitializer.ZBeforeSpriteCanvasId; }

	/**
	* See the sprite render interface library sprite initializer command help for details.
	*/
	const SRenderLayerCanvasProps& GetProps() const { return _InterfaceInitializer.Props; }

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