#include "SprRenHelper_Canvas.h"
#include "../Utils/TestUtils.h"
#include "../ISpriteRenderSubsystemManager.h"

namespace Test
{

Canvas::Canvas(ISpriteRenderSubsystemManager* pInSubsys, const MySprRen::SSpriteCanvasCreateCommandInitializer& InInitializer) :
	pSubsys{pInSubsys}
,	Initializer{InInitializer} 
{
	BOOST_ASSERT(pInSubsys);
}

CanvasId Canvas::GetId() const 
{
	return Initializer.TargetSpriteCanvasId; 
}

const MySpr::SSpriteCanvasProps& Canvas::GetProps() const 
{ 
	return Initializer.Props; 
}

const MySpr::SCanvasRect& Canvas::GetRect() const
{
	return GetProps().RTRect;
}

ScreenCoordSystemDesc Canvas::GetCoordSystem() const
{
	return pSubsys->GetCanvasCoordSystem(GetId());
}

int Canvas::GetWidth() const
{
	return GetRect().Width;
}

int Canvas::GetHeight() const
{
	return GetRect().Height;
}

int Canvas::GetLeft() const
{
	return GetRect().Left;
}

int Canvas::GetTop() const
{
	return GetRect().Top;
}

int Canvas::GetRight() const
{
	return GetRect().GetRight();
}

int Canvas::GetBottom() const
{
	return GetRect().GetBottom();
}

} // Test