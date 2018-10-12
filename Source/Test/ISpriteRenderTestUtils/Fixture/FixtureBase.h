#pragma once

#include "ISpriteRenderTestUtils/TesterConfig.h"
#include "../Utils/TestUtils.h"

namespace Test
{
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

	protected:
		std::ofstream& GetLog() const;

		/**
		* Is environment globally ready for testing (still may be that individual test is NOT set up)
		*/
		bool IsGloballyReadyForTesting() const;

		/**
		* Returns configuration to be used by default (some tests may override).
		*/
		const TesterConfig& GetDefaultConfig() const;

		/**
		* Returns true if default config is set at least once.
		*/
		bool IsDefaultConfigSet() const;

		/**
		* Sets the default configuration.
		* (WARNING!!! No reinitialization will be performed automatically).
		*/		
		void ResetDefaultConfig(const TesterConfig& InConfig);

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

	private:
	};
} // Test