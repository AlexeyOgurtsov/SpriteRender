#pragma once

#include "ISprPerTestFixture_SingleCanvas.h"
#include "ISpriteRenderTestUtils/Fixture/ResourceFixture.h"

namespace Test::ISpr
{
	class ISprPerTestFixture_SingleCanvas_SmokePick :
		public ISprPerTestFixture_SingleCanvas
	,	public ResourceFixture
	{
	public:
		ISprPerTestFixture_SingleCanvas_SmokePick();
		~ISprPerTestFixture_SingleCanvas_SmokePick();

		constexpr static MySpr::PickObjectId DEFAULT_PICK_OBJECT_ID = MySpr::MINIMAL_PICK_OBJECT_ID;

	protected:		
		// ~ Checker helpers Begin
		/**
		* Returns pick result for for the given sprite returned by the pick request.
		*/
		MySprRen::SPickResult PickSprite(const IFrameCheckContextHandle& pChecker, SpriteHandle InSprite);

		/**
		* Returns pick result for the given sprite returned by the pick request at the given canvas point.
		*
		* Canvas point is specified in the canvas coord system (NOT fract!).
		*/
		MySprRen::SPickResult PickSprite_AtCanvasPoint(const IFrameCheckContextHandle& pChecker, SpriteHandle InSprite, const SprVec2& InSpriteLBPosition_AsCanvasPoint);

		/**
		* Returns true if sprite is picked at the given canvas point.
		*
		* Canvas point is specified in the canvas coord system (NOT fract!).
		*/
		bool SpritePickedAs_AtCanvasPoint(const IFrameCheckContextHandle& pChecker, SpriteHandle InSprite, MySpr::PickObjectId InObjId, const SprVec2& InSpriteLBPosition_AsCanvasPoint);

		/**
		* Returns true if sprite is picked at the given canvas point.
		*/
		bool SpritePickedAs(const IFrameCheckContextHandle& pChecker, SpriteHandle InHandle, MySpr::PickObjectId InObjectId);

		/**
		* Returns true if sprite is not picked.
		*/
		bool SpriteNotPicked(const IFrameCheckContextHandle& pChecker, SpriteHandle InHandle);

		/**
		* Returns true if sprite is not picked at the given position.
		*
		* Canvas point is specified in the canvas coord system (NOT fract!).
		*/
		bool SpriteNotPickedAt(const IFrameCheckContextHandle& pChecker, SpriteHandle InSprite, const SprVec2& InSpriteLBPosition_AsCanvasPoint);

		/**
		* Returns true if state of all sprites is valid according to its handle.
		*/
		boost::test_tools::predicate_result CheckPickability(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites);
		boost::test_tools::predicate_result CheckAllPickable(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites);
		boost::test_tools::predicate_result CheckAllNotPickable(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites);

		// ~ Checker helpers End

		/**
		* Prepares pickable sprite (creates, registers, optionally shows (by default).
		*/
		int PreparePickSprite
		(
			TSSpriteVector* pOutSprites,
			MySpr::PickObjectId InObjectId, 
			const TSMaterial& InMaterial,
			const MySprMath::SVec2& InPosition = DEFAULT_SPRITE_CANV_SPACE_POSITION, const MySprMath::SSize& InSize = DEFAULT_SPRITE_CANV_SPACE_SIZE,
			const MySprMath::SVec2& InOrigin = MySprMath::SVec2{ 0.0F, 0.0F },
			const SpriteTransparencyMode& InTransparencyMode = SpriteTransparencyMode::Opaque,
			float InRotationAngle = 0.0F,
			MySpr::ESpritePickMode InPickMode = MySpr::ESpritePickMode::Enabled,
			bool bInShow = true
		);

		void DisableSpritePick(SpriteHandle InHandle);
		void DisableSpritePick(const TSSprite& InSprite);
		void EnableSpritePick(SpriteHandle InHandle);
		void EnableSpritePick(const TSSprite& InSprite);

		void SetSpriteObjectId(SpriteHandle InHandle, MySpr::PickObjectId InObjectId);
		void SetSpriteObjectId(const TSSprite& InSprite, MySpr::PickObjectId InObjectId);

		void AssignSpriteObjectIds(TSSpriteVector& Sprites, MySpr::PickObjectId InStartObjectId = DEFAULT_PICK_OBJECT_ID);

	private:
	};
} // Test::ISpr