#pragma once

#include "SprRenHelper_System.h"
#include "ISpriteRender/SpriteCanvasCommandInitializers.h"
#include "../Utils/MathUtils.h"
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
		const MySpr::SCanvasRect& GetRect() const;
		ScreenCoordSystemDesc GetCoordSystem() const;
		int GetWidth() const;
		int GetHeight() const;
		int GetLeft() const;
		int GetTop() const;
		int GetRight() const;
		int GetBottom() const;

	private:	
		ISpriteRenderSubsystemManager* pSubsys = nullptr;
		MySprRen::SSpriteCanvasCreateCommandInitializer Initializer;
	};
	using CanvasHandle = std::shared_ptr<Canvas>;
} // Test