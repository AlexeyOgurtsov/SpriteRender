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

const std::string& Canvas::GetName() const
{
	return Initializer.Name;
}

bool Canvas::IsDebug() const
{
	return Initializer.bDebug;
}


const MySpr::SCanvasPickProps& Canvas::GetPickProps() const
{
	return Initializer.Props.Pick;
}

void Canvas::UpdatePickProps(const MySpr::SCanvasPickProps& InPickProps)
{
	Initializer.Props.Pick = InPickProps;
}

bool Canvas::CanBePicked() const
{
	return GetPickProps().CanBePicked();
}

int Canvas::GetCapacityInSprites() const
{
	return Initializer.InitialCapacityInSprites;
}

bool Canvas::IsAutoResize() const
{
	return Initializer.bAutoResize;
}

bool Canvas::IsVisible() const
{
	return Initializer.bShow;
}

bool Canvas::IsHidden() const
{
	return false == IsVisible();
}

void Canvas::Show()
{
	Initializer.bShow = true;
	pSubsys->ShowCanvas(GetId());
}

void Canvas::Hide()
{
	Initializer.bShow = false;
	pSubsys->HideCanvas(GetId());
}

CanvasId Canvas::GetId() const 
{
	return Initializer.TargetSpriteCanvasId; 
}

const MySpr::SSpriteCanvasProps& Canvas::GetProps() const 
{ 
	return Initializer.Props; 
}

const MySpr::SCanvasRect& Canvas::GetRTRect() const
{
	return GetProps().RTRect;
}

void Canvas::SetRTRect(const MySpr::SCanvasRect& InRect)
{
	Initializer.Props.RTRect = InRect;
}

ScreenCoordSystemDesc Canvas::GetCoordSystem() const
{
	float const AspectWidthOverHeight = GetAspectWidthOverHeight();
	return ScreenCoordSystemDesc
	{
		GetProps().CoordSystem.Center,
		GetProps().CoordSystem.GetHalfWidth(AspectWidthOverHeight),
		GetProps().CoordSystem.HalfHeight		
	};
}

void Canvas::SetCoordSystem(const MySpr::SCanvasCoordSystem& InCoordSystem)
{
	Initializer.Props.CoordSystem = InCoordSystem;
}

float Canvas::GetAspectWidthOverHeight() const
{
	return GetRTRect().GetAspectWidthOverHeight();
}

int Canvas::GetRTWidth() const
{
	return GetRTRect().Width;
}

int Canvas::GetRTHeight() const
{
	return GetRTRect().Height;
}

int Canvas::GetRTLeft() const
{
	return GetRTRect().Left;
}

int Canvas::GetRTTop() const
{
	return GetRTRect().Top;
}

int Canvas::GetRTRight() const
{
	return GetRTRect().GetRight();
}

int Canvas::GetRTBottom() const
{
	return GetRTRect().GetBottom();
}

} // Test