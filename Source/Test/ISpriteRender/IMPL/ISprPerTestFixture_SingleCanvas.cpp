#pragma once

#include "../ISprPerTestFixture_SingleCanvas.h"
#include "../TestHelpers.h"

namespace Test::ISpr
{
	ISprPerTestFixture_SingleCanvas::ISprPerTestFixture_SingleCanvas()
	{
		T_LOG("Fixture: SingleCanvas per test fixture");
	}

	ISprPerTestFixture_SingleCanvas::~ISprPerTestFixture_SingleCanvas()
	{
		T_LOG("Fixture: SingleCanvas per test fixture: ~DTOR");
		EndUpdates();
	}

	void ISprPerTestFixture_SingleCanvas::OnPostSetupTestUser()
	{
		T_LOG("Fixture: SingleCanvas: OnPostSetupTestUser...");

		T_LOG("Fixture SingleCanvas: Creating canvas...");
		GetSprRen()->CreateCanvas(CANV_ID, GetRTWidth(), GetRTHeight());
		T_LOG("Fixture SingleCanvas: Creating canvas DONE");

		T_LOG("Fixture SingleCanvas: Showing canvas...");
		GetSprRen()->ShowCanvas(CANV_ID);
		T_LOG("Fixture SingleCanvas: showing canvas DONE");

		BeginUpdates();

		T_LOG("Fixture: SingleCanvas: OnPostSetupTestUser DONE");
	}

	void ISprPerTestFixture_SingleCanvas::OnFrameCheckContextReleasedUser(IFrameCheckContext* pInSender)
	{
		T_LOG("Fixture: SingleCanvas: OnFrameCheckContextReleasedUser...");
		BeginUpdates();
		T_LOG("Fixture: SingleCanvas: OnFrameCheckContextReleasedUser DONE");
	}

	void ISprPerTestFixture_SingleCanvas::OnPreCommitFrameUser()
	{
		T_LOG("Fixture: SingleCanvas: OnPreCommitFrameUser...");
		EndUpdates();
		T_LOG("Fixture: SingleCanvas: OnPreCommitFrameUser DONE");
	}

	void ISprPerTestFixture_SingleCanvas::BeginUpdates()
	{
		T_LOG("Fixture: SingleCanvas: BeginUpdates...");
		BOOST_ASSERT_MSG(nullptr == pUpdater, "ISprPerTestFixture_SingleCanvas::BeginUpdates(): not allowed to be called here (know that it's called automatically)");

		T_LOG("Fixture: SingleCanvas: ISpriteRender::BeginUpdates...");
		GetSprRen()->BeginUpdates();
		T_LOG("Fixture: SingleCanvas: ISpriteRender::BeginUpdates DONE");
		
		T_LOG("Fixture: SingleCanvas: ISpriteRender::BeginCanvasUpdate...");
		pUpdater = GetSprRen()->BeginCanvasUpdate(CANV_ID);
		BOOST_ASSERT_MSG(pUpdater, "ISprPerTestFixture_SingleCanvas::BeginUpdates(): ISpriteUpdater: BeginCanvasUpdate must return non-nullptr!");
		T_LOG("Fixture: SingleCanvas: ISpriteRender::BeginCanvasUpdate DONE");
		
		T_LOG("Fixture: SingleCanvas: BeginUpdates DONE");
	}

	void ISprPerTestFixture_SingleCanvas::EndUpdates()
	{
		T_LOG("Fixture: SingleCanvas: EndUpdates...");

		BOOST_ASSERT_MSG(pUpdater, "ISprPerTestFixture_SingleCanvas::EndUpdates(): not allowed to be called here (know that it's called automatically)");

		T_LOG("Fixture: SingleCanvas: ISpriteRender::EndCanvasUpdate...");
		GetSprRen()->EndCanvasUpdate(pUpdater);		
		T_LOG("Fixture: SingleCanvas: ISpriteRender::EndCanvasUpdate DONE");

		T_LOG("Fixture: SingleCanvas: ISpriteRender::EndUpdates...");
		GetSprRen()->EndUpdates();
		T_LOG("Fixture: SingleCanvas: ISpriteRender::EndUpdates DONE");

		pUpdater = nullptr;

		T_LOG("Fixture: SingleCanvas: EndUpdates DONE");
	}

	MySprRen::ISpriteUpdater* ISprPerTestFixture_SingleCanvas::GetUpdater() const
	{
		BOOST_ASSERT_MSG(pUpdater, "ISprPerTestFixture_SingleCanvas::GetUpdater: Call is NOT allowed here (maybe you use it in the check context)");
		return pUpdater;
	}

	SprId ISprPerTestFixture_SingleCanvas::CreateSprite
	(
		MySprMath::SVec2& InPosition, 
		float InWidth, float InHeight, 
		MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState, 
		MySpr::ESpriteTransparency InTransparency
	)
	{
		SprId const Id = GetSprId();
		T_LOG("Fixture: SingleCanvas: CreateSprite, Id=" << Id << "...");
		T_LOG("Position: " << ToString(InPosition));
		T_LOG("(Width*Height)=" << InWidth << "*" << InHeight);
		T_LOG("Transparency=" << ToString(InTransparency));
		GetUpdater()->CreateSprite(Id, InPosition, InWidth, InHeight, InRenderState, InTransparency);
		T_LOG("Fixture: SingleCanvas: Sprite " << Id << " Created");
		return Id;
	}

	void ISprPerTestFixture_SingleCanvas::DeleteSprite(SprId InSpriteId)
	{
		T_LOG("Fixture: SingleCanvas: DeleteSprite, Id=" << InSpriteId << "...");
		GetUpdater()->DeleteSprite(InSpriteId);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InSpriteId << " Deleted");
	}

	void ISprPerTestFixture_SingleCanvas::HideSprite(SprId InSpriteId)
	{
		T_LOG("Fixture: SingleCanvas: HideSprite, Id=" << InSpriteId << "...");
		GetUpdater()->HideSprite(InSpriteId);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InSpriteId << " Hidden");
	}

	void ISprPerTestFixture_SingleCanvas::ShowSprite(SprId InSpriteId)
	{
		T_LOG("Fixture: SingleCanvas: ShowSprite, Id=" << InSpriteId << "...");
		GetUpdater()->ShowSprite(InSpriteId);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InSpriteId << " Shown");
	}

	void ISprPerTestFixture_SingleCanvas::SetSpriteTransparency(SprId InSpriteId, MySpr::ESpriteTransparency InTransparency)
	{
		T_LOG("Fixture: SingleCanvas: SetSpriteTransparency, Id=" << InSpriteId << "...");
		T_LOG("New transparency: " << ToString(InTransparency));
		GetUpdater()->SetSpriteTransparency(InSpriteId, InTransparency);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InSpriteId << " Transparency changed");
	}
} // Test::ISpr