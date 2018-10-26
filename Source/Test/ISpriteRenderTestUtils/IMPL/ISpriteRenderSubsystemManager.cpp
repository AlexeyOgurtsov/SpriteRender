#include "../ISpriteRenderSubsystemManager.h"	
#include "ISpriteRender/ISpriteRender.h"
#include "../Utils/TestUtils.h"

namespace Test
{
	CanvasHandle ISpriteRenderSubsystemManager::CreateCanvas(CanvasId InId, unsigned int InWidth, unsigned int InHeight)
	{
		T_LOG("ISpriteRenderSubsystemManager::CreateCanvas, Id = " << InId);
		T_LOG("Width*Height=" << InWidth << "*" << InHeight);

		MySprRen::SSpriteCanvasCreateCommandInitializer Initializer = MySprRen::GetCanvasInit(InId, InWidth, InHeight);

		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::CreateCanvas Sprite render must be initialized");
		GetSpriteRender()->CreateCanvas(Initializer);

		T_LOG("ISpriteRenderSubsystemManager::CreateCanvas DONE");

		return std::make_unique<Canvas>(this, Initializer);
	}

	void ISpriteRenderSubsystemManager::DeleteCanvas(CanvasHandle InHandle)
	{
		SprId const Id = InHandle->GetId();
		T_LOG("ISpriteRenderSubsystemManager::DeleteCanvas, Id=" << Id << "...");
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::DeleteCanvas Sprite render must be initialized");
		GetSpriteRender()->DeleteCanvas(Id);		
		T_LOG("ISpriteRenderSubsystemManager::DeleteCanvas DONE");
	}

	void ISpriteRenderSubsystemManager::ShowCanvas(CanvasHandle InHandle)
	{
		SprId const Id = InHandle->GetId();
		T_LOG("ISpriteRenderSubsystemManager::ShowCanvas, Id=" << Id << "...");
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::ShowCanvas: Sprite render must be initialized");
		GetSpriteRender()->ShowCanvas(Id);
		T_LOG("ISpriteRenderSubsystemManager::ShowCanvas DONE");
	}

	void ISpriteRenderSubsystemManager::HideCanvas(CanvasHandle InHandle)
	{
		SprId Id = InHandle->GetId();
		T_LOG("ISpriteRenderSubsystemManager::HideCanvas, Id=" << Id << "...");
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::HideCanvas: Sprite render must be initialized");
		GetSpriteRender()->HideCanvas(Id);
		T_LOG("ISpriteRenderSubsystemManager::HideCanvas DONE");
	}

	SpriteHandle ISpriteRenderSubsystemManager::CreateSprite_ZOrderAfter
	(
		SprId InId,
		SprId InZBeforeSpriteId,
		MySprRen::ISpriteUpdater* pInUpdater,
		CanvasHandle pInCanvas,
		const MySprMath::SVec2& InPosition,
		float InWidth, float InHeight,
		MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
		MySpr::ESpriteTransparency InTransparency
	)
	{
		T_LOG("ISpriteRenderSubsystemManager::CreateSprite_ZOrderAfter, Id=" << InId << "...");
		BOOST_ASSERT(pInUpdater);
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::CreateSprite: Sprite render must be initialized");
		T_LOG("ZBeforeSprite Id: " << InZBeforeSpriteId << (MySpr::IsValidSpriteId(InZBeforeSpriteId) ? "(valid)" : "(invalid=<OnTop>)"));
		T_LOG("Position: " << ToString(InPosition));
		T_LOG("(Width*Height)=" << InWidth << "*" << InHeight);
		T_LOG("Transparency=" << ToString(InTransparency));
		MySprRen::SSpriteCreateCommandInitializer const Initializer = MySprRen::GetSprInit(InId, InPosition, InWidth, InHeight, InRenderState, InTransparency);
		pInUpdater->CreateSprite_ZOrderAfter(InId, InZBeforeSpriteId, InPosition, InWidth, InHeight, InRenderState, InTransparency);
		T_LOG("ISpriteRenderSubsystemManager: Sprite " << InId << " Created");
		return std::make_unique<Sprite>(this, pInCanvas, Initializer);
	}

	SpriteHandle ISpriteRenderSubsystemManager::CreateSprite
	(
		SprId InId,
		MySprRen::ISpriteUpdater* pInUpdater,
		CanvasHandle pInCanvas,
		const MySprMath::SVec2& InPosition,
		float InWidth, float InHeight,
		MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState,
		MySpr::ESpriteTransparency InTransparency
	)
	{
		return CreateSprite_ZOrderAfter(InId, MySpr::NULL_SPRITE_ID, pInUpdater, pInCanvas, InPosition, InWidth, InHeight, InRenderState, InTransparency);
	}

	void ISpriteRenderSubsystemManager::DeleteSprite(MySprRen::ISpriteUpdater* pInUpdater, SpriteHandle InHandle)
	{
		SprId const Id = InHandle->GetId();
		T_LOG("ISpriteRenderSubsystemManager::DeleteSprite, Id=" << Id << "...");
		BOOST_ASSERT(pInUpdater);
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::DeleteSprite: Sprite render must be initialized");
		pInUpdater->DeleteSprite(Id);		
		T_LOG("ISpriteRenderSubsystemManager::Sprite << " << Id << " Deleted");
	}

	void ISpriteRenderSubsystemManager::ShowSprite(MySprRen::ISpriteUpdater* pInUpdater, SprId InId)
	{		
		T_LOG("ISpriteRenderSubsystemManager::ShowSprite, Id=" << InId << "...");
		BOOST_ASSERT(pInUpdater);
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::ShowSprite: Sprite render must be initialized");		
		pInUpdater->ShowSprite(InId);
		T_LOG("ISpriteRenderSubsystemManager::Sprite << " << InId << " Shown");
	}

	void ISpriteRenderSubsystemManager::HideSprite(MySprRen::ISpriteUpdater* pInUpdater, SprId InId)
	{
		T_LOG("ISpriteRenderSubsystemManager::HideSprite, Id=" << InId << "...");
		BOOST_ASSERT(pInUpdater);
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::HideSprite: Sprite render must be initialized");
		pInUpdater->HideSprite(InId);
		T_LOG("ISpriteRenderSubsystemManager::Sprite << " << InId << " Hidden");
	}

	void ISpriteRenderSubsystemManager::SetMatInst(MySprRen::ISpriteUpdater* pInUpdater, SprId InId, MySprRen::MaterialInstanceRenderStateInitializerPtr InRenderState)
	{
		T_LOG("ISpriteRenderSubsystemManager::SetMatInst, Id=" << InId << "...");
		BOOST_ASSERT(pInUpdater);
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::SetMatInst: Sprite render must be initialized");
		pInUpdater->SetSpriteMaterial(InId, InRenderState);
		T_LOG("ISpriteRenderSubsystemManager::SetMatInst DONE");
	}

	void ISpriteRenderSubsystemManager::MoveSpriteZOrderAfter(MySprRen::ISpriteUpdater* pInUpdater, SprId InId, SprId InZBeforeSpriteId)
	{
		T_LOG("ISpriteRenderSubsystemManager::MoveSpriteZOrderAfter, Id=" << InId << "...");
		BOOST_ASSERT(pInUpdater);
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::CreateSprite: Sprite render must be initialized");
		T_LOG("ZBeforeSprite Id: " << InZBeforeSpriteId << (MySpr::IsValidSpriteId(InZBeforeSpriteId) ? "(valid)" : "(invalid=<OnTop>)"));		
		pInUpdater->SetSpriteZOrder(InId, InZBeforeSpriteId);
		T_LOG("ISpriteRenderSubsystemManager: Sprite " << InId << " MoveSpriteZOrderAfter called");
	}

	void ISpriteRenderSubsystemManager::SetSpriteTransparency(MySprRen::ISpriteUpdater* pInUpdater, SprId InId, MySpr::ESpriteTransparency InTransparency)
	{
		T_LOG("ISpriteRenderSubsystemManager::SetTransparency, Id=" << InId << "...");
		T_LOG("New transparency: " << ToString(InTransparency));
		BOOST_ASSERT(pInUpdater);
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::SetSpriteTransparency: Sprite render must be initialized");
		pInUpdater->SetSpriteTransparency(InId, InTransparency);
		T_LOG("ISpriteRenderSubsystemManager::Sprite << " << InId << " Transparency changed");
	}

	void ISpriteRenderSubsystemManager::SetSpriteGeometry(MySprRen::ISpriteUpdater* pInUpdater, SprId InId, const MySpr::SSpriteGeometryProps& InGeometry)
	{
		T_LOG("ISpriteRenderSubsystemManager::SetSpriteGeometry, Id=" << InId << "...");
		BOOST_ASSERT(pInUpdater);
		BOOST_ASSERT_MSG(GetSpriteRender(), "ISpriteRenderSubsystemManager::SetSpriteGeometry: Sprite render must be initialized");
		pInUpdater->SetSpriteGeometry(InId, InGeometry);
		T_LOG("ISpriteRenderSubsystemManager::Sprite << " << InId << " geometry changed");
	}
} // Test