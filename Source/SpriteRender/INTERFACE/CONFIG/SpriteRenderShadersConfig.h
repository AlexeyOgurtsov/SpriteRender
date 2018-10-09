#pragma once

#include <string>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{

// WARNING!!! WARNING!!! WARNING!!! Never set _DEBUG and SKIP_OPTIMIZATION flags together:
// It will bring the "Function call discards pixel" bug.
constexpr UINT SHADER_COMPILE_FLAGS = D3D10_SHADER_DEBUG;


struct SShadersConfig
{
	/**
	* Full path to the directory, where the shaders are to be outputted.
	*/
	std::string OutputDirectory;

	/**
	* Compilation flags.
	*/
	UINT CompileFlags = SHADER_COMPILE_FLAGS;

	/**
	* Default config
	*/
	SShadersConfig() :
		OutputDirectory{ "" }
	,	CompileFlags{ SHADER_COMPILE_FLAGS } {}

	/**
	* Initialize with the given minimal config.
	*/
	SShadersConfig(const std::string& InOutputDirectory) :
		OutputDirectory{ InOutputDirectory } {}
};

namespace IMPL
{
	namespace D3D
	{
		template<class Strm>
		void LogCompileFlags(Strm& shaderLog, UINT InCompileFlags);
		void ValidateShaderCompileFlags(UINT InCompileFlags);
	}
}

template<class Strm>
inline void LogShadersConfig(Strm& strm, const SShadersConfig& InConfig)
{
	strm << "Shaders config:" << std::endl;
	strm << std::left << std::setw(32) << "Output directory: " << InConfig.OutputDirectory << std::endl;
	strm << std::left << std::setw(32) << "Compile flags: " << std::endl;
	IMPL::D3D::LogCompileFlags(strm, InConfig.CompileFlags);
}

/**
* Checks that config is valid.
* (meaning that the subsystem will work with the given properties)
*/
inline void AssertShadersConfigValid(const SShadersConfig& InConfig)
{
	BOOST_ASSERT_MSG( ! InConfig.OutputDirectory.empty(), "SpriteRender config is invalid: shaders output directory must be a valid)");
	IMPL::D3D::ValidateShaderCompileFlags(InConfig.CompileFlags);
}

} // Dv::Spr::Ren
} // DV::Spr
} // Dv