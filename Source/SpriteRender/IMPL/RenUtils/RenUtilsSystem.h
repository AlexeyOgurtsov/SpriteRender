#pragma once

#include "ISprite/Core/SpriteRenderException.h"
#include "d3d11.h"
#include <memory>

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

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv