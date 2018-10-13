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
		void DeleteSprite(SpriteHandle InHandle);
		void HideSprite(SpriteHandle InHandle);
		void ShowSprite(SpriteHandle InHandle);
		void SetSpriteTransparency(SpriteHandle InHandle, MySpr::ESpriteTransparency InTransparency);
		// @TODO: Update sprite geometry, Resize, Move

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