#pragma once

#include "SprRenHelper_MaterialTypes.h"
#include "SprRenHelper_Canvas.h"
#include "ISpriteRender/SpriteCommandInitializers.h"
#include "SprRenHelper_ISpriteUpdaterFwd.h"
#include "SprRenHelper_MathTypes.h"
#include <memory>

namespace Test
{
	class ISpriteRenderSubsystemManager;

	/**
	* Sprite, registered in the system.
	*/
	class Sprite
	{
	public:
		Sprite(ISpriteRenderSubsystemManager* pInSubsys, CanvasHandle pInCanvas, const MySprRen::SSpriteCreateCommandInitializer& InInitializer);

		CanvasHandle GetCanvas() const { return pCanvas; }

		SprId GetId() const;

		const Handle_SprMaterialInstance& GetMatInst();
		const MySpr::SSpriteProps& GetProps() const;
		const MySpr::SSpriteGeometryProps& GetGeometry() const;
		const MySprMath::SSpriteTransform& GetTransform() const;
		const MySprMath::SVec2& GetPosition() const;
		const MySprMath::SSize& GetSize() const;
		const MySprMath::SVec2 GetCenter() const;
		const MySpr::ESpriteTransparency& GetTransparency() const;

		const Handle_SprMaterialInstance& GetInitMatInst();
		const MySpr::SSpriteProps& GetInitProps() const;

		void SetGeometry(MySprRen::ISpriteUpdater* pInUpdater, const MySpr::SSpriteGeometryProps& InGeometry);
		void Resize(MySprRen::ISpriteUpdater* pInUpdater, float InNewWidth, float InNewHeight);
		void SetPosition(MySprRen::ISpriteUpdater* pInUpdater, const SprVec2& InPosition);
		void SetMatInst(MySprRen::ISpriteUpdater* pInUpdater, const Handle_SprMaterialInstance& InInst);
		bool IsVisible() const;

		void Show(MySprRen::ISpriteUpdater* pInUpdater);
		void Hide(MySprRen::ISpriteUpdater* pInUpdater);
		void SetTransparency(MySprRen::ISpriteUpdater* pInUpdater, MySpr::ESpriteTransparency InTransparency);

	private:	
		ISpriteRenderSubsystemManager* pSubsys = nullptr;
		CanvasHandle pCanvas = nullptr;

		MySprRen::SSpriteCreateCommandInitializer Initializer;

		/**
		* WARNING! Updated only if you call methods on the Sprite itself!
		*/
		MySprRen::SSpriteCreateCommandInitializer CurrInitializer;
	};
	using SpriteHandle = std::shared_ptr<Sprite>;
} // Test