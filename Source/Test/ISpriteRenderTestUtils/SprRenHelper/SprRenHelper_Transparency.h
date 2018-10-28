#pragma once

#include "ISprite/SpriteRenderMode.h"
#include "SprRenHelper_System.h"

namespace Test
{
	/**
	* Abstracts from ISpriteRender interface transparency mode implementation.	
	*/
	class SpriteTransparencyMode
	{
	public:
		SpriteTransparencyMode(MySpr::ESpriteTransparency InTransparency);

		/**
		* Underlying ISpriteRender transparency
		*/
		MySpr::ESpriteTransparency GetTransparency() const { return Transparency; }
	
		/**
		* Opaque mode.
		*/
		static const SpriteTransparencyMode Opaque;

		/**
		* Additive alpha-blending.
		*/
		static const SpriteTransparencyMode AlphaBlend_Additive;

	private:
		MySpr::ESpriteTransparency Transparency;
	};
} // Test