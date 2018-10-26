#include "TestTypes.h"

namespace Test
{
	TestMaterial::TestMaterial
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
		const TestMaterial& InMaterial,
		const MySprMath::SSize& InInitSize,
		const MySprMath::SVec2& InInitPos
	) :
		InitMaterial{InMaterial}
	,	InitSize{InInitSize}
	,	InitPos{InInitPos} {}


	void TSSprite::SetHandle(const SpriteHandle& InHandle) 
	{
		BOOST_ASSERT_MSG(nullptr == InHandle || IsNotCreated(), "TSSprite: You cannot assign handle for already-created sprite");
		Handle = InHandle;
	}
} // Test