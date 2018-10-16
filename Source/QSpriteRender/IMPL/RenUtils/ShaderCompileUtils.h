#pragma once

#include "RenUtilsSystem.h"
#include "ShaderProg.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

namespace D3D
{
	constexpr const char* VERTEX_SHADER_PROFILE = "vs_5_0";
	constexpr const char* PIX_SHADER_PROFILE = "ps_5_0";
	constexpr const char* GEOMETRY_SHADER_PROFILE = "gs_5_0";
	constexpr const char* HULL_SHADER_PROFILE = "hs_5_0";
	constexpr const char* DOMAIN_SHADER_PROFILE = "ds_5_0";
	constexpr const char* COMPUTE_SHADER_PROFILE = "cs_5_0";

	constexpr const char* VERTEX_SHADER_ENTRY_POINT = "vs_main";
	constexpr const char* PIX_SHADER_ENTRY_POINT = "ps_main";
	constexpr const char* GEOMETRY_SHADER_ENTRY_POINT = "gs_main";
	constexpr const char* HULL_SHADER_ENTRY_POINT = "hs_main";
	constexpr const char* DOMAIN_SHADER_ENTRY_POINT = "ds_main";
	constexpr const char* COMPUTE_SHADER_ENTRY_POINT = "cs_main";

	/**
	* Contains various objects that describe how to compile the shader and with what.
	*/ 
	struct SShaderCompileEnv
	{
		ID3D11Device* pDev = nullptr;
		std::ofstream* pLog = nullptr;
		UINT CompileFlags = 0;
		const char* pGeneratedShadersDirectory = nullptr;

		SShaderCompileEnv(ID3D11Device* pInDev, std::ofstream* pInLog, UINT InCompileFlags, const char* pInGeneratedShadersDirectory) :
			pDev(pInDev)
		,	pLog(pInLog)
		,	CompileFlags(InCompileFlags)
		,	pGeneratedShadersDirectory(pInGeneratedShadersDirectory) {}
	};

	std::unique_ptr<ShaderProg> CreateShaderProgFromMemory
	(
		const char* pInSourceName,
		std::string_view InSource,
		const SShaderCompileEnv& InEnv
	);

	std::unique_ptr<ShaderProg> CreateShaderProgFromMemory
	(
		ID3D11Device* pDev,
		std::ofstream& InLog,
		const char* pInSourceName,
		std::string_view InSource,
		UINT InCompileFlags,
		const char* pInGeneratedShadersDirectory
	);
	VertexShader CreateVertexShaderFromMemory
	(
		ID3D11Device* pDev,
		std::ofstream& shaderLog,
		const char* pInSourceName,
		std::string_view InSource,
		const char* pInEntryPoint,
		const char* pInProfileName,
		UINT InCompileFlags
	);
	PixShader CreatePixShaderFromMemory
	(
		ID3D11Device* pDev,
		std::ofstream& shaderLog,
		const char* pInSourceName,
		std::string_view InSource,
		const char* pInEntryPoint,
		const char* pInProfileName,
		UINT InCompileFlags
	);

	std::ofstream OpenShaderLogFile(const char* pInSourceName, const char* pInGeneratedShadersDirectory);
} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv