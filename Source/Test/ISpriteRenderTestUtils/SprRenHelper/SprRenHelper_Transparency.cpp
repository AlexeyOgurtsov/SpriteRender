#include "SprRenHelper_Transparency.h"

namespace Test
{
	SpriteTransparencyMode::SpriteTransparencyMode(MySpr::ESpriteTransparency InTransparency) :
		Transparency{InTransparency}
	{
	}

	const SpriteTransparencyMode SpriteTransparencyMode::Opaque { MySpr::ESpriteTransparency::Opaque };

	// @TODO!!! Set the real transparency mode corresponding the additive alpha blending
	const SpriteTransparencyMode SpriteTransparencyMode::AlphaBlend_Additive { MySpr::ESpriteTransparency::BlendAdditive };

} // Test