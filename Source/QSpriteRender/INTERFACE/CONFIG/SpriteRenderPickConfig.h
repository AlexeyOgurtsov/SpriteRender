#pragma once

#include <string>
#include <boost/assert.hpp>
#include <windows.h> // UINT

namespace Dv
{
namespace Spr
{
namespace QRen
{

/**
* Special value for pick dimensions that means that 
* width/height of the buffer are equal to the render target's dimensions.
*/
constexpr UINT PICKBUFFER_COPY_FROM_RT = 0;

struct SPickConfig
{
	/**
	* Is pick buffer enabled at all.
	*/
	bool bEnabled = false;

	/**
	* Width of the pick buffer.
	*/
	UINT BufferWidth = PICKBUFFER_COPY_FROM_RT; 

	/**
	* Height of the pick buffer.
	*/
	UINT BufferHeight = PICKBUFFER_COPY_FROM_RT; 

	/**
	* Default config
	*/
	SPickConfig() = default;

	/**
	* Initialize with the given minimal config.
	*/
	SPickConfig(UINT InBufferWidth, UINT InBufferHeight) :
		bEnabled{true}
	,	BufferWidth{InBufferWidth}
	,	BufferHeight{InBufferHeight} {}
};

template<class Strm>
inline void LogPickConfig(Strm& strm, const SPickConfig& InConfig)
{
	strm << "Pick config:" << std::endl;
	strm << std::left << std::setw(32) << "Enabled: " << (InConfig.bEnabled ? "YES" : "no") << std::endl;
	if(InConfig.bEnabled)
	{
		strm << std::left << std::setw(32) << "BufferWidth: " << InConfig.BufferWidth << std::endl;
		strm << std::left << std::setw(32) << "BufferHeight: " << InConfig.BufferHeight << std::endl;
	}
}

/**
* Checks that config is valid.
* (meaning that the subsystem will work with the given properties)
*/
inline void AssertPickConfigValid(const SPickConfig& InConfig)
{
	BOOST_ASSERT_MSG(InConfig.BufferWidth <= 4000, "QSpriteRender: Pick buffer width is too high");
	BOOST_ASSERT_MSG(InConfig.BufferHeight <= 4000, "QSpriteRender: Pick buffer height is too high");
}

} // Dv::Spr::QRen
} // DV::Spr
} // Dv