#include "SpriteCommands.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

MaterialInstanceRenderStateHandle SCreateSpriteArgs::GetRenderState() const
{
	return std::dynamic_pointer_cast<MaterialInstanceRenderState>(_InterfaceInitializer.pRenderState);
}

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv