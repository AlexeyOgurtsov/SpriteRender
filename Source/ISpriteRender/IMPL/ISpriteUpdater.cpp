#include "../ISpriteUpdater.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
	void ISpriteUpdater::CreateSprite(SpriteId InSpriteId, const SSpriteProps& InProps, MaterialInstanceRenderStateInitializerPtr InRenderState)
	{
		SSpriteCreateCommandInitializer Initializer { InSpriteId, InProps, InRenderState };
		return CreateSprite(Initializer);
	}

	void ISpriteUpdater::CreateSprite(SpriteId InSpriteId, const SSpriteGeometryProps& InGeometry, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		SSpriteProps Props { InGeometry, SSpriteRenderMode { InTransparency } };
		return CreateSprite(InSpriteId, Props, InRenderState);
	}

	void ISpriteUpdater::CreateSprite(SpriteId InSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		SSpriteGeometryProps Geometry { InTransform, InSize };
		return CreateSprite(InSpriteId, Geometry, InRenderState, InTransparency);
	}

	void ISpriteUpdater::CreateSprite(SpriteId InSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		return CreateSprite(InSpriteId, Math::SSpriteTransform{InPosition}, Math::SSize{InWidth, InHeight}, InRenderState, InTransparency);
	}

	void ISpriteUpdater::DeleteSprite(SpriteId InSpriteId)	
	{
		SSpriteDeleteCommandInitializer Initializer { InSpriteId };
		return DeleteSprite(Initializer);	
	}

	void ISpriteUpdater::HideSprite(SpriteId InSpriteId)
	{
		SSpriteHideCommandInitializer Initializer { InSpriteId };
		return HideSprite(Initializer);
	}

	void ISpriteUpdater::ShowSprite(SpriteId InSpriteId)
	{
		SSpriteShowCommandInitializer Initializer { InSpriteId };
		return ShowSprite(Initializer);
	}

	void ISpriteUpdater::SetSpriteTransparency(SpriteId InSpriteId, ESpriteTransparency InTransparency)
	{
		SSpriteSetTransparencyCommandInitializer Initializer { InSpriteId, InTransparency };
		return SetSpriteTransparency(Initializer);
	}

	void ISpriteUpdater::SetSpriteGeometry(SpriteId InSpriteId, const SSpriteGeometryProps& InGeometry)
	{
		SSpriteSetGeometryCommandInitializer Initializer { InSpriteId, InGeometry };
		return SetSpriteGeometry(Initializer);
	}

	void ISpriteUpdater::SetSpriteMaterial(SpriteId InSpriteId, MaterialInstanceRenderStateInitializerPtr pInRenderState)
	{
		SSpriteSetMaterialCommandInitializer Initializer { InSpriteId, pInRenderState };
		return SetSpriteMaterial(Initializer);
	}

	void ISpriteUpdater::SetSpriteZOrder(SpriteId InSpriteId, SpriteId ZBeforeSpriteId)
	{
		SSpriteSetZOrderCommandInitializer Initializer { InSpriteId, ZBeforeSpriteId };
		return SetSpriteZOrder(Initializer);
	}

	void ISpriteUpdater::MoveSpriteOnTop(SpriteId InSpriteId)
	{
		return SetSpriteZOrder(InSpriteId, NULL_SPRITE_ID);
	}
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv