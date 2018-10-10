#pragma once

#include "FixtureBase.h"
#include "../ISpriteRenderFwd.h"
#include <string>

namespace Test
{
	class PerTestFixtureBase : public FixtureBase
	{
	public:
		PerTestFixtureBase();
		~PerTestFixtureBase();

		/**
		* Returns test name.
		*
		* WARNING!!! Must be called only if SetupTest is called.
		*/
		const std::string& GetTestName() const { return TestName; }

		/**
		* true, if test was set up.
		*/
		bool IsSetUp() const { return bSetUp; }

		/**
		* Gets current instance of the ISpriteRender interface.
		*
		* WARNING!!! Must be called only if SetupTest is called.
		*/
		ISpriteRender* GetSprRen() const;

	protected:
		/**
		* Typically to be called from the SetupTest function of the concrete descendant fixture.
		*/
		void SetupTestBase(const char* InTestName, unsigned int InResetFlags = 0);

	private:
		void ResetEnv(unsigned int InResetFlags);

		bool bSetUp; // set from the ctor
		std::string TestName;
	};
} // Test
