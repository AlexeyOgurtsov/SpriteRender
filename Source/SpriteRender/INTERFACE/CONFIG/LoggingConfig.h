#pragma once

#include <d3d11.h>
#include <fstream>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{

/**
* Configuration of the logging.
*/
struct SLoggingConfig
{
	/**
	* Pointer to the log.
	*/
	std::ofstream* pLog;

	SLoggingConfig() :
		pLog{nullptr} {}
	SLoggingConfig(std::ofstream* pInLog) :
		pLog(pInLog) {}
};

/**
* Checks that config is valid.
* (meaning that the subsystem will work with the given properties)
*/
inline void AssertLogConfigValid(const SLoggingConfig& InConfig)
{
	BOOST_ASSERT_MSG(InConfig.pLog, "SpriteRender config is invalid: Log pointer must be valid");
}

} // Dv::Spr::Ren
} // DV::Spr
} // Dv