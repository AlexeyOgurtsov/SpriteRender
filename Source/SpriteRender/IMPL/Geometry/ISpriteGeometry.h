#pragma once

#include "SpriteGeometryTypedefs.h"
#include "SpriteRender/IMPL/RenUtils/RenDrawCall.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

struct SSpriteGeometryData;

class ISpriteGeometry
{
public:
	virtual ~ISpriteGeometry() = default;

	virtual const SSpriteGeometryData& GetGeometryData() const = 0;

	virtual UINT GetBufferStride() const = 0;
	virtual UINT GetBufferOffset() const = 0;

	/**
	* Vertex buffer that this sprite geometry is loaded into
	* (always points to the newest vertex buffer, where the geometry data are loaded into)
	*/
	virtual ID3D11Buffer* GetBuffer() const = 0;

	/**
	* Index of sprite in the d3d11 buffer.
	*/
	virtual SpriteGeometryIndex GetD3D11BufferIndex() const = 0;

	/**
	* Draw call, using the triangle strip.
	*/
	virtual D3D::SDrawCall GetD3DStripDrawCall() const = 0;

	virtual void MarkSpriteAsDeletable() = 0;
	virtual void UpdateSpriteGeometry(const SSpriteGeometryData&) = 0;

	virtual bool IsMarkedForDelete() const = 0;
	virtual bool IsGeometryUpdated() const = 0;
};


} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren::IMPL
} // Dv::Spr
} // Dv