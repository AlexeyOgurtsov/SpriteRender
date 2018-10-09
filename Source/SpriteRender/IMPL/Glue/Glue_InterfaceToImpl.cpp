#include "Glue_InterfaceToImpl.h"
#include "ISprite/SpriteProps.h"
#include "SpriteRender/IMPL/Geometry/SpriteGeometryData.h"
#include "ISpriteRender/SpriteCommandInitializers.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

SSpriteGeometryData FillGeometryDataStruct(const SSpriteGeometryProps& InProps)
{
	SSpriteGeometryData resultGeometry;
	resultGeometry.HalfHeight = InProps.Size.Height / 2.0F;
	resultGeometry.HalfWidth = InProps.Size.Width / 2.0F;
	resultGeometry.PositionX = InProps.Transform.Position.X;
	resultGeometry.PositionY = InProps.Transform.Position.Y;
	for (int vertexIndex = 0; vertexIndex < 4; vertexIndex++)
	{
		resultGeometry.TexCoordX[vertexIndex] = InProps.Vertices.V[vertexIndex].TexCoord.X;
		resultGeometry.TexCoordY[vertexIndex] = InProps.Vertices.V[vertexIndex].TexCoord.Y;
	}
	return resultGeometry;
}

SSpriteData FillSpriteDataStruct(const SSpriteCreateCommandInitializer& InInitializer)
{
	SSpriteData resultData;
	resultData.pRenderState = DowncastMaterialInstanceRSHandle(InInitializer.pRenderState);
	resultData.RenderMode = InInitializer.Props.RenderMode;
	return resultData;
}

MaterialInstanceRenderStateHandle DowncastMaterialInstanceRSHandle(const MaterialInstanceRenderStateInitializerPtr& pInHandle)
{
	return std::dynamic_pointer_cast<MaterialInstanceRenderState>(pInHandle);
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv