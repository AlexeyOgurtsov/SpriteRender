#include "Glue_InterfaceToImpl.h"
#include "QSpriteRender/IMPL/Canvas/SpriteGeometryData.h"
#include "ISpriteRender/SpriteCommandInitializers.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

SSpriteGeometryData FillGeometryDataStruct(const SSpriteGeometryProps& InProps)
{
	SSpriteGeometryData resultGeometry;
	resultGeometry.Height = InProps.Size.Height;
	resultGeometry.Width = InProps.Size.Width;
	resultGeometry.PositionX = InProps.Transform.Position.X;
	resultGeometry.PositionY = InProps.Transform.Position.Y;
	resultGeometry.OriginOffset[0] = InProps.Transform.Origin.X;
	resultGeometry.OriginOffset[1] = InProps.Transform.Origin.Y;
	resultGeometry.AngleDegs = InProps.Transform.AngleDegs;
	for (int vertexIndex = 0; vertexIndex < 4; vertexIndex++)
	{
		resultGeometry.TexCoordX[vertexIndex] = InProps.Vertices.V[vertexIndex].TexCoord.X;
		resultGeometry.TexCoordY[vertexIndex] = InProps.Vertices.V[vertexIndex].TexCoord.Y;
	}
	return resultGeometry;
}

MaterialInstanceRenderStateHandle DowncastMaterialInstanceRSHandle(const Ren::MaterialInstanceRenderStateInitializerPtr& pInHandle)
{
	return std::dynamic_pointer_cast<MaterialInstanceRenderState>(pInHandle);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv