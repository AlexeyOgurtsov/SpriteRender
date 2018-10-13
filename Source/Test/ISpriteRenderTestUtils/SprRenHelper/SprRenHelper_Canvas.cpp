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

} // Test