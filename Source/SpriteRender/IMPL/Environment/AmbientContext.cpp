#include "AmbientContext.h"
#include "ISprite/Core/SpriteRenderException.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

std::ofstream OpenLog(const std::string& InLogFilename)
{
	std::ofstream ofstream { InLogFilename, std::ios::out };
	if (false == ofstream.is_open())
	{
		throw SpriteRenderException("Failed to open log file: "+InLogFilename);
	}
	return ofstream;
}
 
AmbientContext::AmbientContext(const SAmbientContextInitializer& InInitializer) :
	_pLog(InInitializer.pLog)
{
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // DV::Spr
} // Dv