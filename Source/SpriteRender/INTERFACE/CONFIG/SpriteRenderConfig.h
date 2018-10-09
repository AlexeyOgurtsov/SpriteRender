#pragma once

#include "LoggingConfig.h"
#include "RenTargetConfig.h"
#include "SpriteRenderShadersConfig.h"
#include "RenderCachingConfig.h"
#include "RenderDeviceConfig.h"
#include <string>
#include <iomanip>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{

constexpr const int INITIAL_SPRITE_BUFFER_CAPACITY = 100;


/**
* Contains a set of minimally required config parameters.
*/
struct SMinimalConfig
{
	std::ofstream* pLog;
	ID3D11Device* pDev;
	ID3D11DeviceContext* pDevCon;
	ID3D11RenderTargetView* pRTView;
	ID3D11DepthStencilView* pDepthStencilView;
	int RTWidth, RTHeight;
	std::string ShaderOutputDirectory;

	SMinimalConfig() :
		pLog{ nullptr }
	,	pDev{ nullptr }
	,	pDevCon{ nullptr }
	,	pRTView{ nullptr }
	,	pDepthStencilView{ nullptr }
	,	RTWidth{ 0 }, RTHeight{ 0 }
	,	ShaderOutputDirectory{ "" } {}
};

struct SConfig
{
	/**
	* Logging configuration.
	*/
	SLoggingConfig Log;

	/**
	* Device configuration.
	*/
	SRenderDeviceConfig Device;

	/**
	* Render target configuration
	*/
	SRenTargetConfig RenderTarget;

	/**
	* Configuration of shaders.
	*/
	SShadersConfig Shaders;

	/**
	* Configuration of caching.
	*/
	SRenderCachingConfig RenderCaching;

	/**
	* Initial size of the sprite vertex buffer in sprites.
	*/
	int InitialSpriteBufferCapacity = INITIAL_SPRITE_BUFFER_CAPACITY;

	/**
	* Default ctor.
	*
	* WARNING!!! Creates non-usable config.
	*/
	SConfig() :
		InitialSpriteBufferCapacity{ INITIAL_SPRITE_BUFFER_CAPACITY } {}

	/**
	* Constructor with a set of minimal parameters.
	*/
	SConfig(const SMinimalConfig& InMinimalConfig);
};

template<class Strm>
inline void LogRenderConfig(Strm& strm, const SConfig& InConfig)
{
	LogRenderTargetConfig(strm, InConfig.RenderTarget);
	LogRenderCachingConfig(strm, InConfig.RenderCaching);
	LogShadersConfig(strm, InConfig.Shaders);
	strm << std::setw(32) << std::left << "Initial sprite buffer capacity: " << InConfig.InitialSpriteBufferCapacity << std::endl;
}

/**
* Checks that config is valid.
* (meaning that the subsystem will work with the given properties)
*/
inline void AssertConfigValid(const SConfig& InConfig)
{
	AssertLogConfigValid(InConfig.Log);
	AssertDeviceConfigValid(InConfig.Device);
	AssertRenderTargetConfigValid(InConfig.RenderTarget);
	AssertShadersConfigValid(InConfig.Shaders);
	AssertRenderCachingConfigValid(InConfig.RenderCaching);
	BOOST_ASSERT_MSG(InConfig.InitialSpriteBufferCapacity > 0, "SpriteRender config is invalid: sprite buffer capacity must be greater than zero");
}

} // Dv::Spr::Ren
} // DV::Spr
} // Dv