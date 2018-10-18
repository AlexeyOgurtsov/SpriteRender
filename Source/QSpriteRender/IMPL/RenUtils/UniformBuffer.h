#pragma once

#include "RenUtilsSystem.h"
#include "RenBufferTypes.h"

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

struct SUniformBufferInitializer
{
	/**
	* Size of single chunk of data
	*/
	UINT SlotSize = 0;

	/**
	* Capacity in slots.
	*/
	UINT CapacityInSlots = 0;
	
	/**
	* Bind flags.
	*/
	UINT BindFlags = 0;

	/**
	* Usage
	*/
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;

	/**
	* Cpu access flags
	*/
	UINT CpuAccessFlags = 0;

	/**
	* True if buffer can change its size automatically, when it needs to.
	*/
	bool bAutoResizeable = false;
};

/**
* Checks the initializer structure using the asserts.
*/
void CheckUniformBufferInitializer(const SUniformBufferInitializer& InInitializer);

struct UniformBufferAlloc
{
	/**
	* Index of the first datum stored in the buffer.
	* Really offset in slots from the start of the buffer.
	*/
	UINT OffsetInSlots = 0;

	/**
	* Number of slots that this allocation occupies.
	*/
	UINT NumSlots = 0;

	UniformBufferAlloc() = default;
	UniformBufferAlloc(UINT InOffsetInSlots, UINT InNumSlots) :
		OffsetInSlots{InOffsetInSlots}
	,	NumSlots{InNumSlots} {}

	void Invalidate() { OffsetInSlots = 0; NumSlots = 0; }
};

class UniformBuffer
{
public:
	UniformBuffer(const SUniformBufferInitializer& InInitializer);

	ID3D11Buffer* GetBuffer() const { return pBuffer.get(); }

	UINT GetSlotSize() const { return SlotSize; }
	UINT GetCapacityInSlots() const { return CapacityInSlots; }
	UINT GetBindFlags() const { return BindFlags; }
	D3D11_USAGE GetUsage() const { return Usage; }
	UINT GetCpuAccessFlags() const { return CpuAccessFlags; }
	bool IsAutoResizeable() const { return bAutoResizeable; }

	void ResetCapacity(UINT InCapacityInSlots);
	void Clear();

	UINT GetNumFreeSlots() const;
	UINT GetNumOccupiedSlots() const;
	UINT GetNumAllocs() const;
	
	// @TODO: Add iterator for allocs

	/**
	* Starts storing of the data into the buffer.
	*/
	void StartStore() throw(SpriteRenderException);

	/**
	* Ends storing of the data into the buffer.
	*/
	void EndStore() throw(SpriteRenderException);

	/**
	* Is between StartStore()/EndStore().
	*/
	bool IsStoring() const;

	/**
	* Store one or multiple datum(s) into the buffer, so all datums are stored consecutively.
	* Size of the source data must be a multiple of the SlotSize.
	* Must be called between StartStore/EndStore.
	*
	* Performs automatic resize if bAutoResizeable is true.
	* If autoresize is performed, old allocations never change their offsets in the buffer.
	* Throws an exception if autoresize is disabled and unable to allocate.
	*
	* @returns: allocation that was created.
	*/
	UniformBufferAlloc Alloc(const void* pInSourceData, UINT InSizeInBytes) throw(SpriteRenderException);

	/**
	* Updates the given allocation, by reallocated its data.	
	* Size of the source data must be a multiple of the SlotSize;
	* (throws an exception if unable to occupy the data)
	* Must be called between StartStore/EndStore.
	* Offset of the allocation DO may change.
	*
	* @param: InAlloc - allocation. Will be updated.
	*/
	void Realloc(UniformBufferAlloc& InAlloc, const void* pInSourceData, UINT InSizeInBytes) throw(SpriteRenderException);

	/**
	* Frees the allocation, so a new data may be stored in the given position.
	*
	* @param: InAlloc - allocation. Do will be updated (invalidated by changing its size to zero).
	*/
	void FreeAlloc(UniformBufferAlloc& InAlloc);


private:
	BufferHandle pBuffer;
	UINT SlotSize;
	UINT CapacityInSlots;
	UINT BindFlags;
	D3D11_USAGE Usage;
	UINT CpuAccessFlags;
	bool bAutoResizeable;
};

} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv