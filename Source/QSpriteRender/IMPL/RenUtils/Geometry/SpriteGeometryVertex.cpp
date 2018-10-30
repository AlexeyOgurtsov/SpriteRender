#include "SpriteGeometryVertex.h"
#include <cstdint>
#include <new>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{
namespace D3D
{

void SetD3D11SpriteVertex
(
	void* pDest,
	Spr::SpriteId InSpriteId,
	float InPositionX, float InPositionY, float InPositionZ,
	float InTexCoordX, float InTexCoordY
)
{
	SD3D11SpriteGeometryVertex* pDestVert = new (pDest) SD3D11SpriteGeometryVertex();

	pDestVert->SpriteId = InSpriteId;

	pDestVert->Position[0] = InPositionX;
	pDestVert->Position[1] = InPositionY;
	pDestVert->Position[2] = InPositionZ;

	pDestVert->TexCoord[0] = InTexCoordX;
	pDestVert->TexCoord[1] = InTexCoordY;
}

} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv