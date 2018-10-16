#pragma once

#include "Test/ISpriteRender/ISprGlobalFixtureBase.h"
#include "SpriteRenderManager.h"

namespace Test::ISpr::QSpr
{
	/**
	* Base class for global fixtures for testing the SpriteRender implementation.
	*/
	class QSprGlobFixtureBase : public ISpr::ISprGlobFixtureBase
	{
	public:
		/**
		* Default ctor: WARNING!!! Automatically initializes default config.
		* NO Init method necessary!
		*/
		QSprGlobFixtureBase();
		~QSprGlobFixtureBase();
	protected:
		virtual void DoSpriteRenderManager_CustomSetup() override;

	private:
	}; 
} // Test::ISpr::QSpr