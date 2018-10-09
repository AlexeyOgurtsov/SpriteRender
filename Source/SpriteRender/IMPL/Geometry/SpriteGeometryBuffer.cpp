#include "SpriteGeometryBuffer.h"
#include "SpriteGeometryVertex.h"
#include "SpriteGeometry.h"
#include "SpriteRender/IMPL/RenUtils/RenBufferUtils.h"
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

void CommitSpriteGeometryToVB(SpriteGeometry* pInGeometry, void* pMappedData)
{
	uint8_t* const pSpriteGeometryMappedData = D3D::SPRITE_GEOMETRY_SIZE * pInGeometry->GetD3D11BufferIndex() + static_cast<uint8_t*>(pMappedData);

	const SSpriteGeometryData& geo = pInGeometry->GetGeometryData();

	float const LEFT_X = geo.PositionX;
	float const RIGHT_X = geo.PositionX + 2.0F * geo.HalfWidth;
	float const TOP_Y = geo.PositionY + 2.0F * geo.HalfHeight; 
	float const DOWN_Y = geo.PositionY;

	D3D::SetD3D11SpriteVertex
	(
		pSpriteGeometryMappedData,
		LEFT_X, TOP_Y, geo.Z,
		geo.TexCoordX[0], geo.TexCoordY[0] 
	);
	D3D::SetD3D11SpriteVertex
	(
		pSpriteGeometryMappedData + D3D::SPRITE_VERTEX_SIZE, 
		RIGHT_X, TOP_Y, geo.Z,
		geo.TexCoordX[1], geo.TexCoordY[1] 
	);
	D3D::SetD3D11SpriteVertex
	(
		pSpriteGeometryMappedData + D3D::SPRITE_VERTEX_SIZE * 2, 
		LEFT_X, DOWN_Y, geo.Z,
		geo.TexCoordX[2], geo.TexCoordY[2] 
	);
	D3D::SetD3D11SpriteVertex
	(
		pSpriteGeometryMappedData + D3D::SPRITE_VERTEX_SIZE * 3, 
		RIGHT_X, DOWN_Y, geo.Z,
		geo.TexCoordX[3], geo.TexCoordY[3] 
	);
}

void CommitUpdatedGeometriesToVB(ID3D11DeviceContext* pInDevCon, const SSpriteGeometryBufferUpdate& InBufferUpdate, ID3D11Buffer* pInVB)
{
	BOOST_ASSERT_MSG(pInVB, "CommitUpdatedGeometriesToVB: vertex buffer must be valid when using this function");
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	HRESULT hr_map = pInDevCon->Map(pInVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedSubresource);
	if (FAILED(hr_map))
	{
		throw SpriteRenderHRFailure("CommitUpdatedGeometriesToVB: Failed to map the vertex buffer", hr_map);
	}
	for(SpriteGeometry* pGeometry : InBufferUpdate.DeletedOrUpdatedGeometries)
	{
		if( ! pGeometry->IsMarkedForDelete() )
		{
			CommitSpriteGeometryToVB(pGeometry, mappedSubresource.pData);
		}
	}
	pInDevCon->Unmap(pInVB, 0);
}

void CommitAddedGeometriesToVB(ID3D11DeviceContext* pInDevCon, const SSpriteGeometryBufferUpdate& InBufferUpdate, ID3D11Buffer* pInVB)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	HRESULT hr_map = pInDevCon->Map(pInVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedSubresource);
	if (FAILED(hr_map))
	{
		throw SpriteRenderHRFailure("CommitAddedGeometriesToVB: Failed to map the vertex buffer", hr_map);
	}
	for(SpriteGeometry* pGeometry : InBufferUpdate.NewGeometriesToCommit)
	{
		CommitSpriteGeometryToVB(pGeometry, mappedSubresource.pData);
	}
	pInDevCon->Unmap(pInVB, 0);
}

SpriteGeometryBuffer::SpriteGeometryBuffer(const SSpriteGeometryBufferInitializer& InInitializer) :
	_pDev(InInitializer.pDev)
,	_bufferHandleIndex(0)
,	_oldBufferHandleIndex(1) // WARNING!!!: Very important to initialize old buffer with ONE
,	_capacityToCommit(0)
{
	BOOST_ASSERT(_pDev);
	BOOST_ASSERT_MSG(_oldBufferHandleIndex != _bufferHandleIndex, "SpriteGeometryBuffer ctor: buffer and old buffer indices must differ");
	_ChangeCapacityToCommit(InInitializer.InitialCapacity);
	_bufferSizeInSprites[0] = _bufferSizeInSprites[1] = 0;
}

bool SpriteGeometryBuffer::AreOldResourcesResetted() const
{
	return GetOldBufferHandle()->get() == nullptr;
}

int SpriteGeometryBuffer::GetCapacityToCommit() const
{
	return _capacityToCommit;
}

void SpriteGeometryBuffer::HintCapacity(int InNewCapacity)
{
	// Here we only alltow to change capacity, if the old capacity is less,
	// to prevent slicing the old sprites.
	if(GetNumSprites() < InNewCapacity)
	{
		_ChangeCapacityToCommit(InNewCapacity);
	}
}

ISpriteGeometry* SpriteGeometryBuffer::AddSprite(const SSpriteGeometryData& InGeometryData)
{
	// Grow the capacity if necessary
	int NewMinimalRequiredCapacity = GetNumSprites() + 1;
	if(_allocManager.GetCapacity() < NewMinimalRequiredCapacity)
	{
		_allocManager.SetNewCapacity(NewMinimalRequiredCapacity);
		if(GetCapacityToCommit() < _allocManager.GetCapacity())
		{
			_ChangeCapacityToCommit(NewMinimalRequiredCapacity);
		}
	}
	
	// Perform the creation and allocation
	SReservedGeometryAlloc reservedSprite = _allocManager.Reserve();
	SpriteGeometry* pSprite = CreateSpriteGeometry(reservedSprite, InGeometryData);
	_allocManager.Allocate(reservedSprite, pSprite);

	// Register the sprite as update
	_update.NewGeometriesToCommit.push_back(pSprite);

	return pSprite;
}

SSpriteGeometryUpdateResource SpriteGeometryBuffer::CommitAdded(ID3D11DeviceContext* pInDevCon)
{
	// We should resize the buffer, if the current buffer is not enough
	bool bShouldCreateNewBuffer = (GetBufferSizeInSprites() < GetCapacityToCommit());
	if(bShouldCreateNewBuffer)
	{
		BOOST_ASSERT(AreOldResourcesResetted());

		// Setup new buffer
		SetupNewBuffer(GetCapacityToCommit());

		// We must commit updated geometries to the VB
		CommitUpdatedGeometriesToVB(pInDevCon, GetUpdate(), GetBuffer());
	}
	CommitAddedGeometriesToVB(pInDevCon, GetUpdate(), GetBuffer());
	for (SpriteGeometry* pSpriteGeometry : _update.NewGeometriesToCommit)
	{
		pSpriteGeometry->BindToD3D11Buffer(GetBuffer());
	}
	_update.NewGeometriesToCommit.clear();
	return SSpriteGeometryUpdateResource{GetBuffer()};
}

void SpriteGeometryBuffer::CommitDeletesAndUpdates(ID3D11DeviceContext* pInDevCon)
{
	if (_update.DeletedOrUpdatedGeometries.empty())
	{
		return;
	}
	CommitUpdatedGeometriesToVB(pInDevCon, _update, GetBuffer());
	for(SpriteGeometry* pSpriteGeometry : _update.DeletedOrUpdatedGeometries)
	{
		pSpriteGeometry->MarkGeometryUpdateCommitted();
	}
	_update.DeletedOrUpdatedGeometries.clear();
	_allocManager.DeleteAllPending();
	ReleaseOldResources();
}

void SpriteGeometryBuffer::ReleaseOldResources()
{
	if(GetOldBufferHandle()->get() != nullptr)
	{
		GetOldBufferHandle()->reset();
	}
}

void SpriteGeometryBuffer::UpdateSpriteGeometry(SpriteGeometry* pSpriteGeometry, const SSpriteGeometryData& InGeometryData)
{
	BOOST_ASSERT(pSpriteGeometry);
	BOOST_ASSERT_MSG(pSpriteGeometry->IsMarkedForDelete(), "We may NOT update sprite geometry because it is marked for delete");
	// We do NOT overwrite the content of the VB here, because the content of the VB
	// is to be updated on the commit only.

	// Here we preventing from inserting the same geometry more than once
	// (as the same geometry may be both updated and deleted at the same time)
	if(! pSpriteGeometry->IsGeometryUpdated())
	{
		_update.DeletedOrUpdatedGeometries.push_back(pSpriteGeometry);
	}
}

void SpriteGeometryBuffer::MarkSpriteAsDeletable(SpriteGeometry* pSpriteGeometry)
{
	BOOST_ASSERT(pSpriteGeometry);
	// WARNING!!! At this time sprite geometry is already marked for delete by the handle sprite geometry class.
	_allocManager.MarkForDelete(pSpriteGeometry);

	// Here we preventing from inserting the same geometry more than once
	// (as the same geometry may be both updated and deleted at the same time)
	if(! pSpriteGeometry->IsGeometryUpdated())
	{
		_update.DeletedOrUpdatedGeometries.push_back(pSpriteGeometry);
	}
}

SpriteGeometry* SpriteGeometryBuffer::CreateSpriteGeometry(const SReservedGeometryAlloc& InReservedSprite, const SSpriteGeometryData& InGeometryData)
{
	return new SpriteGeometry(this, InReservedSprite.it_Alloc, InGeometryData);
}

void SpriteGeometryBuffer::_ChangeCapacityToCommit(int InNewCapacity)
{
	_capacityToCommit = InNewCapacity;
}

void SpriteGeometryBuffer::SetupNewBuffer(int InNewCapacity)
{
	FlipBuffers();
	D3D::BufferHandle* ppNewBuffer = GetBufferHandle();
	const bool bDynamicBuffer = true;
	*ppNewBuffer = D3D::CreateVertexBuffer(_pDev, InNewCapacity * D3D::SPRITE_GEOMETRY_SIZE, bDynamicBuffer);
	_bufferSizeInSprites[_bufferHandleIndex] = InNewCapacity;
}

int SpriteGeometryBuffer::GetNumSprites() const
{
	return _allocManager.GetNumSprites();
}

unsigned int SpriteGeometryBuffer::GetVerticesPerSprite() const
{
	return 4;
}

D3D::BufferHandle* SpriteGeometryBuffer::GetBufferHandle()
{
	return &_pVB[_bufferHandleIndex];
}

const D3D::BufferHandle* SpriteGeometryBuffer::GetBufferHandle() const
{
	return &_pVB[_bufferHandleIndex];
}

ID3D11Buffer* SpriteGeometryBuffer::GetBuffer() const
{
	return GetBufferHandle()->get();
}

D3D::BufferHandle* SpriteGeometryBuffer::GetOldBufferHandle()
{
	return &(_pVB[_oldBufferHandleIndex]);
}

const D3D::BufferHandle* SpriteGeometryBuffer::GetOldBufferHandle() const
{
	return &(_pVB[_oldBufferHandleIndex]);
}

ID3D11Buffer* SpriteGeometryBuffer::GetOldBuffer() const
{
	return GetOldBufferHandle()->get();
}

void SpriteGeometryBuffer::FlipBuffers()
{
	std::swap(_oldBufferHandleIndex, _bufferHandleIndex);
}

int SpriteGeometryBuffer::GetBufferSizeInSprites() const
{
	return _bufferSizeInSprites[_bufferHandleIndex];
}


} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv
