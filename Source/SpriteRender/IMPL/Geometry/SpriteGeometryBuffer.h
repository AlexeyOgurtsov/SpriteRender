#pragma once

#include "SpriteGeometryAllocManager.h"
#include "ISpriteGeometryBufferUpdateInterface.h"
#include "SpriteRender/IMPL/RenUtils/RenBufferTypes.h"
#include <vector>

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

/**
* Result of the update of the geometry buffer.
*/
struct SSpriteGeometryUpdateResource
{
	ID3D11Buffer* pVertexBuffer;

	SSpriteGeometryUpdateResource() :
		pVertexBuffer(nullptr) {}
	SSpriteGeometryUpdateResource(ID3D11Buffer* pInVertexBuffer) :
		pVertexBuffer(pInVertexBuffer) {}
};

struct SSpriteGeometryBufferInitializer
{
	ID3D11Device* pDev;
	int InitialCapacity;

	SSpriteGeometryBufferInitializer(ID3D11Device* pInDev, int InInitialCapacity) :
		pDev{ pInDev }
	,	InitialCapacity{InInitialCapacity} {}
};

/**
* Stores the update state of the buffer.
*/
struct SSpriteGeometryBufferUpdate
{
	/**
	* References all added geometries that are not yet committed to the buffer.
	* To be cleared every commit of the added geometries.
	*/
	std::vector<SpriteGeometry*> NewGeometriesToCommit;

	/**
	* References all deleted or updated geometries.
	* If more than one update, or both update and delete
	* are performed on the geometry, we need to see it as a single operation.
	* To be cleared every commit of the deleted or updated geometries.
	*/
	std::vector<SpriteGeometry*> DeletedOrUpdatedGeometries;
};

/**
* D3D11 buffers for working with the sprite geometry
*/
class SpriteGeometryBuffer : public ISpriteGeometryBufferUpdateInterface
{
public:
	SpriteGeometryBuffer(const SSpriteGeometryBufferInitializer& InInitializer);

	ID3D11Buffer* GetBuffer() const;
	ID3D11Buffer* GetOldBuffer() const;

	/**
	* See the corresponding interface for help.
	*/
	virtual ISpriteGeometry* AddSprite(const SSpriteGeometryData& InGeometryData) override;

	/**
	* Commits added sprite updates to the underlying d3d11 buffers.
	*/
	SSpriteGeometryUpdateResource CommitAdded(ID3D11DeviceContext* pInDevCon);

	/**
	* Delete all geometries loaded into the old VB
	* and commit all updates of the geometries to the VB.
	* WARNING!!! Check that this geometries are NOT used by the rendering code.
	*/
	void CommitDeletesAndUpdates(ID3D11DeviceContext* pInDevCon);

	/**
	* Checks that old resources are reset
	*/
	bool AreOldResourcesResetted() const;

	/**
	* Get capacity of the sprite buffer in sprites.
	* This is the capacity that is to be committed the next frame.
	*/
	int GetCapacityToCommit() const;

	/**
	* Get count of sprites corrently stored in the buffer.
	*/
	int GetNumSprites() const;

	/**
	* Get count of vertices per sprite in the buffer.
	*/
	unsigned int GetVerticesPerSprite() const;

	/**
	* Get update of the sprites.
	*/
	const SSpriteGeometryBufferUpdate& GetUpdate() const { return _update; }

	/**
	* Hint capacity of the sprite buffer in sprites.
	* (Does NOT necessarily changes the capacity).
	*/
	void HintCapacity(int InNewCapacity);

	/**
	* Marks sprite as deletable.
	* For internal use only.
	*/
	void MarkSpriteAsDeletable(SpriteGeometry* pSpriteGeometry);
	
	/**
	* Updates sprite geometry.
	* For internal use only. 
	*/
	void UpdateSpriteGeometry(SpriteGeometry* pSpriteGeometry, const SSpriteGeometryData& InGeometryData);

private:
	void SetupNewBuffer(int InNewCapacity);
	void ReleaseOldResources();

	void _ChangeCapacityToCommit(int InNewCapacity);
	SpriteGeometry* CreateSpriteGeometry(const SReservedGeometryAlloc& InReservedSprite, const SSpriteGeometryData& InGeometryData);

	ID3D11Device* _pDev;
	int GetBufferSizeInSprites() const;
	D3D::BufferHandle* GetBufferHandle();
	const D3D::BufferHandle* GetBufferHandle() const;
	D3D::BufferHandle* GetOldBufferHandle();
	const D3D::BufferHandle* GetOldBufferHandle() const;
	void FlipBuffers();
	D3D::BufferHandle _pVB[2];
	int _bufferSizeInSprites[2];
	int _capacityToCommit;
	int _bufferHandleIndex;
	int _oldBufferHandleIndex;

	SpriteGeometryAllocManager _allocManager;
	SSpriteGeometryBufferUpdate _update;
};

} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv