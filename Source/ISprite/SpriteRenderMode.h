#pragma once

namespace Dv
{
namespace Spr
{

enum class ESpriteTransparency
{
	Opaque = 0,
	Transparent
};
inline bool IsTransparent(ESpriteTransparency InTransparency)
{
	return ESpriteTransparency::Opaque != InTransparency;
}

inline const char* ToString(ESpriteTransparency InTransparency)
{
	switch(InTransparency)
	{
	case ESpriteTransparency::Opaque:
		return "Opaque";

	case ESpriteTransparency::Transparent:
		return "Transparent";

	default:
		break;
	}
	return "UnknownLiteral";
}


struct SSpriteRenderMode
{
	ESpriteTransparency Transparency;

	SSpriteRenderMode() :
		SSpriteRenderMode(ESpriteTransparency::Opaque) {}
	explicit SSpriteRenderMode(ESpriteTransparency InTransparency) :
		Transparency(InTransparency) {}
	
};

} // Dv::Spr
} // Dv