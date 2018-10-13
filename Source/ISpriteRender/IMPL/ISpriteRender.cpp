#include "../ISpriteRender.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
	SSpriteCanvasCreateCommandInitializer GetCanvasInit(SpriteCanvasId InCanvasId, const SSpriteCanvasProps& InCanvasProps)
	{
		return SSpriteCanvasCreateCommandInitializer { InCanvasId, InCanvasProps };
	}

	void ISpriteRender::CreateCanvas(SpriteCanvasId InCanvasId, const SSpriteCanvasProps& InCanvasProps)
	{	
		return CreateCanvas(GetCanvasInit(InCanvasId, InCanvasProps));
	}

	SSpriteCanvasCreateCommandInitializer GetCanvasInit(SpriteCanvasId InCanvasId, const SCanvasRect& InRect)
	{
		SSpriteCanvasProps Props { InRect };
		return GetCanvasInit(InCanvasId, Props);
	}

	void ISpriteRender::CreateCanvas(SpriteCanvasId InCanvasId, const SCanvasRect& InRect)
	{
		return CreateCanvas(GetCanvasInit(InCanvasId, InRect));
	}

	SSpriteCanvasCreateCommandInitializer GetCanvasInit(SpriteCanvasId InCanvasId, int InWidth, int InHeight, int InLeft, int InTop)
	{
		SCanvasRect Rect { InWidth, InHeight, InLeft, InTop };
		return GetCanvasInit(InCanvasId, Rect);
	}

	void ISpriteRender::CreateCanvas(SpriteCanvasId InCanvasId, int InWidth, int InHeight, int InLeft, int InTop)
	{		
		return CreateCanvas(GetCanvasInit(InCanvasId, InWidth, InHeight, InLeft, InTop));
	}
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv