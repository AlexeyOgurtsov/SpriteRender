#pragma once


namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

/**
* Completely describes the geometry of the sprite and its position.
*/
struct SSpriteGeometryData
{
	float Z;
	float PositionX, PositionY;
	float HalfWidth, HalfHeight;
	float TexCoordX[4], TexCoordY[4];
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv