#pragma once

#include "CONFIG/SpriteRenderConfig.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{

/**
* Minimum set of required parameters for initialization of the sprite render subsystem.
*/
struct SSubsystemMinimalConfig
{
	/**
	* Configuration of the render
	*/
	SMinimalConfig Render;

	/**
	* Default ctor: creates an uninitialized config.
	*/
	SSubsystemMinimalConfig() = default;

	/**
	* Constructor with a minimal set of parameters.
	*/
	SSubsystemMinimalConfig(const SMinimalConfig& InRender) :
		Render{InRender} {}
};

/**
* Contains all properties for initializing the sprite render subsystem.
*/
struct SSpriteRenderSubsystemInitializer
{
	/**
	* Config of the render itself.
	*/
	SConfig Render;
	
	/**
	* Default ctor.
	* WARNING!!! Creates invalid config, you must initialize it by yourself.
	*/
	SSpriteRenderSubsystemInitializer() = default;

	/**
	* Constructor with parameters.
	*/
	SSpriteRenderSubsystemInitializer(const SConfig& InRenderConfig) :
		Render{InRenderConfig} {}

	/**
	* Constructor with a minimum set of required parameters.
	*/
	SSpriteRenderSubsystemInitializer(const SSubsystemMinimalConfig& InMinimalConfig) :
		Render{InMinimalConfig.Render} {}
};

/**
* Checks that initializer is valid.
* (meaning that the subsystem will work with the given properties)
*/
inline void AssertInitializerValid(const SSpriteRenderSubsystemInitializer& InInitializer)
{
	AssertConfigValid(InInitializer.Render);
}

template<class Strm>
inline void LogSpriteRenderInitializer(Strm& strm, const SSpriteRenderSubsystemInitializer& InInitializer)
{
	LogRenderConfig(strm, InInitializer.Render);
}

} // Dv::Spr::Ren
} // Dv::Spr
} // Dv