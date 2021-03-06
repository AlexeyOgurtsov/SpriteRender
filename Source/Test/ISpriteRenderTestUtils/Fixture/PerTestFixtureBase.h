#pragma once

#include "FixtureBase.h"
#include "../SprRenHelper/SprRenHelper_ISpriteRenderFwd.h"
#include "../Environment_PublicTypes.h"
#include <string>

namespace Test
{
	class IFrameCheckContext;

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

		/**
		* WARNING!!! To be called from the IFrameCheckContext objects only!
		*/
		void OnFrameCheckContextReleased(IFrameCheckContext* pInSender);

	protected:
		/**
		* Typically to be called from the SetupTest function of the concrete descendant fixture.
		*/
		void SetupTestBase(const char* InTestName, unsigned int InResetFlags = 0);	

		/**
		* Pauses the testing according to setting.
		*/
		void Pause_IfEnabled();

		/**
		* Ticks the environment.
		*
		* May close the application.
		*/
		void TickEnv(float InDeltaSeconds);
		void BeginFrame();
		void EndFrame();

		virtual void OnFrameCheckContextReleasedUser(IFrameCheckContext* pInSender) {}

		/**
		* Index of the frame relative to the current test time.
		*/
		int GetLocalFrameIndex() const { return LocalFrameIndex; }

		/**
		* Returns delta seconds since the last frame.
		*/
		float GetDeltaSeconds_SinceLastBeginFrame() const;

	private:
		/**
		* Terminate the application.
		*
		* Normally terminates with error state corresponding the test.
		*/
		void Quit();
		void ResetEnv(unsigned int InResetFlags);
		void ShowTestInfo_IfEnabled();

		void BeginInteractive(bool bPauseTimers = true);
		void EndInteractive();
		bool bInteractive = false;

		int LocalFrameIndex; // set from the ctor

		bool bSetUp; // set from the ctor
		std::string TestName; // set from the ctor
	};
} // Test
