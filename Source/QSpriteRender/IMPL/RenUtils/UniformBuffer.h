#pragma once

#include "RenUtilsSystem.h"
#include "RenBufferTypes.h"
#include "BufferAllocManager.h"
#include <fstream>

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
	* Are we debugging the uniform buffer now?
	*/
	bool bDebug = false;

	/**
	* Name of the buffer.
	*/
	std::string Name = "{UNNAMED}";

	/**
	* Log to be used.
	*/
	std::ofstream* pLog = nullptr;

	/**
	* d3d11 device.
	*/
	ID3D11Device* pDev = nullptr;

	/**
	* d3d11 immediate device context.
	*/
	ID3D11DeviceContext* pDevCon = nullptr;

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

class UniformBuffer
{
public:
	/**
	* Creates buffer with the given default parameters.
	*
	* Vertex buffer is created with the given default capacity, but NOT automatically flushed.
	*/
	UniformBuffer(const SUniformBufferInitializer& InInitializer);

	const std::string& GetBufferName() const { return BufferName; }
	bool IsDebug() const { return bDebug; }

	ID3D11Device* GetDev() const { return pDev; }
	ID3D11DeviceContext* GetDevCon() const { return pDevCon; }

	/**
	* Get buffer: always returns valid buffer pointer;
	*/
	ID3D11Buffer* GetBuffer() const;

	/**
	* true if d3d buffer data matches data in ram.
	*/
	bool IsD3DBufferUpToDate() const { return bD3DBufferUpToDate; }

	UINT GetSlotSize() const { return SlotSize; }
	UINT GetNumSlots() const { return CapacityInSlots; }
	UINT GetCapacityInBytes() const;
	UINT GetBindFlags() const { return BindFlags; }
	D3D11_USAGE GetUsage() const { return Usage; }
	bool IsDynamic() const;
	UINT GetCpuAccessFlags() const { return CpuAccessFlags; }
	bool IsAutoResizeable() const { return bAutoResizeable; }

	std::ofstream& GetLog() const;

	/**
	* Resets capacity to a new value.
	* Capacity maybe even less than current slot count.
	*/
	void ResetCapacity(UINT InNewCapacityInSlots);

	/**
	* Clears entire contents of the buffer.
	* Should work ever if there're allocations in the buffer.
	*
	* @argument: bInResetCapacity: if true, capacity is automatically resetted to initial
	*/
	void Clear(bool bInResetCapacity = false);
	UINT GetNumFreeSlots() const;
	UINT GetNumOccupiedSlots() const;
	UINT GetNumAllocs() const;

	/**
	* Data: always has size in bytes equal to the current capacity.
	*/
	const void* GetData() const;
	
	// @TODO: Add iterator for allocs

	/**
	* Flush: submits data in RAM into the vertex buffer
	*/
	void Flush() throw(SpriteRenderException);

	/**
	* Starts storing of the data into the buffer.
	*/
	void StartStore() throw(SpriteRenderException);

	/**
	* Ends storing of the data into the buffer.
	* 
	* WARNING!!! Does NOT update data in the d3d11 vertex buffer (@see Flush).
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
	BufferAlloc Alloc(const void* pInSourceData, UINT InSizeInBytes) throw(SpriteRenderException);

	/**
	* Updates the given allocation, by reallocated its data.	
	* Size of the source data must be a multiple of the SlotSize;
	* (throws an exception if unable to occupy the data)
	* Must be called between StartStore/EndStore.
	* Offset of the allocation DO may change.
	*
	* @param: InAlloc - allocation. Will be updated.
	*/
	void Realloc(BufferAlloc& InAlloc, const void* pInSourceData, UINT InSizeInBytes) throw(SpriteRenderException);

	/**
	* Frees the allocation, so a new data may be stored in the given position.
	*
	* @param: InAlloc - allocation. Do will be updated (invalidated by changing its size to zero).
	*/
	void FreeAlloc(BufferAlloc& InAlloc);


private:
	void ReCreateD3DBuffer() throw(SpriteRenderException);

	std::string BufferName;
	bool bDebug;

	bool bD3DBufferUpToDate;
	BufferHandle pBuffer;
	ID3D11Device* pDev;
	ID3D11DeviceContext* pDevCon;
	UINT SlotSize;
	UINT CapacityInSlots;
	UINT BindFlags;
	D3D11_USAGE Usage;
	UINT CpuAccessFlags;
	bool bAutoResizeable;
	UINT InitialCapacity;

	bool bStoring;
	BufferAllocManager Allocs;
	std::vector<uint8_t> Data;

	std::ofstream* pLog = nullptr;
};
void LogBufferState(std::ofstream& InLog, const UniformBuffer* pInBuffer);

} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv