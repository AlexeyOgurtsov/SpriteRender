#pragma once

#include <d3d11.h>
#include <memory>
#include "RenUtilsSystem.h"

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

	class Shader
	{
	public: 
		Shader
		(
			HRESULT InHrCreate, 
			HRESULT InHrCompile, ID3D10Blob* pInBlob, ID3D10Blob* pInErrBlob
		);

		ID3D10Blob*                                                   GetBlob           () const                      { return _pBlob.get(); }
		ID3D10Blob*                                                   GetErrBlob        () const                      { return _pErrBlob.get(); }

		HRESULT                                                       GetHRCompile      () const { _hr_compile; }
		HRESULT                                                       GetHRCreateObj    () const { return _hr_createObj; }

	protected:
		HRESULT _hr_createObj                                         = E_FAIL;
		HRESULT _hr_compile                                           = E_FAIL;
		std::unique_ptr<ID3D10Blob, ComReleaser>               _pBlob;
		std::unique_ptr<ID3D10Blob, ComReleaser>               _pErrBlob;
	};

	class VertexShader : public Shader
	{
	public:
		VertexShader
		(
			ID3D11VertexShader* pInShader,
			HRESULT InHrCreate,
			HRESULT InHrCompile, ID3D10Blob* pInBlob, ID3D10Blob* pInErrBlob
		);

		ID3D11VertexShader*                                           GetShader() const { return _pShader.get(); }

	private:
		std::unique_ptr<ID3D11VertexShader, ComReleaser>       _pShader;
	};

	class PixShader : public Shader
	{
	public:
		PixShader
		(
			ID3D11PixelShader* pInShader,
			HRESULT InHrCreate,
			HRESULT InHrCompile, ID3D10Blob* pInBlob, ID3D10Blob* pInErrBlob
		);

		ID3D11PixelShader*                                           GetShader() const { return _pShader.get(); }

	private:
		std::unique_ptr<ID3D11PixelShader, ComReleaser>       _pShader;
	};

} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL 
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv