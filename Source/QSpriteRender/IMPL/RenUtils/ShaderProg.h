#pragma once

#include "Shader.h"
#include <memory>

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

	class ShaderProg
	{
	public:
		ShaderProg(VertexShader&& InVertexShader, PixShader&& InPixShader);

		const VertexShader*                                                        GetVertex             () const          { return &_vertex; }
		const PixShader*                                                           GetPix                () const          { return &_pix; }

		VertexShader*                                                              GetVertex             () { return &_vertex; }
		PixShader*                                                                 GetPix                () { return &_pix; }

	private:
		VertexShader                                                         _vertex;
		PixShader                                                            _pix;
	};
	using ShaderProgOwnerHandle = std::unique_ptr<ShaderProg>;
	using ShaderProgHandle = ShaderProg*;

	inline ShaderProg::ShaderProg(VertexShader&& InVertexShader, PixShader&& InPixShader) :
		_vertex(std::move(InVertexShader)),
		_pix(std::move(InPixShader))
	{
	}

} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv