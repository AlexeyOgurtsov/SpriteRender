#pragma once

#include "ISprite/SpriteMaterialTypedefs.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class IMaterial
{
public:
	virtual ~IMaterial() = default;

	virtual SpriteMaterialId GetId() const = 0;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv