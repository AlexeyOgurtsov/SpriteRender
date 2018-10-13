#pragma once

#include "ISprPerTestFixtureBase.h"
#include "TestHelpers.h"

namespace Test::ISpr
{
	/**
	* Automatizes the following:
	* 
	* - Creates Canvas, Shows it at SetupTest time;
	* - Calls BeginUpdates()/BeginUpdateCanvas() at SetupTest time + after each CommitFrame() call;
	* - Calls EndUpdates()/EndUpdateCanvas() at CommitFrame() time + at context release time;
	*/
	class ISprPerTestFixture_SingleCanvas : public ISprPerTestFixtureBase
	{
	public:
		ISprPerTestFixture_SingleCanvas();
		~ISprPerTestFixture_SingleCanvas();

		MySprRen::ISpriteUpdater* GetUpdater() const;

		// ~ Sprite render helpers Begin
		/**
		* Creates sprite and returns its id.
		*/
		SpriteHandle CreateSprite
		(
			const MySprMath::SVec2& InPosition, 
			float InWidth, float InHeight, 
			MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState, 
			MySpr::ESpriteTransparency InTransparency = MySpr::ESpriteTransparency::Opaque
		);
		SpriteHandle CreateSprite
		(
			const MySprMath::SVec2& InPosition,
			const MySprMath::SSize& InSize,
			MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
			MySpr::ESpriteTransparency InTransparency = MySpr::ESpriteTransparency::Opaque
		);
		void DeleteSprite(SpriteHandle InHandle);
		void HideSprite(SpriteHandle InHandle);
		void ShowSprite(SpriteHandle InHandle);
		void SetSpriteTransparency(SpriteHandle InHandle, MySpr::ESpriteTransparency InTransparency);
		void SetSpriteGeometry(SpriteHandle InHandle, const MySpr::SSpriteGeometryProps& InGeometry);
		void SetSpritePosition(SpriteHandle InHandle, const MySprMath::SVec2& InPosition);
		void ResizeSprite(SpriteHandle InHandle, const MySprMath::SSize& InSize);
		void ResizeSprite(SpriteHandle InHandle, float InWidth, float InHeight);
		// ~ Sprite render helpers End

		// ~ Screen helpers Begin
		/**
		* Returns canvas point by interpolating between the canvas boundaries.
		*/
		SprVec2 PointAt(float AlphaX, float AlphaY) const;

		/**
		* Returns canvas point by interpolating between the canvas boundaries.
		*/
		SprVec2 PointAt(const SprVec2& AlphaVec) const;

		/**
		* Returns size by takings factors of width and height of the canvas
		*/
		SprSize ScreenPart(float InWidthFactor, float InHeightFactor) const;
		// ~ Screen helpers End

	protected:
		virtual void OnPostSetupTestUser() override;
		virtual void OnFrameCheckContextReleasedUser(IFrameCheckContext* pInSender) override;
		virtual void OnPreCommitFrameUser() override;

	private:
		MySprRen::ISpriteUpdater* pUpdater = nullptr;		

		CanvasHandle CanvHandle;
		constexpr static CanvasId CANV_ID = 0;
		void BeginUpdates();
		void EndUpdates();

		SprId GetSprId() { return NextSprId++; } 
		SprId NextSprId = SprId { 0 };
	};
} // Test::ISpr