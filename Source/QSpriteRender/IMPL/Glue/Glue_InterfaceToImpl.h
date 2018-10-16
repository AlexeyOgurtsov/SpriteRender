#pragma once

//#include "QSpriteRender/IMPL/Geometry/SpriteGeometryData.h"
//#include "QSpriteRender/IMPL/Sprite/SpriteData.h"
#include "QSpriteRender/Material/MaterialInstanceRenderState.h"
#include "ISpriteRender/SpriteCommandInitializers.h"

/**
* Stuff for converting incoming data structures from the ISpriteRender interface
* into the internal representation of the render.
*/

namespace Dv
{
namespace Spr
{

struct SSpriteGeometryProps;
struct SSpriteCreateCommandInitializer;

namespace QRen
{
namespace IMPL
{

//SSpriteGeometryData FillGeometryDataStruct(const SSpriteGeometryProps& InProps);
//SSpriteData FillSpriteDataStruct(const SSpriteCreateCommandInitializer& InInitializer);
MaterialInstanceRenderStateHandle DowncastMaterialInstanceRSHandle(const Ren::MaterialInstanceRenderStateInitializerPtr& pInHandle);

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv