#pragma once

#include "SpriteTypes.h"
#include <iomanip>
#include <string>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{

enum class ESpritePickMode
{
	Enabled,
	Disabled
};
inline std::string ToString(ESpritePickMode InMode)
{
	switch (InMode)
	{
	case ESpritePickMode::Enabled:
		return "{Enabled}";

	case ESpritePickMode::Disabled:
		return "{Disabled}";

	default:
		break;
	}
	BOOST_ASSERT_MSG(false, "ToString(ESpritePickMode): unknown pick mode");
	return std::string("SHOULD_NOT_GET_HERE");
}

template<class Strm> Strm& operator<<(Strm& S, ESpritePickMode InMode)
{
	return S << ToString(InMode);
}

/**
* Pick properties of the sprite.
*/
struct SSpritePickProps
{
	/**
	* Associated object id.
	*/
	PickObjectId PickId;

	/**
	* Pick mode.
	*
	* (we provided mode variable instead of just assigning invalid id to allow dynamically switch between "pickable"/"non-pickable" states
	* without losing the associated id).
	*/
	ESpritePickMode Mode;

	bool CanBePicked() const;

	inline SSpritePickProps() :
		SSpritePickProps{ ZERO_PICK_OBJECT_ID, ESpritePickMode::Disabled } {}
	inline SSpritePickProps(PickObjectId InPickId, ESpritePickMode InMode) :
		PickId{ InPickId }
	,	Mode{ InMode } {}

	static const SSpritePickProps Disabled;
};

template<class Strm>
Strm& operator<<(Strm& S, const SSpritePickProps& InProps)
{
	S << "Mode: " << InProps.Mode << std::endl;
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