#pragma once

#include "ISprite/Core/SpriteRenderException.h"
#include "d3d11.h"
#include <boost/assert.hpp>
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

struct ComReleaser
{
	void operator() (IUnknown* pUnknown)
	{
		if (pUnknown)
		{
			pUnknown->Release();
		}
	}
};

} // Dv::Spr::QRen::IMPL::D3D
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv