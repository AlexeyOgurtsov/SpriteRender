#include "TestTypes.h"
#include "../IMPL/Resources/TextureLib.h"

namespace Test
{
	TSMaterial::TSMaterial
	(
		const TexelColor& InUniColor,
		const Handle_SprMaterialInstance& InMatInst
	) :
		UniColor{InUniColor}
	,	InitMatInst{InMatInst} 
	{
		BOOST_ASSERT(InitMatInst);
	}

	TSSprite::TSSprite
	(
		const MySpr::SSpritePickProps& InPickProps,
		const TSMaterial& InMaterial,
		const MySprMath::SSize& InInitSize,
		const MySprMath::SVec2& InInitPos,
		const SpriteTransparencyMode& InInitTransparencyMode,
		const MySprMath::SVec2& InInitOrigin,
		float InInitRotationAngle
	) :
		TSSprite{nullptr, InPickProps, InMaterial, InInitSize, InInitPos, InInitTransparencyMode, InInitOrigin, InInitRotationAngle }
	{
	}

	TSSprite::TSSprite
	(
		TSSprite* InZBeforeSpriteId,
		const MySpr::SSpritePickProps& InPickProps,
		const TSMaterial& InMaterial,
		const MySprMath::SSize& InInitSize,
		const MySprMath::SVec2& InInitPos,
		const SpriteTransparencyMode& InInitTransparencyMode,
		const MySprMath::SVec2& InInitOrigin,
		float InInitRotationAngle
	) :
		InZBeforeSpriteId{InZBeforeSpriteId}
	,	InitPickProps{InPickProps}
	,	InitMaterial{InMaterial}
	,	InitSize{InInitSize}
	,	InitPos{InInitPos}
	,	InitOrigin{InInitOrigin}
	,	InitRotationAngle{ InInitRotationAngle }
	,	InitTransparencyMode { InInitTransparencyMode }  {}

	SpriteHandle TSSprite::GetInitZBeforeSpriteHandle() const
	{
		BOOST_ASSERT_MSG(nullptr == InZBeforeSpriteId || InZBeforeSpriteId->GetHandle(), "ZBefore sprite must be created, or set as nullptr");
		return InZBeforeSpriteId ? InZBeforeSpriteId->GetHandle() : nullptr;
	}

	void TSSprite::SetHandle(const SpriteHandle& InHandle)
	{
		BOOST_ASSERT_MSG(nullptr == InHandle || IsNotCreated(), "TSSprite: You cannot assign handle for already-created sprite");
		Handle = InHandle;
	}

	const TSMaterialGenProps TSMaterialGenProps::StrictDefault{ TMAT_GET_DEFAULT_FLAGS | TSMAT_GEN_NO_BRIGHT_COEFF, /*bStrict*/true };
	const TSMaterialGenProps TSMaterialGenProps::StrictWithBrightCoeff{ TMAT_GET_DEFAULT_FLAGS, /*bStrict*/true };
	const TSMaterialGenProps TSMaterialGenProps::StrictOnlyMain{ TMAT_GET_DEFAULT_FLAGS | TSMAT_GEN_NO_BRIGHT_COEFF | TSMAT_GEN_ONLY_ONE_COMPONENT, /*bStrict*/true };
} // Test