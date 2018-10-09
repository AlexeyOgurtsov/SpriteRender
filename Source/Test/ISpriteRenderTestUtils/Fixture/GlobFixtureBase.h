#pragma once

#include "FixtureBase.h"

namespace Test
{
	class ISpriteRenderSubsystemManager;

	class GlobFixtureBase : public FixtureBase
	{
	public:
		GlobFixtureBase();
		~GlobFixtureBase();

	protected:
		/**
		* Sets the default config and initializes core subsystems and d3d11.
		*/
		void InitGlobal(const TesterConfig& InDefaultConfig);
	
		/**
		* Sets the sprite render manager (must be created in dynamic memory with new).
		*/
		void SetSpriteRenderManager(ISpriteRenderSubsystemManager* pInManager);

		/**
		* Performs custom setup for the sprite render after it has been created.
		* The sprite render manager is guaranteed to exist.
		*/
		virtual void DoSpriteRenderManager_CustomSetup() = 0;

	private:
		void UpdateSpriteRenderManagerState(ISpriteRenderSubsystemManager* pInManager);
	};
} // Test