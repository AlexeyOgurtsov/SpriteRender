#pragma once


namespace Dv
{
namespace Spr
{
namespace QRen
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
	float Width, Height;
	float TexCoordX[4], TexCoordY[4];
	float OriginOffset[2];
	float AngleDegs;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv