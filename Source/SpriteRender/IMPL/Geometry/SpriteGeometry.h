#pragma once

#include "ISpriteGeometry.h"
#include "SpriteGeometryAllocManagerList.h"
#include "SpriteGeometryData.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

class SpriteGeometryBuffer;

class SpriteGeometry : public ISpriteGeometry
{
public:
	/**
	* Constructor
	*
	* D3d11 buffer link will be initialized as nullptr because the geometry is NOT loaded yet into any buffer.
	*/
	SpriteGeometry(SpriteGeometryBuffer* pInOwnerBuffer, SpriteGeometryAllocList::iterator InItAlloc, const SSpriteGeometryData& InGeometryData);

	virtual ID3D11Buffer* GetBuffer() const override;
	virtual UINT GetBufferStride() const override;
	virtual UINT GetBufferOffset() const override;

	SpriteGeometryAllocList::iterator GetItAlloc() const { return _it_Alloc; }

	virtual const SSpriteGeometryData& GetGeometryData() const override { return _geometryData; }

	virtual void MarkSpriteAsDeletable() override;
	virtual void UpdateSpriteGeometry(const SSpriteGeometryData& InNewData) override;

	virtual SpriteGeometryIndex GetD3D11BufferIndex() const override;

	virtual D3D::SDrawCall GetD3DStripDrawCall() const override;

	virtual bool IsMarkedForDelete() const override { return _bMarkedForDelete; }
	virtual bool IsGeometryUpdated() const override { return _bGeometryUpdated; }

	/**
	* For internal usage of the SpriteGeometryBuffer only:
	* We must mark that the geometry has been updated. 
	*/
	void MarkGeometryUpdateCommitted();
	void BindToD3D11Buffer(ID3D11Buffer* pInVB);

private:
	SpriteGeometryBuffer* _pOwnerBuffer;
	ID3D11Buffer* _pD3D11Buffer;
	SpriteGeometryAllocList::iterator _it_Alloc;
	SSpriteGeometryData _geometryData;
	bool _bMarkedForDelete;
	bool _bGeometryUpdated;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv