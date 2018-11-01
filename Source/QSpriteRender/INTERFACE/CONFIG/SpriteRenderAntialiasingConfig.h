#pragma once

#include <string>
#include <boost/assert.hpp>
#include <d3d11.h> // D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT

namespace Dv
{
namespace Spr
{
namespace QRen
{

struct SAntialiasingConfig
{
	/**
	* If sample count is greater than one, than MSAA is enabled.
	*/
	UINT MSAA_SampleCount = 1;

	/**
	* Quality level of MSAA (must be zero if MSAA is disabled).
	*/
	UINT MSAA_Quality = 0;

	/**
	* Returns true if MSAA is enabled
	*/
	bool IsMSAAEnabled() const { return MSAA_SampleCount > 1; }

	/**
	* Default config: creates config with antialiasing turned off.
	*/
	SAntialiasingConfig() = default;

	/**
	* Initialize with the given minimal config.
	*/
	SAntialiasingConfig(UINT InMSAA_SampleCount, UINT InMSAA_Quality) :
		MSAA_SampleCount{InMSAA_SampleCount}
	,	MSAA_Quality{InMSAA_Quality} {}
};

template<class Strm>
inline void LogAntialiasingConfig(Strm& strm, const SAntialiasingConfig& InConfig)
{
	strm << "Antialiasing config:" << std::endl;
	if(InConfig.IsMSAAEnabled())
	{
		strm << std::left << std::setw(32) << "MSAA enabled" << std::endl;
		strm << std::left << std::setw(32) << "MSAA Sample count: " << InConfig.MSAA_SampleCount << std::endl;
		strm << std::left << std::setw(32) << "MSAA Quality: " << InConfig.MSAA_Quality << std::endl;
	}
	else
	{
		strm << std::left << std::setw(32) << "MSAA disabled" << std::endl;
	}
}

/**
* Checks that config is valid.
* (meaning that the subsystem will work with the given properties)
*/
inline void AssertAntialiasingConfigValid(const SAntialiasingConfig& InConfig)
{
	BOOST_ASSERT_MSG(InConfig.MSAA_SampleCount >= 1, "QSpriteRender: MSAA sample count must contain at least ONE sample");
	BOOST_ASSERT_MSG(InConfig.MSAA_SampleCount <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT, "QSpriteRender: MSAA sample count must be not greater than D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT");

	if(InConfig.IsMSAAEnabled())
	{
		BOOST_ASSERT_MSG(InConfig.MSAA_Quality > 0, "QSpriteRender: MSAA quality must be greater than zero if MSAA is enabled");
	}
}

} // Dv::Spr::QRen
} // DV::Spr
} // Dv