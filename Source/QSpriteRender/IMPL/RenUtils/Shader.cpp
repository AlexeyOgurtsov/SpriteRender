#include "Shader.h"
#include <boost/assert.hpp>

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
		Shader::Shader
		(
			HRESULT InHrCreate,
			HRESULT InHrCompile, ID3D10Blob* pInBlob, ID3D10Blob* pInErrBlob
		) :
			_hr_createObj(InHrCreate),
			_hr_compile(InHrCompile),
			_pBlob(pInBlob),
			_pErrBlob(pInErrBlob)
		{
			BOOST_ASSERT(pInBlob);
		}

		VertexShader::VertexShader
		(
			ID3D11VertexShader* pInShader,
			HRESULT InHrCreate,
			HRESULT InHrCompile, ID3D10Blob* pInBlob, ID3D10Blob* pInErrBlob
		) :
			Shader(InHrCreate, InHrCompile, pInBlob, pInErrBlob),
			_pShader(pInShader)
		{
			BOOST_ASSERT(pInShader);
		}

		PixShader::PixShader
		(
			ID3D11PixelShader* pInShader,
			HRESULT InHrCreate,
			HRESULT InHrCompile, ID3D10Blob* pInBlob, ID3D10Blob* pInErrBlob
		) :
			Shader(InHrCreate, InHrCompile, pInBlob, pInErrBlob),
			_pShader(pInShader)
		{
			BOOST_ASSERT(pInShader);
		}
} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv