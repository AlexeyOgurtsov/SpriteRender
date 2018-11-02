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

		/**
		* Sets pick properties of the canvas.
		*
		* To be called before SetupTest!
		*/
		void SetPickProps(const MySpr::SCanvasPickProps& InPickProps);

		/**
		* Returns current pick props of the canvas!
		*/
		const MySpr::SCanvasPickProps& GetPickProps() const { return PickProps; }

		// ~ Sprite render helpers Begin
		/**
		* Creates sprite and returns its id.
		*/
		SpriteHandle CreateSprite
		(
			const MySpr::SSpritePickProps& InPickProps,
			const MySprMath::SVec2& InPosition,
			float InWidth, float InHeight,
			MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
			const SpriteTransparencyMode& InTransparencyMode = SpriteTransparencyMode::Opaque,
			const MySprMath::SVec2& InOrigin = { 0.0F, 0.0F },
			float InRotationAngle = 0.0F
		);
		SpriteHandle CreateSprite_ZOrderAfter
		(
			const SpriteHandle& InZBeforeSpriteId,
			const MySpr::SSpritePickProps& InPickProps,
			const MySprMath::SVec2& InPosition,			
			float InWidth, float InHeight,
			MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
			const SpriteTransparencyMode& InTransparencyMode = SpriteTransparencyMode::Opaque,
			const MySprMath::SVec2& InOrigin = { 0.0F, 0.0F },
			float InRotationAngle = 0.0F
		);
		SpriteHandle CreateSprite
		(
			const MySpr::SSpritePickProps& InPickProps,
			const MySprMath::SVec2& InPosition,
			const MySprMath::SSize& InSize,
			MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
			const SpriteTransparencyMode& InTransparencyMode = SpriteTransparencyMode::Opaque,
			const MySprMath::SVec2& InOrigin = { 0.0F, 0.0F },
			float InRotationAngle = 0.0F
		);
		SpriteHandle CreateSprite_ZOrderAfter
		(
			const SpriteHandle& InZBeforeSpriteId,
			const MySpr::SSpritePickProps& InPickProps,
			const MySprMath::SVec2& InPosition,
			const MySprMath::SSize& InSize,
			MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
			const SpriteTransparencyMode& InTransparencyMode = SpriteTransparencyMode::Opaque,
			const MySprMath::SVec2& InOrigin = { 0.0F, 0.0F },
			float InRotationAngle = 0.0F
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
		void SetRotationAngle(SpriteHandle InHandle, float InAngleDegs);
		void SetRotationAngle(const TSSprite& InSprite, float InAngleDegs);
		/**
		* Sets sprite new origin relative to the sprite left-bottom position.
		*/
		void SetSpriteOrigin(SpriteHandle InHandle, const MySprMath::SVec2& InNewOrigin_FromSpriteLB);
		void SetSpriteOrigin(const TSSprite& InSprite, const MySprMath::SVec2& InNewOrigin_FromSpriteLB);
		/**
		* Sets sprite new origin at the center of the sprite.
		*/
		void SetSpriteOriginToCenter(SpriteHandle InHandle);
		void SetSpriteOriginToCenter(const TSSprite& InSprite);
		void ResizeSprite(SpriteHandle InHandle, const MySprMath::SSize& InSize);
		void ResizeSprite(const TSSprite& InSprite, const MySprMath::SSize& InSize);
		void ResizeSprite(SpriteHandle InHandle, float InWidth, float InHeight);
		void ResizeSprite(const TSSprite& InSprite, float InWidth, float InHeight);
		void SetSpriteMaterial(SpriteHandle InHandle, const MySprRen::MaterialInstanceRenderStateInitializerPtr& InRenderState);
		void SetSpriteMaterial(const TSSprite& InSprite, const MySprRen::MaterialInstanceRenderStateInitializerPtr& InRenderState);
		void MoveSpriteZOrderAfter(SpriteHandle InHandle, SpriteHandle InZBeforeSpriteHandle);
		void MoveSpriteZOrderAfter(const TSSprite& InSprite, const TSSprite* InZBeforeSprite);
		void MoveSpriteZOrderOnTop(SpriteHandle InHandle);
		void MoveSpriteZOrderOnTop(const TSSprite& InSprite);
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

		/**
		* Returns true if source and dest sprites are blended properly.
		*/
		bool SpriteBlendedProperly(const IFrameCheckContextHandle& ContextHandle, const TSSprite& InSourceSprite, const TSSprite& InDestSprite, EBlendOp InBlendOp);

		/**
		* Returns true if the given sprite is blended propely with the given dest color
		*/
		bool SpriteBlendedProperly(const IFrameCheckContextHandle& ContextHandle, const TSSprite& InSourceSprite, const TexelColor& InDestColor, EBlendOp InBlendOp);

		// ~ Screen check helpers End

		// ~ TestSprite helpers Begin
		/**
		* Creates vector of TestSprite object with the given colors and assigns their positions, so they NOT overlapped;
		* WARNING!!! CreateSprite is NOT called automatically!
		*/
		TSSpriteVector LayoutSprites(const TSMaterialVector& InMaterials, const SpriteTransparencyMode& InInitTransparencyMode = SpriteTransparencyMode::Opaque, const MySpr::SSpritePickProps& InPickProps = MySpr::SSpritePickProps::Disabled);

		/**
		* Prepares sprites (creates, registers, optionally shows).
		* By default shows sprites.
		*/
		TSSpriteVector PrepareSprites(const TSMaterialVector& InMaterials, const SpriteTransparencyMode& InInitTransparencyMode = SpriteTransparencyMode::Opaque, const MySpr::SSpritePickProps& InPickProps = MySpr::SSpritePickProps::Disabled, bool bShouldShow = true);

		/**
		* Creates all sprites in the vector (by default invisible).
		*/
		void CreateSprites(TSSpriteVector& InSprites);

		/**
		* Shows all sprites in the vector.
		*/
		void ShowSprites(const TSSpriteVector& InSprites);

		/**
		* Moves all sprites to the given pos.
		*/
		void MoveAllSpritesTo(const TSSpriteVector& InSprites, const MySprMath::SVec2& InPos);

		/**
		* Prepares sprite (creates, registers, optionally shows (by default).
		*/
		int PrepareSprite_ZOrderAfter
		(
			TSSpriteVector* pOutSprites,
			TSSprite* pZBeforeSprite,
			const MySpr::SSpritePickProps& InPickProps,
			const TSMaterial& InMaterial,
			const MySprMath::SVec2& InPosition = DEFAULT_SPRITE_CANV_SPACE_POSITION, const MySprMath::SSize& InSize = DEFAULT_SPRITE_CANV_SPACE_SIZE,
			const MySprMath::SVec2& InOrigin = MySprMath::SVec2{ 0.0F, 0.0F },
			float InRotationAngle = 0.0F,
			bool bInShow = true
		);

		/**
		* Prepares sprite (creates, registers, optionally shows (by default).
		* Optionally sets transparency.
		*/
		int PrepareSprite_ZOrderAfter
		(
			TSSpriteVector* pOutSprites, 
			TSSprite* pZBeforeSprite,
			const MySpr::SSpritePickProps& InPickProps,
			const SpriteTransparencyMode& InTransparencyMode, 
			const TSMaterial& InMaterial, 
			const MySprMath::SVec2& InPosition = DEFAULT_SPRITE_CANV_SPACE_POSITION, const MySprMath::SSize& InSize = DEFAULT_SPRITE_CANV_SPACE_SIZE,
			const MySprMath::SVec2& InOrigin = MySprMath::SVec2{ 0.0F, 0.0F },
			float InRotationAngle = 0.0F,
			bool bInShow = true
		);

		/**
		* Prepares sprite (creates, registers, optionally shows (by default).
		*/
		int PrepareSprite
		(
			TSSpriteVector* pOutSprites,
			const MySpr::SSpritePickProps& InPickProps,
			const TSMaterial& InMaterial,
			const MySprMath::SVec2& InPosition = DEFAULT_SPRITE_CANV_SPACE_POSITION, const MySprMath::SSize& InSize = DEFAULT_SPRITE_CANV_SPACE_SIZE,
			const MySprMath::SVec2& InOrigin = MySprMath::SVec2{ 0.0F, 0.0F },
			float InRotationAngle = 0.0F,
			bool bInShow = true
		);

		/**
		* Prepares sprite (creates, registers, optionally shows (by default).
		* Optionally sets transparency.
		*/
		int PrepareSprite
		(
			TSSpriteVector* pOutSprites,
			const MySpr::SSpritePickProps& InPickProps,
			const SpriteTransparencyMode& InTransparencyMode,
			const TSMaterial& InMaterial,
			const MySprMath::SVec2& InPosition = DEFAULT_SPRITE_CANV_SPACE_POSITION, const MySprMath::SSize& InSize = DEFAULT_SPRITE_CANV_SPACE_SIZE,
			const MySprMath::SVec2& InOrigin = MySprMath::SVec2{ 0.0F, 0.0F },
			float InRotationAngle = 0.0F,
			bool bInShow = true
		);

		/**
		* Returns true if all sprites are either visible or not, according to current state;
		*/
		boost::test_tools::predicate_result CheckInitialVisibility(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites);
		boost::test_tools::predicate_result CheckAllHidden(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites);
		boost::test_tools::predicate_result CheckAllVisible(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites);

		// ~ TestSprite helpers End

	protected:
		virtual void OnPostSetupTestUser() override;
		virtual void OnFrameCheckContextReleasedUser(IFrameCheckContext* pInSender) override;
		virtual void OnPreCommitFrameUser() override;

		const CanvasHandle& GetCanvHandle() const { return CanvHandle; }
		CanvasId GetCanvId() const { return CANV_ID; }

	private:
		MySprRen::ISpriteUpdater* pUpdater = nullptr;		

		CanvasHandle CanvHandle;
		constexpr static CanvasId CANV_ID = 0;
		void BeginUpdates();
		void EndUpdates();		

		SprId GetSprId() { return NextSprId++; } 
		SprId NextSprId = SprId { 0 };

		MySpr::SCanvasPickProps PickProps;
	};
} // Test::ISpr