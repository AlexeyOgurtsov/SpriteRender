#pragma once

#include "../ISprPerTestFixture_SingleCanvas.h"
#include "ISpriteRenderTestUtils/ISpriteRenderSubsystemManager.h"
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
		CanvHandle = GetSpriteRenderSubsystemManager()->CreateCanvas(CANV_ID, GetRTWidth(), GetRTHeight());
		T_LOG("Fixture SingleCanvas: Creating canvas DONE");

		T_LOG("Fixture SingleCanvas: Showing canvas...");
		GetSpriteRenderSubsystemManager()->ShowCanvas(CanvHandle);
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

	SpriteHandle ISprPerTestFixture_SingleCanvas::CreateSprite_ZOrderAfter
	(
		const SpriteHandle& InZBeforeSpriteId,
		const MySprMath::SVec2& InPosition,		
		float InWidth, float InHeight,
		MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
		MySpr::ESpriteTransparency InTransparency
	)
	{
		SprId const Id = GetSprId();
		T_LOG("Fixture: SingleCanvas: CreateSprite_ZOrderAfter, Id=" << Id << "...");
		MySpr::SpriteId ZBeforeSpriteId = ((InZBeforeSpriteId != nullptr) ? InZBeforeSpriteId->GetId() : MySpr::NULL_SPRITE_ID);
		SpriteHandle Handle = GetSpriteRenderSubsystemManager()->CreateSprite_ZOrderAfter(Id, ZBeforeSpriteId, GetUpdater(), CanvHandle, InPosition, InWidth, InHeight, InRenderState, InTransparency);
		T_LOG("Fixture: SingleCanvas: Sprite " << Id << " Created");
		return Handle;
	}

	SpriteHandle ISprPerTestFixture_SingleCanvas::CreateSprite
	(
		const MySprMath::SVec2& InPosition, 
		float InWidth, float InHeight, 
		MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState, 
		MySpr::ESpriteTransparency InTransparency
	)
	{
		return CreateSprite_ZOrderAfter(nullptr, InPosition, InWidth, InHeight, InRenderState, InTransparency);
	}

	SpriteHandle ISprPerTestFixture_SingleCanvas::CreateSprite_ZOrderAfter
	(
		const SpriteHandle& InZBeforeSpriteId,
		const MySprMath::SVec2& InPosition,
		const MySprMath::SSize& InSize,
		MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
		MySpr::ESpriteTransparency InTransparency
	)
	{
		return CreateSprite_ZOrderAfter(InZBeforeSpriteId, InPosition, InSize.Width, InSize.Height, InRenderState, InTransparency);
	}

	SpriteHandle ISprPerTestFixture_SingleCanvas::CreateSprite
	(
		const MySprMath::SVec2& InPosition,
		const MySprMath::SSize& InSize,
		MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
		MySpr::ESpriteTransparency InTransparency
	)
	{
		return CreateSprite_ZOrderAfter
		(
			nullptr,
			InPosition,
			InSize,
			InRenderState,
			InTransparency
		);
	}

	void ISprPerTestFixture_SingleCanvas::CreateSprite(TSSprite& InSprite)
	{
		InSprite.SetHandle(CreateSprite_ZOrderAfter(InSprite.GetInitZBeforeSpriteHandle(), InSprite.GetInitPos(), InSprite.GetInitSize(), InSprite.GetInitMatInst()));
	}	

	void ISprPerTestFixture_SingleCanvas::DeleteSprite(TSSprite& InSprite)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		DeleteSprite(InSprite.GetHandle());
		InSprite.SetHandle(Test::SpriteHandle{});
	}

	void ISprPerTestFixture_SingleCanvas::HideSprite(const TSSprite& InSprite)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		HideSprite(InSprite.GetHandle());
	}

	void ISprPerTestFixture_SingleCanvas::ShowSprite(const TSSprite& InSprite)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		ShowSprite(InSprite.GetHandle());
	}

	void ISprPerTestFixture_SingleCanvas::SetSpriteTransparency(const TSSprite& InSprite, MySpr::ESpriteTransparency InTransparency)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		SetSpriteTransparency(InSprite.GetHandle(), InTransparency);
	}

	void ISprPerTestFixture_SingleCanvas::SetSpriteGeometry(const TSSprite& InSprite, const MySpr::SSpriteGeometryProps& InGeometry)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		SetSpriteGeometry(InSprite.GetHandle(), InGeometry);
	}

	void ISprPerTestFixture_SingleCanvas::SetSpritePosition(const TSSprite& InSprite, const MySprMath::SVec2& InPosition)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		SetSpritePosition(InSprite.GetHandle(), InPosition);
	}

	void ISprPerTestFixture_SingleCanvas::ResizeSprite(const TSSprite& InSprite, const MySprMath::SSize& InSize)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		ResizeSprite(InSprite.GetHandle(), InSize);
	}

	void ISprPerTestFixture_SingleCanvas::ResizeSprite(const TSSprite& InSprite, float InWidth, float InHeight)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		ResizeSprite(InSprite.GetHandle(), InWidth, InHeight);
	}

	void ISprPerTestFixture_SingleCanvas::SetSpriteMaterial(const TSSprite& InSprite, const MySprRen::MaterialInstanceRenderStateInitializerPtr& InRenderState)
	{
		BOOST_ASSERT(InSprite.IsCreated());
		SetSpriteMaterial(InSprite.GetHandle(), InRenderState);
	}

	void ISprPerTestFixture_SingleCanvas::DeleteSprite(SpriteHandle InHandle)
	{
		T_LOG("Fixture: SingleCanvas: DeleteSprite, Id=" << InHandle->GetId() << "...");
		GetSpriteRenderSubsystemManager()->DeleteSprite(GetUpdater(), InHandle);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InHandle->GetId() << " Deleted");
	}

	void ISprPerTestFixture_SingleCanvas::HideSprite(SpriteHandle InHandle)
	{
		T_LOG("Fixture: SingleCanvas: HideSprite, Id=" << InHandle->GetId() << "...");
		InHandle->Hide(GetUpdater());
		T_LOG("Fixture: SingleCanvas: Sprite << " << InHandle->GetId() << " Hidden");
	}

	void ISprPerTestFixture_SingleCanvas::ShowSprite(SpriteHandle InHandle)
	{
		T_LOG("Fixture: SingleCanvas: ShowSprite, Id=" << InHandle->GetId() << "...");
		InHandle->Show(GetUpdater());
		T_LOG("Fixture: SingleCanvas: Sprite << " << InHandle->GetId() << " Shown");
	}

	void ISprPerTestFixture_SingleCanvas::SetSpriteMaterial(SpriteHandle InHandle, const MySprRen::MaterialInstanceRenderStateInitializerPtr& InRenderState)
	{
		T_LOG("Fixture: SingleCanvas: SetSpriteMaterial, Id=" << InHandle->GetId() << "...");
		InHandle->SetMatInst(GetUpdater(), InRenderState);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InHandle->GetId() << " Material changed");
	}

	void ISprPerTestFixture_SingleCanvas::SetSpriteTransparency(SpriteHandle InHandle, MySpr::ESpriteTransparency InTransparency)
	{
		T_LOG("Fixture: SingleCanvas: SetSpriteTransparency, Id=" << InHandle->GetId() << "...");
		InHandle->SetTransparency(GetUpdater(), InTransparency);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InHandle->GetId() << " Transparency changed");
	}

	void ISprPerTestFixture_SingleCanvas::SetSpriteGeometry(SpriteHandle InHandle, const MySpr::SSpriteGeometryProps& InGeometry)
	{
		T_LOG("Fixture: SingleCanvas: SetSpriteGeometry, Id=" << InHandle->GetId() << "...");
		InHandle->SetGeometry(GetUpdater(), InGeometry);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InHandle->GetId() << " Geometry changed");
	}

	void ISprPerTestFixture_SingleCanvas::SetSpritePosition(SpriteHandle InHandle, const MySprMath::SVec2& InPosition)
	{
		T_LOG("Fixture: SingleCanvas: SetSpritePosition, Id=" << InHandle->GetId() << "...");
		T_LOG("New position: (X;Y)" << InPosition.X << ";" << InPosition.Y);
		InHandle->SetPosition(GetUpdater(), InPosition);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InHandle->GetId() << " Position changed");
	}

	void ISprPerTestFixture_SingleCanvas::ResizeSprite(SpriteHandle InHandle, const MySprMath::SSize& InSize)
	{
		return ResizeSprite(InHandle, InSize.Width, InSize.Height);
	}

	void ISprPerTestFixture_SingleCanvas::ResizeSprite(SpriteHandle InHandle, float InWidth, float InHeight)
	{
		T_LOG("Fixture: SingleCanvas: ResizeSprite, Id=" << InHandle->GetId() << "...");
		T_LOG("New size (width*height)=" << InWidth << ";" << InHeight);
		InHandle->Resize(GetUpdater(), InWidth, InHeight);
		T_LOG("Fixture: SingleCanvas: Sprite << " << InHandle->GetId() << " resized");
	}

	SprVec2 ISprPerTestFixture_SingleCanvas::PointAt(float AlphaX, float AlphaY) const
	{
		return GetSpriteRenderSubsystemManager()->GetCanvasCoordSystem(CANV_ID).PointAt(AlphaX, AlphaY);
	}

	SprVec2 ISprPerTestFixture_SingleCanvas::PointAt(const SprVec2& AlphaVec) const
	{
		return PointAt(AlphaVec.X, AlphaVec.Y);
	}
	
	SprSize ISprPerTestFixture_SingleCanvas::ScreenPart(float InWidthFactor, float InHeightFactor) const
	{
		return GetSpriteRenderSubsystemManager()->GetCanvasCoordSystem(CANV_ID).GetSizePart(InWidthFactor, InHeightFactor);
	}

	TexelColor ISprPerTestFixture_SingleCanvas::ScreenColorAt(const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint)
	{
		return ISprPerTestFixtureBase::CanvasColorAt(CanvHandle, ContextHandle, InCanvasPoint);
	}

	bool ISprPerTestFixture_SingleCanvas::ScreenClearAt(const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint)
	{
		return ISprPerTestFixtureBase::CanvasClearAt(CanvHandle, ContextHandle, InCanvasPoint);
	}

	bool ISprPerTestFixture_SingleCanvas::ColorMatchesScreenAt(const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, const TexelColor& InColor)
	{
		return ISprPerTestFixtureBase::ColorMatchesCanvasAt(CanvHandle, ContextHandle, InCanvasPoint, InColor);
	}

	bool ISprPerTestFixture_SingleCanvas::TextureMatchesScreenAt(const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, const IMPL::TextureElement& InTextureElement)
	{
		return ISprPerTestFixtureBase::TextureMatchesCanvasAt(CanvHandle, ContextHandle, InCanvasPoint, InTextureElement);
	}

	bool ISprPerTestFixture_SingleCanvas::SpriteVisibleAsColor(const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, const TexelColor& InColor)
	{		
		return ISprPerTestFixtureBase::SpriteVisibleAsColor(CanvHandle, ContextHandle, InSprite, InColor);
	}

	bool ISprPerTestFixture_SingleCanvas::SpriteHidden(const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite)
	{
		return ISprPerTestFixtureBase::SpriteHidden(CanvHandle, ContextHandle, InSprite);
	}
	
	bool ISprPerTestFixture_SingleCanvas::SpriteHidden(const IFrameCheckContextHandle& ContextHandle, const TSSprite& InSprite)
	{
		return ISprPerTestFixture_SingleCanvas::SpriteHidden(ContextHandle, InSprite.GetHandle());
	}

	bool ISprPerTestFixture_SingleCanvas::SpriteHiddenAt(const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, const SprVec2& InCanvasPoint)
	{
		return ISprPerTestFixtureBase::SpriteHiddenAt(CanvHandle, ContextHandle, InSprite, InCanvasPoint);
	}

	bool ISprPerTestFixture_SingleCanvas::SpriteHiddenAt(const IFrameCheckContextHandle& ContextHandle, const TSSprite& InSprite, const SprVec2& InCanvasPoint)
	{
		return SpriteHiddenAt(ContextHandle, InSprite.GetHandle(), InCanvasPoint);
	}

	TSSpriteVector ISprPerTestFixture_SingleCanvas::LayoutSprites(const TSMaterialVector& InMaterials)
	{
		size_t const NUM_SPRITES = InMaterials.size();

		float const X_COEFF = 2.0F;
		float const DENOM_SIZE_X = (X_COEFF * NUM_SPRITES + 1);
		float const DENOM_SIZE_Y = DENOM_SIZE_X;
		float const INV_DENOM_SIZE_X = 1.0F/ DENOM_SIZE_X;
		float const INV_DENOM_SIZE_Y = 1.0F/ DENOM_SIZE_Y;
		MySprMath::SSize const SPRITE_SIZE = ScreenPart(INV_DENOM_SIZE_X, INV_DENOM_SIZE_Y);

		TSSpriteVector Sprites;
		for (int i = 0; i < static_cast<int>(NUM_SPRITES); i++)
		{
			const TSMaterial* pMat = &InMaterials[i];			
			MySprMath::SVec2 Pos = PointAt ((1 + i * X_COEFF) * INV_DENOM_SIZE_X, HALF);
			Sprites.emplace_back(*pMat, SPRITE_SIZE, Pos);
		}
	
		return Sprites;
	}

	TSSpriteVector ISprPerTestFixture_SingleCanvas::PrepareSprites(const TSMaterialVector& InMaterials, bool bShouldShow)
	{
		TSSpriteVector Sprites = LayoutSprites(InMaterials);
		CreateSprites(Sprites);
		if (bShouldShow)
		{
			ShowSprites(Sprites);
		}
		return Sprites;
	}
	
	void ISprPerTestFixture_SingleCanvas::CreateSprites(TSSpriteVector& InSprites)
	{
		for(int i = 0; i < static_cast<int>(InSprites.size()); i++)
		{
			CreateSprite(InSprites[i]);
		}
	}

	void ISprPerTestFixture_SingleCanvas::ShowSprites(const TSSpriteVector& InSprites)
	{
		for (const TSSprite& S : InSprites)
		{
			ShowSprite(S);
		}
	}

	boost::test_tools::predicate_result ISprPerTestFixture_SingleCanvas::CheckVisibility(const IFrameCheckContextHandle& ContextHandle, const TSSpriteVector& InSprites)
	{
		for (int SpriteIndex = 0; SpriteIndex < static_cast<int>(InSprites.size()); SpriteIndex++)
		{
			const TSSprite* pS = &(InSprites[SpriteIndex]);

			if (pS->GetHandle()->IsVisible())
			{
				if (false == SpriteVisibleAsColor(ContextHandle, pS->GetHandle(), pS->GetInitUniColor()))
				{
					boost::test_tools::predicate_result res{ false };
					res.message() << "Sprite " << SpriteIndex << "must be visible";
					return res;
				}
			}
			else
			{
				// At this point the sprite must be hidden
				if (false == SpriteHidden(ContextHandle, pS->GetHandle()))
				{
					boost::test_tools::predicate_result res{ false };
					res.message() << "Sprite " << SpriteIndex << "must be hidden";
					return res;
				}
			}
		}
		return true;
	}
} // Test::ISpr