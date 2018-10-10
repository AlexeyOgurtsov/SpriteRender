#pragma once

#include "../ISprPerTestFixtureBase.h"
#include "ISpriteRender/ISpriteRender.h"

namespace Test::ISpr
{
	ISprPerTestFixtureBase::ISprPerTestFixtureBase()
	{
		T_LOG("ISprPerTestFixtureBase::CTOR...");
		T_LOG("ISprPerTestFixtureBase::CTOR DONE");
	}

	ISprPerTestFixtureBase::~ISprPerTestFixtureBase()
	{
		T_LOG("ISprPerTestFixtureBase::~DTOR...");
		T_LOG("ISprPerTestFixtureBase::~DTOR DONE");
	}

	void ISprPerTestFixtureBase::SetupTest(const char *InTestName, UINT InResetFlags)
	{
		T_LOG("ISprPerTestFixtureBase::SetupTest...");
		SetupTestBase(InTestName, InResetFlags);
		T_LOG("ISprPerTestFixtureBase::SetupTest DONE");
	}

	void ISprPerTestFixtureBase::CommitFrame()
	{
		ISpriteRender* pRen = GetSprRen();
		BOOST_ASSERT(pRen);

		// Rendering frame
		pRen->BeginFrame();
		pRen->RenderAllVisibleCanvasses();
		pRen->EndFrame();

		// @TODO Should we tick here?
	}
		
	// @TODO: CommitFrame
} // Test::ISpr