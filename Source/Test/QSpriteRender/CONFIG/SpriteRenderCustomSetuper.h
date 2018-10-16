#pragma once

#include "QSpriteRender/INTERFACE/SpriteRenderSubsystemInitializer.h"

namespace Test::ISpr::QSpr
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
		void SetupCustomSettings(Dv::Spr::QRen::SSpriteRenderSubsystemInitializer& OutInitializer) const;

		void SetInitialSpriteBufferCapacity(UINT InValue) { InitialSpriteBufferCapacity = InValue; }
		void SetZFar(float InZFar) { RenderTarget_ZFar = InZFar; }
		void SetShadersConfig(const Dv::Spr::QRen::SShadersConfig& InShadersConfig) { ShadersConfig = InShadersConfig; }

	private:
		UINT InitialSpriteBufferCapacity = Dv::Spr::QRen::INITIAL_SPRITE_BUFFER_CAPACITY;
		float RenderTarget_ZFar = Dv::Spr::QRen::DEFAULT_ZFAR;
		Dv::Spr::QRen::SShadersConfig ShadersConfig;
	};
} // Test::ISpr::QSpr