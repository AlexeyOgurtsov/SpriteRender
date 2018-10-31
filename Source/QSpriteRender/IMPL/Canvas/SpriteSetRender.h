#pragma once

#include "../Render/Resources/CanvasCB.h"
#include "ISprite/SpriteCanvasProps.h"
#include <fstream>
#include <d3d11.h>

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
		class RenResources;
	}
	class SpriteManager;
	class AmbientContext;
	class Sprite;

	struct SpriteSetRenderInitializer
	{		
		bool bDebug = false;
		std::string Name;
		Dv::Spr::SpriteCanvasId CanvasId;

		AmbientContext* pAmbientContext = nullptr;
		SpriteManager* pSpriteManager = nullptr;
		const D3D::RenResources* pRenResources = nullptr;

		SpriteSetRenderInitializer
		(
			Dv::Spr::SpriteCanvasId InCanvasId,
			bool bInDebug,
			const std::string& InName,
			AmbientContext* pInAmbientContext,
			SpriteManager* pInSpriteManager,
			const D3D::RenResources* pInRenResources
		) :
			CanvasId{InCanvasId}
		,	bDebug{bInDebug}
		,	Name{InName}
		,	pAmbientContext{pInAmbientContext}
		,	pSpriteManager{pInSpriteManager}
		,	pRenResources{pInRenResources} {}
	};

	class SpriteSetRender
	{
	public:
		SpriteSetRender(const SpriteSetRenderInitializer& InInitializer);
		void Render(ID3D11DeviceContext* pInDevCon, UINT InVBSlot);

		void FlushD3D();
		void UpdateCoordSystem(const float* pMatrix44, const Spr::SCanvasCoordSystem& InCoordSystem, const SCanvasRect& InRect);
		
		SpriteManager* GetSprites() const { return pSprites; }
		const D3D::RenResources* GetRenResources() const { return pRenResources; }

		std::ofstream& GetLog() const;

	private:
		void RenderSprite(ID3D11DeviceContext* pInDevCon, const Sprite* pInSprite);

		AmbientContext* pAmbientContext = nullptr;
		SpriteManager* pSprites = nullptr;
		const D3D::RenResources* pRenResources = nullptr;

		std::unique_ptr<D3D::CanvasCB> pCB;
	};
} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv