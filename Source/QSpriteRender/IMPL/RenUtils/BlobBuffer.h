#pragma once

#include "RenUtilsSystem.h"
#include "RenBufferTypes.h"
#include <fstream>
#include <vector>
#include <cstdint>

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

struct SBlobBufferInitializer
{
	/**
	* Are we debugging the blob buffer now?
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
	* Capacity in bytes.
	*/
	UINT CapacityInBytes = 0;
	
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
};


/**
* Checks the initializer structure using the asserts.
*/
void CheckBlobBufferInitializer(const SBlobBufferInitializer& InInitializer);

class BlobBuffer
{
public:
	/**
	* Creates buffer with the given default parameters.
	*
	* Buffer is created with the given default capacity, but NOT automatically flushed.
	*/
	BlobBuffer(const SBlobBufferInitializer& InInitializer);

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

	UINT GetCapacityInBytes() const;
	UINT GetBindFlags() const { return BindFlags; }
	D3D11_USAGE GetUsage() const { return Usage; }
	bool IsDynamic() const;
	UINT GetCpuAccessFlags() const { return CpuAccessFlags; }

	std::ofstream& GetLog() const;

	/**
	* Data: always has size in bytes equal to the current capacity.
	*/
	const void* GetData() const;

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
	* WARNING!!! Does NOT update data in the d3d11 buffer (@see Flush).
	*/
	void EndStore() throw(SpriteRenderException);

	/**
	* Is between StartStore()/EndStore().
	*/
	bool IsStoring() const;

	/**
	* Set constant buffer to vertex and pixel shaders.
	*/
	void SetCB(ID3D11DeviceContext* pDevCon, UINT InSlot);

	/**
	* Copies data from source to the destination.
	*
	* Must be called between StartStore/EndStore only.
	*/
	void CopyBytesToData(UINT InOffset, const void* pInSourceData, UINT InSizeInBytes);

	/**
	* Gets pointer to buffer, where to copy data into.
	*
	* Must be called between StartStore/EndStore only.
	*/
	uint8_t* GetDestBuffer(UINT InOffset = 0);

	/*
	* Helper for copying typed data from source to the destination.
	*
	* Must be called between StartStore/EndStore only.
	*/
	template<class T>
	void SetValue(UINT InOffset, const T* pInValue)
	{
		CopyBytesToData(InOffset, pInValue, sizeof(T));
	}

	/**
	* Helper for copying 3x3 matrix.
	*
	* WARNING!!! Remember 3x3 matrix representation is constant buffer is a sequence of three 4-component vectors.
	*
	* Must be called between StartStore/EndStore only.
	*/
	void SetMatrix4x4(UINT InOffset, const float* pInValue);

	/**
	* Helper for copying 4x4 matrix.
	*
	* Must be called between StartStore/EndStore only.
	*/
	void SetMatrix3x3(UINT InOffset, const float* pInValue);

	/*
	* Helper for copying typed data from source to the destination.
	*
	* Must be called between StartStore/EndStore only.
	*/
	void SetVectorFour(UINT InOffset, const float* pInValue);

	/*
	* Helper for copying typed data from source to the destination.
	*
	* Must be called between StartStore/EndStore only.
	*/
	void SetVectorThree(UINT InOffset, const float* pInValue);
	
	/*
	* Helper for copying typed data from source to the destination.
	*
	* Must be called between StartStore/EndStore only.
	*/
	void SetVector2d(UINT InOffset, const float* pInValue);


private:
	void ReCreateD3DBuffer() throw(SpriteRenderException);

	std::string BufferName;
	bool bDebug;

	bool bD3DBufferUpToDate;
	BufferHandle pBuffer;
	ID3D11Device* pDev;
	ID3D11DeviceContext* pDevCon;
	UINT BindFlags;
	D3D11_USAGE Usage;
	UINT CpuAccessFlags;
	UINT CapacityInBytes;

	bool bStoring;
	std::vector<uint8_t> Data;

	std::ofstream* pLog = nullptr;
};
void LogBufferState(std::ofstream& InLog, const BlobBuffer* pInBuffer);

} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv