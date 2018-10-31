#include "RSShaderText.h"
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
	void AppendShaderText_ConstantBuffers(ShaderText* pOutText)
	{
		BOOST_ASSERT(pOutText);

		AppendShaderText_CanvasCBLayout(pOutText);
	}
} // D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv