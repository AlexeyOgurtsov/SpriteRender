#pragma once

#include "ISprPerTestFixtureBase.h"
#include "TestHelpers.h"
#include "ISpriteRenderTestUtils/Utils/TestTypes.h"

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
		SpriteHandle CreateSprite_ZOrderAfter
		(
			const SpriteHandle& InZBeforeSpriteId,
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
		SpriteHandle CreateSprite_ZOrderAfter
		(
			const SpriteHandle& InZBeforeSpriteId,
			const MySprMath::SVec2& InPosition,
			const MySprMath::SSize& InSize,
			MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
			MySpr::ESpriteTransparency InTransparency = MySpr::ESpriteTransparency::Opaque
		);		
		void CreateSprite(TSSprite& InSprite);
		void DeleteSprite(SpriteHandle InHandle);
		void DeleteSprite(TSSprite& InSprite);
		void HideSprite(SpriteHandle InHandle);
		void HideSprite(const TSSprite& InSprite);
		void ShowSprite(SpriteHandle InHandle);
		void ShowSprite(const TSSprite& InSprite);
		void SetSpriteTransparency(SpriteHandle InHandle, MySpr::ESpriteTransparency InTransparency);
		void SetSpriteTransparency(const TSSprite& InSprite, MySpr::ESpriteTransparency InTransparency);
		void SetSpriteGeometry(SpriteHandle InHandle, const MySpr::SSpriteGeometryProps& InGeometry);
		void SetSpriteGeometry(const TSSprite& InSprite, const MySpr::SSpriteGeometryProps& InGeometry);
		void SetSpritePosition(SpriteHandle InHandle, const MySprMath::SVec2& InPosition);
		void SetSpritePosition(const TSSprite& InSprite, const MySprMath::SVec2& InPosition);
		void ResizeSprite(SpriteHandle InHandle, const MySprMath::SSize& InSize);
		void ResizeSprite(const TSSprite& InSprite, const MySprMath::SSize& InSize);
		void ResizeSprite(SpriteHandle InHandle, float InWidth, float InHeight);
		void ResizeSprite(const TSSprite& InSprite, float InWidth, float InHeight);
		void SetSpriteMaterial(SpriteHandle InHandle, const MySprRen::MaterialInstanceRenderStateInitializerPtr& InRenderState);
		void SetSpriteMaterial(const TSSprite& InSprite, const MySprRen::MaterialInstanceRenderStateInitializerPtr& InRenderState);
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

		// ~ Screen check helpers Begin
		/*
		* Returns canvas texel color at the given point.
		* The color is always returned as color in format of the render target.
		*
		* @InCanvasPoint: Point in CoordSystem of the canvas (as the sprite render takes).
		*/
		TexelColor ScreenColorAt(const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint);

		/**
		* Returns true if canvas at the given point is clear (meaning that color matches the clear color).
		*/
		bool ScreenClearAt(const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint);

		/**
		* Returns true if canvas point color at the given coordinates matches the given;
		*/
		bool ColorMatchesScreenAt(const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, const TexelColor& InColor);

		/**
		* Returns true if the given screen point color matches color of the texture.
		*/
		bool TextureMatchesScreenAt(const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, const IMPL::TextureElement& InTextureElement);

		/**
		* Returns true if the given sprite is visible as a color.
		*/
		bool SpriteVisibleAsColor(const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, const TexelColor& InColor);

		/**
		* Returns true if sprite is hidden.
		*/
		bool SpriteHidden(const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite);

		/**
		* Returns true if sprite is hidden.
		*/
		bool SpriteHidden(const IFrameCheckContextHandle& ContextHandle, const TSSprite& InSprite);

		/**
		* Returns true if the given sprite is not rendered at the given canvas position.
		*
		* For example, may be used to check that sprite is moved from the given position.
		*/
		bool SpriteHiddenAt(const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, const SprVec2& InCanvasPoint);

		/**
		* Returns true if the given sprite is not rendered at the given canvas position.
		*
		* For example, may be used to check that sprite is moved from the given position.
		*/
		bool SpriteHiddenAt(const IFrameCheckContextHandle& ContextHandle, const TSSprite& InSprite, const SprVec2& InCanvasPoint);

		// ~ Screen check helpers End

		// ~ TestSprite helpers Begin
		/**
		* Creates vector of TestSprite object with the given colors and assigns their positions, so they NOT overlapped;
		* WARNING!!! CreateSprite is NOT called automatically!
		*/
		TSSpriteVector LayoutSprites(const TSMaterialVector& InMaterials);

		/**
		* Prepares sprites (creates, registers, optionally shows).
		* By default shows sprites.
		*/
		TSSpriteVector PrepareSprites(const TSMaterialVector& InMaterials, bool bShouldShow = true);

		/**
		* Creates all sprites in the vector.
		*/
		void CreateSprites(TSSpriteVector& InSprites);

		/**
		* Shows all sprites in the vector.
		*/
		void ShowSprites(const TSSpriteVector& InSprites);

		boost::test_tools::predicate_result CheckVisibility(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites);
		// ~ TestSprite helpers End

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