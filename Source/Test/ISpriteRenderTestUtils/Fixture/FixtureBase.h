#pragma once

#include "ISpriteRenderTestUtils/TesterConfig.h"
#include "../Utils/TestUtils.h"
#include "../ISpriteRenderSubsystemManager_PublicTypes.h"

namespace Test
{
	/**
	* @see: GetSpriteRenderSubsystemManager() getter.
	*/
	class ISpriteRenderSubsystemManager;

	/**
	* Base class for UNIT-TEST fixtures of any types (Globals, Tests).
	*/
	class FixtureBase
	{
	public:
		FixtureBase();
		~FixtureBase();

		UINT GetRTWidth() const;
		UINT GetRTHeight() const;

		// ~ Sprite Render helper accessors Begin
		/**
		* Gets the default texture format of the sprite render.
		*/
		DXGI_FORMAT GetSprRen_DefaultTextureFormat_Diffuse() const;
		// ~ Sprite Render helper accessors End		

		std::ofstream& GetLog() const;



		void PushConfig(const TesterConfig& InConfig);

		/**
		* Pops config.
		* Default config should not (and cannot) be popped.
		*/
		void PopConfig();


		TesterConfig* BeginUpdateConfig();
		void EndUpdateConfig(const TesterConfig* pConfig);

	protected:
		/**
		* Is environment globally ready for testing (still may be that individual test is NOT set up)
		*/
		bool IsGloballyReadyForTesting() const;

		/**
		* Returns config to be used now.
		*/
		const TesterConfig& GetConfig() const;

		/**
		* Returns true if default config is set at least once.
		*/
		bool IsDefaultConfigSet() const;

		/**
		* Initializes only the Core (logs etc).
		*/
		void InitMinimalCore();

		/**
		* ReInitializes viewport.
		*/
		void ReInitViewport();

		/**
		* ReInitializes the D3DDevice.
		*/
		void ReInitD3DDevice();

		/**
		* ReInitializes the Resources.
		*/
		void ReInitResources();

		/**
		* ReInit SpriteRender
		*/
		void ReInit_SpriteRender();

		/**
		* We allow to access ISpriteRenderSubsystemManager,
		* But! Use it only for implementation details only (use helper methods whenever possible!).
		* That's why we used long name delibarately.
		*
		* Access is allowed because otherwise concrete fixtures will be depend on the IMPL::Environment.
		*/		
		ISpriteRenderSubsystemManager* GetSpriteRenderSubsystemManager() const;

		bool PromptPresentationMode_ReturnTrueIfQuit();

	private:
		void UpdateEnvironment_BasedOnConfig(const TesterConfig& InConfig);
		bool bUpdatingConfig = false;
	};
	void DisableInteractiveMode(FixtureBase* pFixture);
} // Test