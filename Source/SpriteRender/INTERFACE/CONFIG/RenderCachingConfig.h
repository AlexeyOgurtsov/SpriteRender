#pragma once

#include <iomanip>

namespace Dv
{
namespace Spr
{
namespace Ren
{

/**
* Parameters of render caching.
*/
struct SRenderCachingConfig
{
	/**
	* Should we use the command list to cache rendering.
	*
	* Warning: the d3d11 device must support command lists.
	*/
	bool bUseCommandListToCacheRendering;

	SRenderCachingConfig() :
		bUseCommandListToCacheRendering{ false } {}
};

template<class Strm>
inline void LogRenderCachingConfig(Strm& strm, const SRenderCachingConfig& InConfig)
{
	strm << "Render caching config:" << std::endl;
	strm << "Should use command list to cache rendering: " << (InConfig.bUseCommandListToCacheRendering ? "YES" : "no") << std::endl;
}

/**
* Checks that config is valid.
* (meaning that the subsystem will work with the given properties)
*/
inline void AssertRenderCachingConfigValid(const SRenderCachingConfig& InConfig)
{
	// Nothing to do here YET.
}

} // Dv::Spr::Ren
} // DV::Spr
} // Dv