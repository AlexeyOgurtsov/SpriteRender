#include "CanvasCB.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{

struct SConfig;

namespace IMPL
{

namespace D3D
{
	namespace
	{
		SBlobBufferInitializer GetBlobBufferInitializer(const CanvasCBInitializer& InInitializer)
		{
			SBlobBufferInitializer Result;
			Result.bDebug = InInitializer.bDebug;
			Result.Name = InInitializer.Name;

			Result.pDev = InInitializer.pDev;
			Result.pDevCon = InInitializer.pDevCon;
			Result.pLog = InInitializer.pLog;

			Result.Usage = D3D11_USAGE_DEFAULT;
			Result.CapacityInBytes = CanvasCBLayout::SIZE_IN_BYTES;
			Result.CpuAccessFlags = 0;

			Result.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			return Result;
		}
	} // anonymous

	CanvasCB::CanvasCB(const CanvasCBInitializer& InInitializer) :
		Buffer{GetBlobBufferInitializer(InInitializer)}
	{
	}

	void CanvasCB::Flush()
	{
		Buffer.Flush();
	}

	void CanvasCB::StartStore()
	{
		Buffer.StartStore();
	}

	void CanvasCB::EndStore()
	{
		Buffer.EndStore();
	}

	void CanvasCB::SetCB(ID3D11DeviceContext* pDevCon, UINT InSlot)
	{
		Buffer.SetCB(pDevCon, InSlot);
	}

	void CanvasCB::SetMatrix(const float* pInColumnFirstMatrix)
	{
		static_assert(sizeof(float[4][4]) == CanvasCBLayout::MATRIX_SIZE_IN_BYTES);
		Buffer.SetMatrix4x4(CanvasCBLayout::MATRIX_OFFSET, pInColumnFirstMatrix);
	}

	void CanvasCB::SetCanvasId(Spr::SpriteCanvasId InSpriteId)
	{
		static_assert(sizeof(InSpriteId) == CanvasCBLayout::CANVAS_ID_SIZE_IN_BYTES);
		Buffer.SetValue(CanvasCBLayout::CANVAS_ID_OFFSET, &InSpriteId);
	}
	
	void CanvasCB::SetCanvasSizeInTexels(uint32_t InWidth, uint32_t InHeight)
	{
		static_assert(sizeof(InWidth) == CanvasCBLayout::VIEWPORT_WIDTH_SIZE_IN_BYTES);
		static_assert(sizeof(InHeight) == CanvasCBLayout::VIEWPORT_HEIGHT_SIZE_IN_BYTES);
		Buffer.SetValue(CanvasCBLayout::VIEWPORT_WIDTH_OFFSET, &InWidth);
		Buffer.SetValue(CanvasCBLayout::VIEWPORT_HEIGHT_OFFSET, &InHeight);
	}

	void CanvasCB::SetMinimumZ(float InMinimumZ)
	{
		static_assert(sizeof(InMinimumZ) == CanvasCBLayout::CANVAS_MINIMUM_Z_SIZE_IN_BYTES);
		Buffer.SetValue(CanvasCBLayout::CANVAS_MINIMUM_Z_OFFSET, &InMinimumZ);
	}
} // D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv