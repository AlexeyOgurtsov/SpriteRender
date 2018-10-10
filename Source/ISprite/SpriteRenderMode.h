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