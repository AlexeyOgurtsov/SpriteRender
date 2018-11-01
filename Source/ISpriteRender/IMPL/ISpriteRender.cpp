#include "../ISpriteRender.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
	SSpriteCanvasCreateCommandInitializer GetCanvasInit(bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName,  const SSpriteCanvasProps& InCanvasProps, int InInitialCapacityInSprites, bool bInAutoResize)
	{
		SSpriteCanvasCreateCommandInitializer Initializer { InCanvasId, InName, InCanvasProps };
		Initializer.bDebug = bInDebug;
		Initializer.bAutoResize = bInAutoResize;
		return Initializer;
	}

	void ISpriteRender::CreateCanvas(bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName, const SSpriteCanvasProps& InCanvasProps, int InInitialCapacityInSprites, bool bInAutoResize)
	{	
		return CreateCanvas(GetCanvasInit(bInDebug, InCanvasId, InName, InCanvasProps, InInitialCapacityInSprites, bInAutoResize));
	}

	SSpriteCanvasCreateCommandInitializer GetCanvasInit(bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName, const SCanvasPickProps& InPickProps, const SCanvasRect& InRect, const SCanvasCoordSystem& InCoordSystem, int InInitialCapacityInSprites, bool bInAutoResize)
	{
		SSpriteCanvasProps Props { InRect, InCoordSystem, InPickProps };
		return GetCanvasInit(bInDebug, InCanvasId, InName, Props, InInitialCapacityInSprites, bInAutoResize);
	}

	void ISpriteRender::CreateCanvas(bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName, const SCanvasPickProps& InPickProps, const SCanvasRect& InRect, const SCanvasCoordSystem& InCoordSystem, int InInitialCapacityInSprites, bool bInAutoResize)
	{
		return CreateCanvas(GetCanvasInit(bInDebug, InCanvasId, InName, InPickProps, InRect, InCoordSystem, InInitialCapacityInSprites, bInAutoResize));
	}

	SSpriteCanvasCreateCommandInitializer GetCanvasInit(bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName, const SCanvasPickProps& InPickProps, int InWidth, int InHeight, int InLeft, int InTop, const SCanvasCoordSystem& InCoordSystem, int InInitialCapacityInSprites, bool bInAutoResize)
	{
		SCanvasRect Rect { InWidth, InHeight, InLeft, InTop };
		return GetCanvasInit(bInDebug, InCanvasId, InName, InPickProps, Rect, InCoordSystem, InInitialCapacityInSprites, bInAutoResize);
	}

	void ISpriteRender::CreateCanvas(bool bInDebug, SpriteCanvasId InCanvasId, const std::string& InName, const SCanvasPickProps& InPickProps, int InWidth, int InHeight, int InLeft, int InTop, const SCanvasCoordSystem& InCoordSystem, int InInitialCapacityInSprites, bool bInAutoResize)
	{		
		return CreateCanvas(GetCanvasInit(bInDebug, InCanvasId, InName, InPickProps, InWidth, InHeight, InLeft, InTop, InCoordSystem, InInitialCapacityInSprites, bInAutoResize));
	}

	int ISpriteRender::GetNumHiddenCanvasses() const
	{
		return GetNumCanvasses() - GetNumVisibleCanvasses();
	}

	int ISpriteRender::GetNumHiddenSprites_ForCanvas(SpriteCanvasId InCanvasId) const
	{
		return GetNumSprites_ForCanvas(InCanvasId) - GetNumVisibleSprites_ForCanvas(InCanvasId);
	}
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv