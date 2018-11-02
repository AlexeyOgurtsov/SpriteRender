#pragma once

#include "../ISprPerTestFixtureBase.h"
#include "ISpriteRender/ISpriteRender.h"
#include "ISpriteRenderTestUtils/FrameCheckContext/FrameCheckContext.h"

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

		OnPostSetupTest();
	}

	void ISprPerTestFixtureBase::OnPreCommitFrame()
	{
		T_LOG("OnPreCommitFrame...");
		OnPreCommitFrameUser();
		T_LOG("OnPreCommitFrame DONE");
	}

	void ISprPerTestFixtureBase::OnPostSetupTest()
	{
		T_LOG("OnPostSetupTest...");
		OnPostSetupTestUser();
		T_LOG("OnPostSetupTest DONE");
	}

	IFrameCheckContextHandle ISprPerTestFixtureBase::CommitFrame(int InMainLoopIterCount, int InTickCount, bool bCallRender)
	{
		T_LOG("CommitFrame...");

		OnPreCommitFrame();

		BOOST_ASSERT(InMainLoopIterCount >= 0);
		T_LOG("LoopIterCount=" << InMainLoopIterCount);
		for(int i = 0; i < InMainLoopIterCount; i++)
		{
			if (bCallRender)
			{
				RenderFrame();
				RenderPickFrame_IfPickEnabled();
			}
			TickN(InTickCount);
		}
		T_LOG("CommitFrame RETURNING");

		return CreateFrameCheckContext();
	}

	IFrameCheckContextHandle ISprPerTestFixtureBase::CreateFrameCheckContext()
	{
		return std::make_unique<Test::IMPL::FrameCheckContext>(this);
	}

	void ISprPerTestFixtureBase::RenderFrame()
	{
		ISpriteRender* pRen = GetSprRen();
		BOOST_ASSERT(pRen);

		T_LOG("ISpr PerTest: RenderingFrame...");

		BeginFrame();

		pRen->BeginFrame();
		pRen->RenderAllVisibleCanvasses();
		pRen->EndFrame();

		EndFrame();

		T_LOG("ISpr PerTest: RenderingFrame DONE");
	}

	void ISprPerTestFixtureBase::RenderPickFrame_IfPickEnabled()
	{
		ISpriteRender* pRen = GetSprRen();
		BOOST_ASSERT(pRen);

		T_LOG("ISpr PerTest: RenderPickFrame_IfPickEnabled...");
		if (false == GetConfig().Pick.bEnabled)		
		{
			T_LOG("ISpr PerTest: Pick is disabled, skipping rendering pick frame");
		}
		else
		{
			T_LOG("ISpr PerTest: Render pick frame...");
			pRen->BeginPickFrame();
			pRen->RenderAllPickableCanvasses();
			pRen->EndPickFrame();
			T_LOG("ISpr PerTest: Render pick frame DONE");
		}
		T_LOG("ISpr PerTest: RenderPickFrame_IfPickEnabled DONE");
	}

	void ISprPerTestFixtureBase::TickN(int InTickCount)
	{
		T_LOG("ISpr PerTest: Ticking...");
		int RealCountToTick = InTickCount;
		if (InTickCount == 0)
		{
			int DefaultTickCount = 1;
			T_LOG("0 passed for tick count, using default count: " << DefaultTickCount);
			RealCountToTick = DefaultTickCount;
		}
		if (InTickCount < 0)
		{
			T_LOG("NEGATIVE value passed for tick count => skipping ticking");
			RealCountToTick = 0;
		}
		T_LOG("Real count of ticks: " << RealCountToTick);

		// The first iteration the real delta seconds since last begin frame is to be passed.
		// For next iterations (as they are consecutive) it's like many loop iterations are called.
		float DeltaSecondsToPass = GetDeltaSeconds_SinceLastBeginFrame();
		for (int i = 0; i < RealCountToTick; i++)
		{			
			TickEnv(DeltaSecondsToPass);
			DeltaSecondsToPass = 1; // For next iteration (Sleep(1) is accounted)
		}

		T_LOG("ISpr PerTest: Ticking DONE");
	}


	TexelColor ISprPerTestFixtureBase::CanvasColorAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint)
	{
		return RT_ColorByCanvasPoint(ContextHandle.get(), InCanvas, InCanvasPoint);
	}

	bool ISprPerTestFixtureBase::CanvasClearAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, bool bInMatchAlpha)
	{
		return RT_IsCanvasClear_AtCanvasPoint(ContextHandle.get(), InCanvas, InCanvasPoint, bInMatchAlpha);
	}

	bool ISprPerTestFixtureBase::ColorMatchesCanvasAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, const TexelColor& InColor, bool bInMatchAlpha)
	{	
		return RT_ColorMatchesCanvas_AtCanvasPoint(ContextHandle.get(), InCanvas, InCanvasPoint, InColor, bInMatchAlpha);
	}

	bool ISprPerTestFixtureBase::TextureMatchesCanvasAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, const SprVec2& InCanvasPoint, const IMPL::TextureElement& InTextureElement, bool bInMatchAlpha)
	{
		return RT_TextureMatches_CanvasPoint(ContextHandle.get(), InCanvas, InCanvasPoint, InTextureElement, bInMatchAlpha);
	}

	bool ISprPerTestFixtureBase::SpriteVisibleAsColor(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, const TexelColor& InColor, bool bInMatchAlpha)
	{	
		return RT_SpriteVisibleAsColor(ContextHandle.get(), InCanvas, InSprite, InColor, bInMatchAlpha);
	}

	bool ISprPerTestFixtureBase::SpriteHidden(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, bool bInMatchAlpha)
	{
		return RT_SpriteHidden(ContextHandle.get(), InCanvas, InSprite, bInMatchAlpha);
	}

	bool ISprPerTestFixtureBase::SpriteHiddenAt(CanvasHandle InCanvas, const IFrameCheckContextHandle& ContextHandle, SpriteHandle InSprite, const SprVec2& InCanvasPoint, bool bInMatchAlpha)
	{
		return RT_SpriteHidden_AtCanvasPoint(ContextHandle.get(), InCanvas, InSprite, InCanvasPoint, bInMatchAlpha);
	}
} // Test::ISpr