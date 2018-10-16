#pragma once

#include "ISprite/SpriteMaterialTypedefs.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

class IMaterial
{
public:
	virtual ~IMaterial() = default;

	virtual SpriteMaterialId GetId() const = 0;
};

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv