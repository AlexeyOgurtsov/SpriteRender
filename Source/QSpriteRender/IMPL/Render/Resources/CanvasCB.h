#pragma once

#include "CanvasCBLayout.h"
#include "../../RenUtils/BlobBuffer.h"

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
	struct CanvasCBInitializer
	{
		/**
		* Are we debugging the buffer now?
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

		CanvasCBInitializer
		(
			bool bInDebug,
			const std::string& InName,
			std::ofstream* pInLog,
			ID3D11Device* pInDev, ID3D11DeviceContext* pInDevCon
		) : bDebug{ bInDebug }
		,	Name{ InName }
		,	pLog{ pInLog }
		,	pDev{ pInDev }
		,	pDevCon{ pInDevCon } {}
	};

	class CanvasCB
	{
	public:
		CanvasCB(const CanvasCBInitializer& InInitializer);

		void Flush();

		void StartStore();
		void EndStore();

		void SetCB(ID3D11DeviceContext* pDevCon, UINT InSlot);

		/**
		* Sets transform matrix.
		*
		* WARNING!!! Column-first representation demanded!!!
		*/
		void SetMatrix(float InColumnFirstMatrix[4][4]);

		/**
		* Sets id of the canvas
		*/
		void SetCanvasId(Spr::SpriteCanvasId InSpriteId);

		/**
		* Sets width and height of the canvas in texels
		*/
		void SetCanvasSizeInTexels(uint32_t InWidth, uint32_t InHeight);

		/**
		* Minimum Z.
		*/
		void SetMinimumZ(float InMinimumZ);

	private:
		BlobBuffer Buffer;
	};
} // D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv