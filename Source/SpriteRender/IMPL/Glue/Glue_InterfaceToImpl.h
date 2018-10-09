#pragma once

#include "SpriteRender/IMPL/Geometry/SpriteGeometryData.h"
#include "SpriteRender/IMPL/Sprite/SpriteData.h"
#include "SpriteRender/Material/MaterialInstanceRenderState.h"
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

namespace Ren
{
namespace IMPL
{

SSpriteGeometryData FillGeometryDataStruct(const SSpriteGeometryProps& InProps);
SSpriteData FillSpriteDataStruct(const SSpriteCreateCommandInitializer& InInitializer);
MaterialInstanceRenderStateHandle DowncastMaterialInstanceRSHandle(const MaterialInstanceRenderStateInitializerPtr& pInHandle);

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv