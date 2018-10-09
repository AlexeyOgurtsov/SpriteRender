#pragma once

#include "SpriteRender/INTERFACE/SpriteRenderSubsystemInitializer.h"

namespace Test::ISpr::Spr
{
	/**
	* Builds partly the SpriteRender Initializer structure from a set of input parameters.
	*/
	class SpriteRenderCustomSetuper
	{
	public:		
		/**
		* Setups the custom settings of the given initializer struct.
		*/
		void SetupCustomSettings(Dv::Spr::Ren::SSpriteRenderSubsystemInitializer& OutInitializer) const;

		void SetInitialSpriteBufferCapacity(UINT InValue) { InitialSpriteBufferCapacity = InValue; }
		void SetZFar(float InZFar) { RenderTarget_ZFar = InZFar; }
		void SetShadersConfig(const Dv::Spr::Ren::SShadersConfig& InShadersConfig) { ShadersConfig = InShadersConfig; }
		void SetRenderCachingConfig(const Dv::Spr::Ren::SRenderCachingConfig& InConfig) { RenderCachingConfig = InConfig; }

	private:
		UINT InitialSpriteBufferCapacity = Dv::Spr::Ren::INITIAL_SPRITE_BUFFER_CAPACITY;
		float RenderTarget_ZFar = Dv::Spr::Ren::DEFAULT_ZFAR;
		Dv::Spr::Ren::SShadersConfig ShadersConfig;
		Dv::Spr::Ren::SRenderCachingConfig RenderCachingConfig;
	};
} // Test::ISpr::Spr