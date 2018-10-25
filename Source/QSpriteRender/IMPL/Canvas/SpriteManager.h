#pragma once

#include "Sprite.h"
#include "../RenUtils/UniformBuffer.h"
#include "SpriteCommands.h"
#include <vector>
#include <unordered_map>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

class AmbientContext;

struct SSpriteManagerInitializer
{
	AmbientContext* pAmbientContext = nullptr;
	ID3D11Device* pDev = nullptr;
	ID3D11DeviceContext* pDevCon = nullptr;
	bool bDebug = false;
	UINT MaxSprites = 100;
	bool bAutoResizeable = false;

	SSpriteManagerInitializer(AmbientContext* pInAmbientContext, ID3D11Device* pInDev, ID3D11DeviceContext* pInDevCon, bool bInDebug);
};

/**
* Manages the vertex buffer of the sprites and accounts sprites.
*/
class SpriteManager
{
public:
	using SpriteIteratorType = SpriteListIterator;
	using VisibleSpriteIteratorType = SpriteListIterator;

	/**
	* Ctor
	*/
	SpriteManager(const SSpriteManagerInitializer& InInitializer);

	/**
	* Flushes all managed d3d resources (typically to be called each frame).
	*/
	void FlushD3D();

	/**
	* Returns the vertex buffer: must always be valid.
	*/
	ID3D11Buffer *GetVB() const;

	/**
	* Returns stride of the vertex buffer.
	*/
	UINT GetVBStride() const;

	/**
	* Setups vertex buffer as render state.
	*/
	void SetVB(ID3D11DeviceContext* pInDevCon, UINT InVBSlot);

	/**
	* Number of all sprites.
	*/
	UINT GetNumSprites() const { return SpritesByZOrder.size(); }

	/**
	* Number of visible sprites.
	*/
	UINT GetNumVisibleSprites() const { return NumVisibleSprites; }

	SpriteIteratorType Iterator_SpriteByZOrder() const;
	VisibleSpriteIteratorType Iterator_VisibleSpriteByZOrder() const;

	/**
	* Returns sprite by the given Id (or nullptr if not found yet)
	*/
	Sprite* GetSpriteById(SpriteId InId) const;

	/**
	* Create a new sprite.
	*/
	void CreateSprite(const SCreateSpriteArgs& InArgs);

	/**
	* Delete sprite by the given id.
	*/
	void DeleteSprite(SpriteId InId);

	/**
	* Show sprite by id.
	*/
	void ShowSprite(SpriteId InId);

	/**
	* Hide sprite by id.
	*/
	void HideSprite(SpriteId InId);

	/**
	* Sets z order of the given sprite to immediately follow the other sprite
	*/
	void MoveSpriteZOrderAfter(SpriteId InId, SpriteId InZBeforeSpriteId);

	/**
	* Sets a new value of the transparency for the sprite.
	*/
	void SetSpriteTransparency(SpriteId InId, ESpriteTransparency InTransparency);

	/**
	* Sets a new material instance for the sprite.
	*/
	void SetMaterialInstance(SpriteId InId, MaterialInstanceRenderStateHandle pInRenderState);

	/**
	* Sets a new geometry props for the sprite.
	*/
	void SetSpriteGeometry(SpriteId InId, const SSpriteGeometryData& InGeometry);

	/**
	* Log
	*/
	std::ofstream& GetLog() const;

private:
	using SpriteByIdCont = std::unordered_map<SpriteId, Sprite*>;

	bool bDebug = false;
	AmbientContext* pAmbientContext = nullptr;
	ID3D11Device* pDev = nullptr;
	ID3D11DeviceContext* pDevCon = nullptr;

	std::unique_ptr<D3D::UniformBuffer> pVB;

	void InitializeVB(UINT InMaxSprites, bool bInAutoResizeable);

	/**
	* Unordered storage of sprites
	*/
	SpriteStorageList Sprites;

	/**
	* Maps sprite id to sprite.
	*/
	SpriteByIdCont SpritesById;

	/**
	* Registers in the SpritesByZOrder list.
	* @returns: iterator to the inserted element in the list.
	*/
	SpriteList::iterator Register_InZOrderList(Sprite* pSprite, SpriteId InZBeforeSpriteId);

	/**
	* Unregisters the given sprite from the SpritesByZOrder list
	*/
	void Unregister_FromZOrderList(const Sprite* pSprite);

	/**
	* All sprites: visible or not, sorted by ZOrder
	*/
	SpriteList SpritesByZOrder;

	void IncrementVisibleSprites();
	void DecrVisibleSprites();
	int NumVisibleSprites = 0;
};

} // Dv::Spr::QRen::IMPL

} // Dv::Spr::QRen
} // Dv::Spr
} // Dv