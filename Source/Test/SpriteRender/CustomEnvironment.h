#pragma once

#include "CONFIG/SpriteRenderCustomSetuper.h"

namespace Test::ISpr::Spr::IMPL
{
	/**
	* Environment specific to the current SpriteRender implementation
	*/
	class CustomEnvironment
	{
	public:
		static CustomEnvironment& Get() 
		{
			static CustomEnvironment Obj;
			return Obj;
		}

		/**
		* Custom setuper of the Sprite Render.
		*/
		const SpriteRenderCustomSetuper& GetDefaultCustomSettings() const { return RenCustomSetuper; }

		/**
		* Call to set the new settings for the custom setuper.
		*
		* Notifies the SpriteRenderManager.
		*/
		void ResetDefaultCustomSettings(const SpriteRenderCustomSetuper& InSetuper);

	private:
		SpriteRenderCustomSetuper RenCustomSetuper;
	};
} // Test::ISpr::Spr::IMPL