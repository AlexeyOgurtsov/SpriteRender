#include "../ISpriteUpdater.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
	SSpriteCreateCommandInitializer GetSprInit_ZOrderAfter(SpriteId InSpriteId, SpriteId InZBeforeSpriteId, const SSpriteProps& InProps, MaterialInstanceRenderStateInitializerPtr InRenderState)
	{
		SSpriteCreateCommandInitializer Initializer = GetSprInit(InSpriteId, InProps, InRenderState);
		Initializer.ZBeforeSpriteId = InZBeforeSpriteId;
		return Initializer;
	}

	SSpriteCreateCommandInitializer GetSprInit_ZOrderAfter(SpriteId InSpriteId, SpriteId InZBeforeSpriteId, const SSpriteGeometryProps& InGeometry, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		SSpriteCreateCommandInitializer Initializer = GetSprInit(InSpriteId, InGeometry, InRenderState, InTransparency);
		Initializer.ZBeforeSpriteId = InZBeforeSpriteId;
		return Initializer;
	}

	SSpriteCreateCommandInitializer GetSprInit_ZOrderAfter(SpriteId InSpriteId, SpriteId InZBeforeSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		SSpriteCreateCommandInitializer Initializer = GetSprInit(InSpriteId, InTransform, InSize, InRenderState, InTransparency);
		Initializer.ZBeforeSpriteId = InZBeforeSpriteId;
		return Initializer;
	}

	SSpriteCreateCommandInitializer GetSprInit_ZOrderAfter(SpriteId InSpriteId, SpriteId InZBeforeSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency, const Math::SVec2& InOrigin, float InAngleDegs)
	{
		SSpriteCreateCommandInitializer Initializer = GetSprInit(InSpriteId, InPosition, InWidth, InHeight, InRenderState, InTransparency, InOrigin, InAngleDegs);
		Initializer.ZBeforeSpriteId = InZBeforeSpriteId;
		return Initializer;
	}	

	SSpriteCreateCommandInitializer GetSprInit(SpriteId InSpriteId, const SSpriteProps& InProps, MaterialInstanceRenderStateInitializerPtr InRenderState)
	{
		return SSpriteCreateCommandInitializer { InSpriteId, InProps, InRenderState };		
	}

	void ISpriteUpdater::CreateSprite_ZOrderAfter(SpriteId InSpriteId, SpriteId InZBeforeSpriteId, const SSpriteProps& InProps, MaterialInstanceRenderStateInitializerPtr InRenderState)
	{
		return CreateSprite(GetSprInit_ZOrderAfter(InSpriteId, InZBeforeSpriteId, InProps, InRenderState));
	}

	void ISpriteUpdater::CreateSprite(SpriteId InSpriteId, const SSpriteProps& InProps, MaterialInstanceRenderStateInitializerPtr InRenderState)
	{
		return CreateSprite(GetSprInit(InSpriteId, InProps, InRenderState));
	}


	SSpriteCreateCommandInitializer GetSprInit(SpriteId InSpriteId, const SSpriteGeometryProps& InGeometry, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		SSpriteProps Props { InGeometry, SSpriteRenderMode { InTransparency } };
		return GetSprInit(InSpriteId, Props, InRenderState);
	}

	void ISpriteUpdater::CreateSprite_ZOrderAfter(SpriteId InSpriteId, SpriteId InZBeforeSpriteId, const SSpriteGeometryProps& InGeometry, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		return CreateSprite(GetSprInit_ZOrderAfter(InSpriteId, InZBeforeSpriteId, InGeometry, InRenderState, InTransparency));
	}

	void ISpriteUpdater::CreateSprite(SpriteId InSpriteId, const SSpriteGeometryProps& InGeometry, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{		
		return CreateSprite(GetSprInit(InSpriteId, InGeometry, InRenderState, InTransparency));
	}

	SSpriteCreateCommandInitializer GetSprInit(SpriteId InSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		SSpriteGeometryProps Geometry { InTransform, InSize };
		return GetSprInit(InSpriteId, Geometry, InRenderState, InTransparency);
	}

	void ISpriteUpdater::CreateSprite_ZOrderAfter(SpriteId InSpriteId, SpriteId InZBeforeSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		return CreateSprite(GetSprInit_ZOrderAfter(InSpriteId, InZBeforeSpriteId, InTransform, InSize, InRenderState, InTransparency));
	}

	void ISpriteUpdater::CreateSprite(SpriteId InSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency)
	{
		return CreateSprite(GetSprInit(InSpriteId, InTransform, InSize, InRenderState, InTransparency));
	}

	SSpriteCreateCommandInitializer GetSprInit(SpriteId InSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency, const Math::SVec2& InOrigin, float InAngleDegs)
	{
		return GetSprInit(InSpriteId, Math::SSpriteTransform{InPosition, InOrigin, InAngleDegs }, Math::SSize{InWidth, InHeight}, InRenderState, InTransparency);
	}

	void ISpriteUpdater::CreateSprite_ZOrderAfter(SpriteId InSpriteId, SpriteId InZBeforeSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency, const Math::SVec2& InOrigin, float InAngleDegs)
	{
		return CreateSprite(GetSprInit_ZOrderAfter(InSpriteId, InZBeforeSpriteId, InPosition, InWidth, InHeight, InRenderState, InTransparency, InOrigin, InAngleDegs));
	}

	void ISpriteUpdater::CreateSprite(SpriteId InSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight, MaterialInstanceRenderStateInitializerPtr InRenderState, ESpriteTransparency InTransparency, const Math::SVec2& InOrigin, float InAngleDegs)
	{
		return CreateSprite(GetSprInit(InSpriteId, InPosition, InWidth, InHeight, InRenderState, InTransparency, InOrigin, InAngleDegs));
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

	SSpriteSetGeometryCommandInitializer GetSprGeometryInit(SpriteId InTargetSpriteId, const SSpriteGeometryProps& InGeometry)
	{
		return SSpriteSetGeometryCommandInitializer { InTargetSpriteId, InGeometry };
	}

	void ISpriteUpdater::SetSpriteGeometry(SpriteId InSpriteId, const SSpriteGeometryProps& InGeometry)
	{		
		return SetSpriteGeometry(GetSprGeometryInit(InSpriteId, InGeometry));
	}

	SSpriteSetGeometryCommandInitializer GetSprGeometryInit(SpriteId InTargetSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize)
	{
		return GetSprGeometryInit(InTargetSpriteId, SSpriteGeometryProps{ InTransform, InSize } );
	}

	void ISpriteUpdater::SetSpriteGeometry(SpriteId InTargetSpriteId, const Math::SSpriteTransform& InTransform, const Math::SSize& InSize)
	{
		return SetSpriteGeometry(GetSprGeometryInit(InTargetSpriteId, InTransform, InSize));
	}

	SSpriteSetGeometryCommandInitializer GetSprGeometryInit(SpriteId InTargetSpriteId, const Math::SVec2& InPosition, const Math::SSize& InSize, const Math::SVec2& InOrigin, float InAngleDegs)
	{
		return GetSprGeometryInit(InTargetSpriteId, Math::SSpriteTransform{InPosition, InOrigin, InAngleDegs }, InSize);
	}

	void ISpriteUpdater::SetSpriteGeometry(SpriteId InSpriteId, const Math::SVec2& InPosition, const Math::SSize& InSize)
	{
		return SetSpriteGeometry(GetSprGeometryInit(InSpriteId, InPosition, InSize));
	}

	SSpriteSetGeometryCommandInitializer GetSprGeometryInit(SpriteId InTargetSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight, const Math::SVec2& InOrigin, float InAngleDegs)
	{
		return GetSprGeometryInit(InTargetSpriteId, InPosition, Math::SSize{InWidth, InHeight}, InOrigin, InAngleDegs);
	}

	void ISpriteUpdater::SetSpriteGeometry(SpriteId InSpriteId, const Math::SVec2& InPosition, float InWidth, float InHeight)
	{
		return SetSpriteGeometry(GetSprGeometryInit(InSpriteId, InPosition, InWidth, InHeight));
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