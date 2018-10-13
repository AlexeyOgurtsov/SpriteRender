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
		const Handle_SprMaterialInstance& GetInitMatInst();
		const MySpr::SSpriteProps& GetInitProps() const;

		// @TODO: Getters
		// @TODO: Updaters (from manager)
		void Resize(MySprRen::ISpriteUpdater* pInUpdater, float InNewWidth, float InNewHeight);
		void SetPosition(MySprRen::ISpriteUpdater* pInUpdater, const SprVec2& InPosition);

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