#include "SprRenHelper_Sprite.h"
#include "../Utils/TestUtils.h"
#include "../Utils/MathUtils.h"
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

void Sprite::SetGeometry(MySprRen::ISpriteUpdater* pInUpdater, const MySpr::SSpriteGeometryProps& InGeometry)
{
	CurrInitializer.Props.Geometry = InGeometry;
	pSubsys->SetSpriteGeometry(pInUpdater, GetId(), CurrInitializer.Props.Geometry);
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

void Sprite::SetMatInst(MySprRen::ISpriteUpdater* pInUpdater, const Handle_SprMaterialInstance& InInst)
{
	CurrInitializer.pRenderState = InInst;
	pSubsys->SetMatInst(pInUpdater, GetId(), InInst);
}

void Sprite::Show(MySprRen::ISpriteUpdater* pInUpdater)
{
	CurrInitializer.bShow = true;
	pSubsys->ShowSprite(pInUpdater, GetId());
}

bool Sprite::IsVisible() const
{
	return CurrInitializer.bShow;
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


const Handle_SprMaterialInstance& Sprite::GetMatInst()
{
	return CurrInitializer.pRenderState;
}

const MySpr::SSpriteProps& Sprite::GetProps() const
{
	return CurrInitializer.Props;
}

const MySpr::SSpriteGeometryProps& Sprite::GetGeometry() const
{
	return CurrInitializer.Props.Geometry;
}

const MySprMath::SSpriteTransform& Sprite::GetTransform() const
{
	return GetGeometry().Transform;
}

const MySprMath::SVec2& Sprite::GetPosition() const
{
	return GetTransform().Position;
}

const MySprMath::SSize& Sprite::GetSize() const
{
	return GetGeometry().Size;
}

const MySprMath::SVec2 Sprite::GetCenter() const
{
	return MySprMath::GetCenter(GetPosition(), GetSize());
}

const MySpr::ESpriteTransparency& Sprite::GetTransparency() const
{
	return GetProps().RenderMode.Transparency;
}

} // Test