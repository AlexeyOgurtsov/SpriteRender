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

		/**
		* Unique Id of the canvas.
		*/
		CanvasId GetId() const;

		/**
		* Name of the canvas
		*/
		const std::string& GetName() const;

		/**
		* Is this canvas is debug mode?
		*/
		bool IsDebug() const;		

		/**
		* Parameters of picking for the given canvas.
		*/
		const MySpr::SCanvasPickProps& GetPickProps() const;

		/**
		* Can be picked according to settings.
		*/
		bool CanBePicked() const;

		/**
		* Current capacity in sprites.
		*/
		int GetCapacityInSprites() const;

		/**
		* Can maximum number of sprites that this canvas holds be resized automatically?
		*/
		bool IsAutoResize() const;

		/**
		* Is visible.
		*/
		bool IsVisible() const;

		/**
		* Is hidden.
		*/
		bool IsHidden() const;

		/**
		* Show the canvas.
		*/
		void Show();

		/**
		* Hide the canvas.
		*/
		void Hide();

		/**
		* Enable picking.
		*/
		void EnablePicking();

		/**
		* Disable picking.
		*/
		void DisablePicking();

		const MySpr::SSpriteCanvasProps& GetProps() const;

		const MySpr::SCanvasRect& GetRTRect() const;
		void SetRTRect(const MySpr::SCanvasRect& InRect);

		ScreenCoordSystemDesc GetCoordSystem() const;
		void SetCoordSystem(const MySpr::SCanvasCoordSystem& InCoordSystem);

		/**
		* Width of the RTRect.
		*/
		int GetRTWidth() const;

		/**
		* Height of the RTRect.
		*/
		int GetRTHeight() const;

		/**
		* Left position of the RTRect.
		*/
		int GetRTLeft() const;

		/**
		* Top position of the RTRect.
		*/
		int GetRTTop() const;

		/**
		* Right position of the RTRect.
		*/
		int GetRTRight() const;

		/**
		* Bottom position of the RTRect.
		*/
		int GetRTBottom() const;

		/**
		* Aspect width over height.
		*/
		float GetAspectWidthOverHeight() const;

	private:	
		ISpriteRenderSubsystemManager* pSubsys = nullptr;
		MySprRen::SSpriteCanvasCreateCommandInitializer Initializer;
	};
	using CanvasHandle = std::shared_ptr<Canvas>;
} // Test