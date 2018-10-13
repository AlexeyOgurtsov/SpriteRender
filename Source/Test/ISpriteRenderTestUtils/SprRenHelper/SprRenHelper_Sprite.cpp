#include "SprRenHelper_Sprite.h"
#include "../Utils/TestUtils.h"
#include "../ISpriteRenderSubsystemManager.h"

namespace Test
{

Sprite::Sprite(ISpriteRenderSubsystemManager* pInSubsys, CanvasHandle pInCanvas,  const MySprRen::SSpriteCreateCommandInitializer& InInitializer) :
	pSubsys{pInSubsys}
,	pCanvas{pInCanvas}
,	Initializer{InInitializer} 
,	CurrInitializer{InInitializer}
{
	BOOST_ASSERT(pInSubsys);
	BOOST_ASSERT(pInCanvas);
}

SprId Sprite::GetId() const 
{
	return Initializer.TargetSpriteId; 
}

const Handle_SprMaterialInstance& Sprite::GetInitMatInst()
{
	return Initializer.pRenderState; 
}

const MySpr::SSpriteProps& Sprite::GetInitProps() const 
{ 
	return Initializer.Props; 
}

void Sprite::Resize(MySprRen::ISpriteUpdater* pInUpdater, float InNewWidth, float InNewHeight)
{
	CurrInitializer.Props.Geometry.Size.Width = InNewWidth;
	CurrInitializer.Props.Geometry.Size.Height = InNewHeight;
	pSubsys->SetSpriteGeometry(pInUpdater, GetId(), CurrInitializer.Props.Geometry);
}

void Sprite::SetPosition(MySprRen::ISpriteUpdater* pInUpdater, const SprVec2& InPosition)
{
	CurrInitializer.Props.Geometry.Transform.Position = InPosition;
	pSubsys->SetSpriteGeometry(pInUpdater, GetId(), CurrInitializer.Props.Geometry);
}

void Sprite::Show(MySprRen::ISpriteUpdater* pInUpdater)
{
	CurrInitializer.bShow = true;
	pSubsys->ShowSprite(pInUpdater, GetId());
}

void Sprite::Hide(MySprRen::ISpriteUpdater* pInUpdater)
{
	CurrInitializer.bShow = false;
	pSubsys->HideSprite(pInUpdater, GetId());
}

void Sprite::SetTransparency(MySprRen::ISpriteUpdater* pInUpdater, MySpr::ESpriteTransparency InTransparency)
{
	CurrInitializer.Props.RenderMode.Transparency = InTransparency;
	pSubsys->SetSpriteTransparency(pInUpdater, GetId(), InTransparency);
}

} // Test