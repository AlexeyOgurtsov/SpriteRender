#pragma once

#include "SprRenHelper_System.h"
#include "ISpriteRender/SpriteCanvasCommandInitializers.h"
#include <memory>

namespace Test
{
	class ISpriteRenderSubsystemManager;

	/**
	* Canvas, registered in the system.
	*/
	class Canvas
	{
	public:
		Canvas(ISpriteRenderSubsystemManager* pInSubsys, const MySprRen::SSpriteCanvasCreateCommandInitializer& InInitializer);

		CanvasId GetId() const;
		const MySpr::SSpriteCanvasProps& GetProps() const;
		// @TODO: Getters
		// @TODO: Updaters (from manager)

	private:	
		ISpriteRenderSubsystemManager* pSubsys = nullptr;
		MySprRen::SSpriteCanvasCreateCommandInitializer Initializer;
	};
	using CanvasHandle = std::shared_ptr<Canvas>;
} // Test