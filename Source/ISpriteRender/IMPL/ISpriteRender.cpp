#include "../ISpriteRender.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
	void ISpriteRender::CreateCanvas(SpriteCanvasId InCanvasId, const SSpriteCanvasProps& InCanvasProps)
	{	
		SSpriteCanvasCreateCommandInitializer Initializer { InCanvasId, InCanvasProps };
		return CreateCanvas(Initializer);
	}

	void ISpriteRender::CreateCanvas(SpriteCanvasId InCanvasId, const SCanvasRect& InRect)
	{
		SSpriteCanvasProps Props { InRect };
		return CreateCanvas(InCanvasId, Props);
	}

	void ISpriteRender::CreateCanvas(SpriteCanvasId InCanvasId, int InWidth, int InHeight, int InLeft, int InTop)
	{
		SCanvasRect Rect { InWidth, InHeight, InLeft, InTop };
		return CreateCanvas(InCanvasId, Rect);
	}
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv