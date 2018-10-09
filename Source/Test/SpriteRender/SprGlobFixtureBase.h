#pragma once

#include "Test/ISpriteRender/ISprGlobalFixtureBase.h"
#include "SpriteRenderManager.h"

namespace Test::ISpr::Spr
{
	/**
	* Base class for global fixtures for testing the SpriteRender implementation.
	*/
	class SprGlobFixtureBase : public ISpr::ISprGlobFixtureBase
	{
	public:
		/**
		* Default ctor: WARNING!!! Automatically initializes default config.
		* NO Init method necessary!
		*/
		SprGlobFixtureBase();
		~SprGlobFixtureBase();
	protected:
		virtual void DoSpriteRenderManager_CustomSetup() override;

	private:
	}; 
} // Test::ISpr::Spr