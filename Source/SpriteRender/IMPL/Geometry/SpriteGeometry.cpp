#include "SpriteGeometry.h"
#include "SpriteGeometryBuffer.h"
#include "SpriteGeometryVertex.h"
#include <boost/assert.hpp>


namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

SpriteGeometry::SpriteGeometry(SpriteGeometryBuffer* pInOwnerBuffer, SpriteGeometryAllocList::iterator InItAlloc, const SSpriteGeometryData& InGeometryData) :
	_pOwnerBuffer(pInOwnerBuffer)
,   _pD3D11Buffer(nullptr) // see header file comments
,	_it_Alloc(InItAlloc)
,	_geometryData(InGeometryData)
,	_bMarkedForDelete(false)
,	_bGeometryUpdated(false) 
{
}

ID3D11Buffer* SpriteGeometry::GetBuffer() const
{
	/*
	BOOST_ASSERT_MSG
	(
		false, 
		"SpriteGeometry::GetBuffer: NOT yet implemented!!!\n"
		"We must choose the newest vertex buffer where the geometry is loaded into"
	);
	*/
	return _pD3D11Buffer;
}

UINT SpriteGeometry::GetBufferStride() const
{
	return D3D::SPRITE_VERTEX_SIZE;
}

UINT SpriteGeometry::GetBufferOffset() const
{
	return 0;
}

void SpriteGeometry::MarkSpriteAsDeletable()
{
	BOOST_ASSERT_MSG(! _bMarkedForDelete, "SpriteGeometry::MarkSpriteAsDeletable: the same sprite may NOT be marked for delete twice");
	_bMarkedForDelete = true;
	_pOwnerBuffer->MarkSpriteAsDeletable(this);
}

void SpriteGeometry::UpdateSpriteGeometry(const SSpriteGeometryData& InNewData)
{
	BOOST_ASSERT_MSG(! _bMarkedForDelete, "SpriteGeometry::UpdateSpriteGeometry: we may not update sprite geometry data if it's marked for delete");
	_bGeometryUpdated = true;
	_geometryData = InNewData;
	_pOwnerBuffer->UpdateSpriteGeometry(this, _geometryData);
}

SpriteGeometryIndex SpriteGeometry::GetD3D11BufferIndex() const
{
	return _it_Alloc->Index;
}

D3D::SDrawCall SpriteGeometry::GetD3DStripDrawCall() const
{
	return D3D::SDrawCall{ GetD3D11BufferIndex() * _pOwnerBuffer->GetVerticesPerSprite(), 4};
}

void SpriteGeometry::MarkGeometryUpdateCommitted()
{
	_bGeometryUpdated = false;
}

void SpriteGeometry::BindToD3D11Buffer(ID3D11Buffer* pInVB)
{
	_pD3D11Buffer = pInVB;
}

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv