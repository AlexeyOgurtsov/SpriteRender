#include "SpriteGeometryVertex.h"
#include <new>

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

void SetD3D11SpriteVertex
(
	void* pDest,
	float InPositionX, float InPositionY, float InPositionZ,
	float InTexCoordX, float InTexCoordY
)
{
	SD3D11SpriteGeometryVertex* pDestVert = new (pDest) SD3D11SpriteGeometryVertex();

	pDestVert->Position[0] = InPositionX;
	pDestVert->Position[1] = InPositionY;
	pDestVert->Position[2] = InPositionZ;

	pDestVert->TexCoord[0] = InTexCoordX;
	pDestVert->TexCoord[1] = InTexCoordY;
}

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv