#pragma once

#include "SpriteTypes.h"
#include <iomanip>
#include <string>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{

/**
* Pick properties of the sprite.
*/
struct SSpritePickProps
{
	PickObjectId PickId;

	bool CanBePicked() const;

	inline SSpritePickProps() :
		SSpritePickProps{ZERO_PICK_OBJECT_ID} {}
	inline SSpritePickProps(PickObjectId InPickId) :
		PickId{ InPickId } {}

	static const SSpritePickProps Disabled;
};

template<class Strm>
Strm& operator<<(Strm& S, const SSpritePickProps& InProps)
{
	S << "PickId: " << InProps.PickId << "(" << GetPickObjectIdValidityStr(InProps.PickId) << ")" << std::endl;
	return S;
}

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

	/**
	* Picking properties of the sprite.
	*/
	SSpritePickProps Pick;	

	SSpriteProps() = default;
	SSpriteProps
	(
		const SSpriteGeometryProps& InGeometry, const SSpriteRenderMode& InRenderMode, 
		const SSpritePickProps& InPick = SSpritePickProps{}
	) :
		Geometry { InGeometry }
	,	RenderMode { InRenderMode }
	,	Pick{InPick} {}
};

} // Dv::Spr
} // Dv