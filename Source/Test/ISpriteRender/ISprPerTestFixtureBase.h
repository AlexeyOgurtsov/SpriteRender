#pragma once

#include "Test/ISpriteRenderTestUtils/Fixture/PerTestFixtureBase.h"
#include "ISpriteRenderTestUtils/IFrameCheckContext.h"
#include "ISpriteRenderTestUtils/IFrameCheckContextUtils.h"
#include "ISpriteRenderTestUtils/IMPL/Utils/TextureElement.h"

namespace Test::ISpr
{
	class ISprPerTestFixtureBase : public PerTestFixtureBase
	{
	public:
		ISprPerTestFixtureBase();
		~ISprPerTestFixtureBase();

		void SetupTest(const char *InTestName, UINT InResetFlags = 0);
		
		
		/**
		* @argument InTickCount: 
		* - By default (zero passed) calls tick default number of times;
		* - If you pass negative value ticking will be disabled;
		* Otherwise calls Tick the specified number of times.
		*
		* @argument InMainLoopIterCount: Call main loop interations exactly specified number of times.
		*/
		IFrameCheckContextHandle CommitFrame(int InMainLoopIterCount = 1, int InTickCount = 0, bool bCallRender = true);
		constexpr static int DISABLE_TICK = -1; // Pass as TickCount to disable ticking (@see CommitFrame)

	protected:
		virtual IFrameCheckContextHandle CreateFrameCheckContext();

		/**
		* Called after SetupTest
		*/
		virtual void OnPostSetupTestUser() {}

		/**
		* This function is called each time before CommitFrame is called.
		*/		
		virtual void OnPreCommitFrameUser() {}

		/**
		* Render all canvasses of the sprite render.
		*/
		void RenderFrame();
		void TickN(int InTickCount);

		/**
		* Render all pickable canvasses.
		*/
		void RenderPickFrame_IfPickEnabled();

		// ~ Screen check helpers Begin
		/*
		* Returns canvas texel color at the given point.
		* The color is always returned as color in format of the render target.
		*
		* @InCanvasPoint: Point in CoordSystem of the canvas (as the sprite render takes).
		*/
		TexelColor CanvasColorAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint);

		/**
		* Returns true if canvas at the given point is clear (meaning that color matches the clear color).
		*/
		bool CanvasClearAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, bool bInMatchAlpha = false);

		/**
		* Returns true if canvas point color at the given coordinates matches the given;
		*/
		bool ColorMatchesCanvasAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, const TexelColor& InColor, bool bInMatchAlpha = false);

		/**
		* Returns true if the given screen point color matches color of the texture.
		*/
		bool TextureMatchesCanvasAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, const IMPL::TextureElement& InTextureElement, bool bInMatchAlpha = false);

		/**
		* Returns true if the given sprite is visible as a color.
		*/
		bool SpriteVisibleAsColor(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, const TexelColor& InColor, bool bInMatchAlpha = false);

		/**
		* Returns true if canvas is hidden.
		*/
		bool SpriteHidden(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, bool bInMatchAlpha = false);

		/**
		* Returns true if the given sprite is not rendered at the given canvas position.
		*
		* For example, may be used to check that sprite is moved from the given position.
		*/
		bool SpriteHiddenAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, const SprVec2& InCanvasPoint, bool bInMatchAlpha = false);
		// ~ Screen check helpers End

	private:		
		void OnPreCommitFrame();
		void OnPostSetupTest();
	};
} // Test::ISpr