#pragma once

#include "ISpriteGeometry.h"
#include "SpriteGeometryData.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class ISpriteGeometry;

/**
* Provides methods for adding, deleting, updating sprite geometry.
*/
class ISpriteGeometryBufferUpdateInterface
{
public:
	virtual ~ISpriteGeometryBufferUpdateInterface() = default;
	
	virtual ISpriteGeometry* AddSprite(const SSpriteGeometryData& InGeometryData) = 0;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv