#pragma once

#include <d3d11.h>

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

struct SDrawCall
{
	UINT firstVertexIndex;
	UINT numVertices;

	SDrawCall() :
		firstVertexIndex(0)
	,	numVertices(0) {}

	SDrawCall(UINT InFirstVertexIndex, UINT InNumVertices) :
		firstVertexIndex(InFirstVertexIndex)
	,	numVertices(InNumVertices) {}
};

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv