#pragma once

#include "Test/ISpriteRenderTestUtils/Fixture/PerTestFixtureBase.h"
#include "ISpriteRenderTestUtils/IFrameCheckContext.h"

namespace Test::ISpr
{
	class ISprPerTestFixtureBase : public PerTestFixtureBase
	{
	public:
		ISprPerTestFixtureBase();
		~ISprPerTestFixtureBase();

		void SetupTest(const char *InTestName, UINT InResetFlags = 0);
		
		
		/**
		* @argument InTickCount: 
		* - By default (zero passed) calls tick default number of times;
		* - If you pass negative value ticking will be disabled;
		* Otherwise calls Tick the specified number of times.
		*
		* @argument InMainLoopIterCount: Call main loop interations exactly specified number of times.
		*/
		IFrameCheckContextHandle CommitFrame(int InMainLoopIterCount = 1, int InTickCount = 0, bool bCallRender = true);
		constexpr static int DISABLE_TICK = -1; // Pass as TickCount to disable ticking (@see CommitFrame)

	protected:
		virtual IFrameCheckContextHandle CreateFrameCheckContext();

		/**
		* Called after SetupTest
		*/
		virtual void OnPostSetupTestUser() {}

		/**
		* This function is called each time before CommitFrame is called.
		*/		
		virtual void OnPreCommitFrameUser() {}

		/**
		* Render all canvasses of the sprite render.
		*/
		void RenderFrame();
		void TickN(int InTickCount);

	private:		
		void OnPreCommitFrame();
		void OnPostSetupTest();
	};
} // Test::ISpr