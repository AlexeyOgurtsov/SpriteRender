#pragma once

#include "SpriteRender/Material/MaterialInstanceRenderState.h"
#include "ISprite/SpriteProps.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

/**
* Data of the sprite that is not incapsulated by the sprite manager,
* so can be freely updated.
*/
struct SSpriteData
{
	/**
	* Render mode of the sprite
	*/
	SSpriteRenderMode RenderMode;

	/**
	* Material instance render state.
	* Smart pointer that owns the state.
	*/
	MaterialInstanceRenderStateHandle pRenderState;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv