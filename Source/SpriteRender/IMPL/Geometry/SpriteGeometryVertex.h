#pragma once

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{
namespace D3D
{

struct SD3D11SpriteGeometryVertex
{
	// Position: X,Y,Z vector of position
	float Position[3];
	float TexCoord[2];
};

constexpr size_t SPRITE_VERTEX_SIZE = sizeof(SD3D11SpriteGeometryVertex);
constexpr size_t SPRITE_GEOMETRY_SIZE = 4 * SPRITE_VERTEX_SIZE;

void SetD3D11SpriteVertex
(
	void* pDest,
	float InPositionX, float InPositionY, float InPositionZ,
	float InTexCoordX, float InTexCoordY
);

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv
