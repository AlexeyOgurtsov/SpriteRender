#pragma once

/**
* Contains all type declarations and definitions, accessible from the ISprite interface.h
* Also contains some system sprite constants.
*/

#include "SpriteTypedefs.h"
#include "Math/Vec2.h"
#include "Math/Size.h"
#include "Math/SpriteTransform.h"
#include "SpriteRenderMode.h"
#include "SpriteMaterialTypedefs.h"
#include "SpriteMaterialSystemConstants.h"

namespace Dv
{
namespace Spr
{

class ISprite;
class ISpriteMaterialInstance;
class ISpriteCanvas;

struct SSpriteVertexData
{
	Math::SVec2 TexCoord;
	Math::SVec2 OpacityTexCoord;

	SSpriteVertexData() = default;
	explicit SSpriteVertexData(const Math::SVec2& InTexCoord) :
		TexCoord(InTexCoord)
	,	OpacityTexCoord(InTexCoord) {}
	SSpriteVertexData(const Math::SVec2& InTexCoord, const Math::SVec2& InOpacityTexCoord) :
		TexCoord(InTexCoord)
	,	OpacityTexCoord(InOpacityTexCoord) {}	
};

struct SSpriteVertices
{
	/**
	* Extra vertex data for each sprite corner in the following order:
	* TopLeft, TopRight, BottomLeft, BottomRight.
	*/
	SSpriteVertexData V[4];

	SSpriteVertices()
	{
		V[0] = SSpriteVertexData{ Math::SVec2{ 0.0F, 0.0F }  };
		V[1] = SSpriteVertexData{ Math::SVec2{ 1.0F, 0.0F }  };
		V[2] = SSpriteVertexData{ Math::SVec2{ 0.0F, 1.0F }  };
		V[3] = SSpriteVertexData{ Math::SVec2{ 1.0F, 1.0F }  };
	}
	SSpriteVertices
	(
		const SSpriteVertexData& InTopLeft, const SSpriteVertexData& InTopRight,
		const SSpriteVertexData& InBottomLeft, const SSpriteVertexData& InBottomRight
	) 
	{
		V[0] = InTopLeft;
		V[1] = InTopRight;
		V[2] = InBottomLeft;
		V[3] = InBottomRight;
	}
};

} // Dv::Spr
} // Dv