#include "ShaderCompileUtils.h"
#include "SpriteRender/IMPL/Utils/String/Text.h"
#include "RenHRUtils.h"
#include <boost/assert.hpp>
#include <d3dcompiler.h>
#include <iomanip>

namespace Dv
{
namespace Spr
{
namespace Ren
{

namespace IMPL
{

namespace D3D
{
	std::string GetShaderLogFullFilename(const char* pInSourceName, const char* pInGeneratedShadersDirectory);
	std::ofstream OpenShaderLogFile(const char* pInSourceName, const char* pInGeneratedShadersDirectory);
	void LogShaderSource(std::ofstream& shaderLog, const char* pInSourceName, std::string_view InSource);

	void LogBlobAsText(std::ofstream& shaderLog, ID3D10Blob* pNullOrBlob)
	{
		if (pNullOrBlob == nullptr)
		{
			shaderLog << "nullptr" << std::endl;
			return;
		}
		const auto pStr = static_cast<const char*>(pNullOrBlob->GetBufferPointer());
		const auto length = pNullOrBlob->GetBufferSize();
		std::string_view blobText{ pStr, length };
		Utils::WriteText(shaderLog, blobText, true);
	}

	void ValidateShaderCompileFlags(UINT InCompileFlags)
	{
		bool bCompileDebug = InCompileFlags & D3DCOMPILE_DEBUG;
		bool bCompileSkipOptimization = InCompileFlags & D3DCOMPILE_SKIP_OPTIMIZATION;
		BOOST_ASSERT_MSG
		(
			(false == bCompileDebug) || (false == bCompileSkipOptimization),
			"Either D3DCOMPILE_DEBUG or D3DCOMPILE_SKIP_OPTIMIAZATION flag is to be disabled,\n"
			"because otherwise the bug (function call discards pixel shader output) will be"
		);
	}

	template<class Strm>
	void LogCompileFlags(Strm& shaderLog, UINT InCompileFlags)
	{
		if (InCompileFlags & D3DCOMPILE_DEBUG)
		{
			shaderLog << "D3DCOMPILE_DEBUG" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_SKIP_OPTIMIZATION)
		{
			shaderLog << "D3DCOMPILE_SKIP_OPTIMIZATION" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_SKIP_VALIDATION)
		{
			shaderLog << "D3DCOMPILE_SKIP_VALIDATION" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR)
		{
			shaderLog << "D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_PACK_MATRIX_ROW_MAJOR)
		{
			shaderLog << "D3DCOMPILE_PACK_MATRIX_ROW_MAJOR" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_PARTIAL_PRECISION)
		{
			shaderLog << "D3DCOMPILE_PARTIAL_PRECISION" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT)
		{
			shaderLog << "D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_FORCE_PS_SOFTWARE_NO_OPT)
		{
			shaderLog << "D3DCOMPILE_FORCE_PS_SOFTWARE_NO_OPT" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_NO_PRESHADER)
		{
			shaderLog << "D3DCOMPILE_NO_PRESHADER" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_AVOID_FLOW_CONTROL)
		{
			shaderLog << "D3DCOMPILE_AVOID_FLOW_CONTROL" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_PREFER_FLOW_CONTROL)
		{
			shaderLog << "D3DCOMPILE_PREFER_FLOW_CONTROL" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_ENABLE_STRICTNESS)
		{
			shaderLog << "D3DCOMPILE_ENABLE_STRICTNESS" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY)
		{
			shaderLog << "D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_IEEE_STRICTNESS)
		{
			shaderLog << "D3DCOMPILE_IEEE_STRICTNESS" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_OPTIMIZATION_LEVEL0)
		{
			shaderLog << "D3DCOMPILE_OPTIMIZATION_LEVEL0" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_OPTIMIZATION_LEVEL1)
		{
			shaderLog << "D3DCOMPILE_OPTIMIZATION_LEVEL1" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_OPTIMIZATION_LEVEL2)
		{
			shaderLog << "D3DCOMPILE_OPTIMIZATION_LEVEL2" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_OPTIMIZATION_LEVEL3)
		{
			shaderLog << "D3DCOMPILE_OPTIMIZATION_LEVEL3" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_WARNINGS_ARE_ERRORS)
		{
			shaderLog << "D3DCOMPILE_WARNINGS_ARE_ERRORS" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_RESOURCES_MAY_ALIAS)
		{
			shaderLog << "D3DCOMPILE_RESOURCES_MAY_ALIAS" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES)
		{
			shaderLog << "D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES" << std::endl;
		}

		if (InCompileFlags & D3DCOMPILE_ALL_RESOURCES_BOUND)
		{
			shaderLog << "D3DCOMPILE_ALL_RESOURCES_BOUND" << std::endl;
		}
	}

	std::string GetHRStringCreateShader(HRESULT hr)
	{
		return GetFormattedHRESULTString(hr, GetCommonHRESULTDescription(hr));
	}

	std::string GetHRStringCompileBlob(HRESULT hr)
	{
		return GetFormattedHRESULTString(hr, GetCommonHRESULTDescription(hr));
	}

	void Log_D3DCompileUtil_ShaderState
	(
		std::ofstream& shaderLog,
		const std::string& infoStringPrefix,
		const char* pInSourceName,
		std::string_view InSource,
		const char* pInEntryPoint,
		const char* pInProfileName,
		UINT InCompileFlags
	)
	{
		shaderLog << std::left << std::setw(32) << "EntryPoint: " << pInEntryPoint << std::endl;
		shaderLog << std::left << std::setw(32) << "ProfileName: " << pInProfileName << std::endl;
		shaderLog << std::left << std::setw(32) << "CompileFlags: " << std::endl;
		LogCompileFlags(shaderLog, InCompileFlags);
	}

	HRESULT D3DCompileUtil
	(
		std::ofstream& shaderLog,
		const std::string& infoStringPrefix,
		const char* pInSourceName,
		std::string_view InSource,
		const char* pInEntryPoint,
		const char* pInProfileName,
		UINT InCompileFlags,
		ID3D10Blob** pOutBlob,
		ID3D10Blob** pOutErrBlob
	)
	{
		ValidateShaderCompileFlags(InCompileFlags);

		const D3D10_SHADER_MACRO* pMacroDefinesArg = nullptr; // TODO
		const auto pInclude = nullptr;


		//GetLog() << "D3DCompile STARTED" << std::endl;
		shaderLog << "D3DCompile STARTED" << std::endl;

		// Log state:
		Log_D3DCompileUtil_ShaderState
		(
			shaderLog,
			infoStringPrefix,
			pInSourceName,
			InSource,
			pInEntryPoint,
			pInProfileName,
			InCompileFlags
		);

		// Compile blob
		HRESULT hr_compile_blob = D3DCompile
		(
			InSource.data(), InSource.size(),
			pInSourceName,
			pMacroDefinesArg,
			pInclude,
			pInEntryPoint,
			pInProfileName,
			InCompileFlags,
			0, // Flags2: NEVER USED BY THE COMPILER
			pOutBlob,
			pOutErrBlob
		);
		const std::string blobHRdesc = GetHRStringCompileBlob(hr_compile_blob);
		shaderLog << "D3DCompile hr: " << blobHRdesc << std::endl;
		//GetLog() << "D3DCompile hr: " << blobHRdesc << std::endl;
		if (FAILED(hr_compile_blob))
		{
			shaderLog << "FATAL ERROR: D3DCompile returned FAILED HR!" << std::endl;
			//GetLog() << "FATAL ERROR: D3DCompile returned FAILED HR!" << std::endl;
		}
		shaderLog << "Error blob:" << std::endl;
		//GetLog() << "Error blob:" << std::endl;
		if (pOutErrBlob)
		{
			//LogBlobAsText(GetLog(), *pOutErrBlob);
			LogBlobAsText(shaderLog, *pOutErrBlob);
		}

		// Handle errors
		if (FAILED(hr_compile_blob))
		{
			throw SpriteRenderHRFailure{"D3DCompileUtil: shader blob compilation failed", hr_compile_blob};
		}

		// Mark compilation ended
		shaderLog << "D3DCompile Ended" << std::endl;
		//GetLog() << "D3DCompile ENDED" << std::endl;
		return hr_compile_blob;
	}

	VertexShader CreateVertexShaderFromMemory
	(
		ID3D11Device* pDev,
		std::ofstream& shaderLog,
		const char* pInSourceName,
		std::string_view InSource,
		const char* pInEntryPoint,
		const char* pInProfileName,
		UINT InCompileFlags
	)
	{
		BOOST_ASSERT(pInSourceName);
		BOOST_ASSERT(pInEntryPoint);
		BOOST_ASSERT(pInProfileName);

		// For logging and info
		std::string infoStringPrefix = std::string("CreateVertexShaderFromMemory, pInSourceName=") + std::string(pInSourceName) + std::string(":");
		//GetLog() << "Shader creation..." << std::endl << infoStringPrefix << std::endl;
		shaderLog << "Shader creation..." << std::endl << infoStringPrefix << std::endl;

		// -----------------------------------------------------
		ID3D10Blob* pBlob = nullptr;
		ID3D10Blob* pErrBlob = nullptr;
		HRESULT hr_compile_blob = D3DCompileUtil
		(
			shaderLog,
			infoStringPrefix,
			pInSourceName,
			InSource,
			pInEntryPoint,
			pInProfileName,
			InCompileFlags,
			&pBlob, &pErrBlob
		);


		// Create vertex shader
		//GetLog() << "CreateVertexShader..." << std::endl;
		shaderLog << "CreateVertexShader..." << std::endl;
		ID3D11ClassLinkage* pClassLinkage = nullptr; // TODO!!!
		ID3D11VertexShader* pShader = nullptr;
		//GetLog() << "pClassLinkage: " << (pClassLinkage ? "YES" : "nullptr") << std::endl;
		HRESULT hr_create = pDev->CreateVertexShader
		(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
			pClassLinkage,
			&pShader
		);
		//GetLog() << "HR: " << GetHRStringCreateShader(hr_create) << std::endl;
		if (FAILED(hr_create))
		{
			throw SpriteRenderHRFailure{ "CreateVertexShaderFromMemory: failed", hr_create };
		}
		//GetLog() << "CreateVertexShader ENDED" << std::endl;
		shaderLog << "CreateVertexShader ENDED" << std::endl;

		// -----------------------------------------------------
		//GetLog() << "Shader creation ENDED: " << infoStringPrefix << std::endl;
		shaderLog << "Shader creation ENDED: " << std::endl;
		return VertexShader(pShader, hr_create, hr_compile_blob, pBlob, pErrBlob);
	}

	PixShader CreatePixShaderFromMemory
	(
		ID3D11Device* pDev,
		std::ofstream& shaderLog,
		const char* pInSourceName,
		std::string_view InSource,
		const char* pInEntryPoint,
		const char* pInProfileName,
		UINT InCompileFlags
	)
	{
		BOOST_ASSERT(pInSourceName);
		BOOST_ASSERT(pInEntryPoint);
		BOOST_ASSERT(pInProfileName);

		// For logging and info
		std::string infoStringPrefix = std::string("CreatePixShaderFromMemory, pInSourceName=") + std::string(pInSourceName) + std::string(":");
		//GetLog() << "Shader creation...: " << std::endl << infoStringPrefix << std::endl;
		shaderLog << "Shader creation...: " << std::endl << infoStringPrefix << std::endl;

		// -----------------------------------------------------
		ID3D10Blob* pBlob = nullptr;
		ID3D10Blob* pErrBlob = nullptr;
		HRESULT hr_compile_blob = D3DCompileUtil
		(
			shaderLog,
			infoStringPrefix,
			pInSourceName,
			InSource,
			pInEntryPoint,
			pInProfileName,
			InCompileFlags,
			&pBlob, &pErrBlob
		);


		// Create vertex shader
		//GetLog() << "CreatePixShader..." << std::endl;
		shaderLog << "CreatePixShader..." << std::endl;
		ID3D11ClassLinkage* pClassLinkage = nullptr; // TODO!!!
		ID3D11PixelShader* pShader = nullptr;
		//GetLog() << "pClassLinkage: " << (pClassLinkage ? "YES" : "nullptr") << std::endl;
		HRESULT hr_create = pDev->CreatePixelShader
		(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
			pClassLinkage,
			&pShader
		);
		//GetLog() << "HR: " << GetHRStringCreateShader(hr_create) << std::endl;
		if (FAILED(hr_create))
		{
			throw SpriteRenderHRFailure{ "CreatePixShaderFromMemory: failed", hr_create };
		}
		//GetLog() << "CreatePixShader ENDED" << std::endl;
		shaderLog << "CreatePixShader ENDED" << std::endl;

		// -----------------------------------------------------
		//GetLog() << "Shader creation ENDED: " << infoStringPrefix << std::endl;
		shaderLog << "Shader creation ENDED: " << std::endl;
		return PixShader(pShader, hr_create, hr_compile_blob, pBlob, pErrBlob);
	}

	std::unique_ptr<ShaderProg> CreateShaderProgFromMemory
	(
		const char* pInSourceName,
		std::string_view InSource,
		const SShaderCompileEnv& InEnv
	)
	{
		return CreateShaderProgFromMemory
		(
			InEnv.pDev, *InEnv.pLog,
			pInSourceName, InSource,
			InEnv.CompileFlags, InEnv.pGeneratedShadersDirectory
		);
	}

	std::unique_ptr<ShaderProg> CreateShaderProgFromMemory
	(
		ID3D11Device* pDev,
		std::ofstream& InLog,
		const char* pInSourceName,
		std::string_view InSource,
		UINT InCompileFlags,
		const char* pInGeneratedShadersDirectory
	)
	{
		InLog << "CreateShaderProgFromMemory entered" << std::endl;
		InLog << std::left << std::setw(32) << "SourceName: " << pInSourceName << std::endl;
		// Shader log
		std::ofstream shaderLog = OpenShaderLogFile(pInSourceName, pInGeneratedShadersDirectory);
		InLog << "Shader log file opened" << std::endl;
		InLog << std::left << std::setw(32) << "Filename: " << GetShaderLogFullFilename(pInSourceName, pInGeneratedShadersDirectory) << std::endl;
		shaderLog << std::left << std::setw(32) << "SourceName: " << pInSourceName << std::endl;

		LogShaderSource(shaderLog, pInSourceName, InSource);

		// Compilation
		VertexShader vert = std::move
		(
			CreateVertexShaderFromMemory
			(
				pDev,
				shaderLog,
				pInSourceName,
				InSource,
				VERTEX_SHADER_ENTRY_POINT, VERTEX_SHADER_PROFILE,
				InCompileFlags
			)
		);
		PixShader pix = std::move
		(
			CreatePixShaderFromMemory
			(
				pDev,
				shaderLog,
				pInSourceName,
				InSource,
				PIX_SHADER_ENTRY_POINT, PIX_SHADER_PROFILE,
				InCompileFlags
			)
		);

		InLog << "CreateShaderProgFromMemory: returning" << std::endl;
		shaderLog << "CreateShaderProgFromMemory: returning" << std::endl;
		return std::make_unique<ShaderProg>(std::move(vert), std::move(pix));
	}

	std::string GetShaderLogFullFilename(const char* pInSourceName, const char* pInGeneratedShadersDirectory)
	{
		return std::string(pInGeneratedShadersDirectory) + std::string(pInSourceName) + ".txt";
	}

	std::ofstream OpenShaderLogFile(const char* pInSourceName, const char* pInGeneratedShadersDirectory)
	{
		std::string shaderLogPath = GetShaderLogFullFilename(pInSourceName, pInGeneratedShadersDirectory);
		std::ofstream ifs(shaderLogPath, std::ios::trunc);
		if (false == ifs.is_open())
		{
			throw SpriteRenderException(std::string("OpenShaderLogFile: ") + shaderLogPath);
		}
		return ifs;
	}

	void LogShaderSource(std::ofstream& shaderLog, const char* pInSourceName, std::string_view InSource)
	{
		Utils::WriteText(shaderLog, InSource, true);
	}
} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv