#pragma once

#include "SpriteTypes.h"

namespace Dv
{
namespace Spr
{

/**
* Describes geometry properties of the sprite.
*/
struct SSpriteGeometryProps
{
	/**
	* Tranform of the sprite.
	*/
	Math::SSpriteTransform Transform;

	/**
	* Size of the sprite.
	*/
	Math::SSize Size;

	/**
	* Vertex data.
	*/
	SSpriteVertices Vertices;

	SSpriteGeometryProps() = default;
	SSpriteGeometryProps(const Math::SSpriteTransform& InTransform, const Math::SSize& InSize) :
		Transform { InTransform }
	,	Size { InSize } {}
};

/**
* All properties of the sprite, except the material instance.
* Initially created to facilitate passing properties to the render.
*/
struct SSpriteProps
{
	/**
	* Geometry of the sprite.
	*/
	SSpriteGeometryProps Geometry;

	/**
	* All rendering settings.
	*/
	SSpriteRenderMode RenderMode;

	SSpriteProps() = default;
	SSpriteProps(const SSpriteGeometryProps& InGeometry, const SSpriteRenderMode& InRenderMode) :
		Geometry { InGeometry }
	,	RenderMode { InRenderMode } {}
};

} // Dv::Spr
} // Dv