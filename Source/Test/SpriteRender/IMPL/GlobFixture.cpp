#pragma once

#include "Test/SpriteRender/GlobFixture.h"
#include "Test/SpriteRender/Config/DefaultGlobalConfig.h"

namespace Test::ISpr::Spr
{

	GlobFixture::GlobFixture() :
		SprGlobFixtureBase{}
	{
		T_LOG("GlovFixtureBase: Construction...");

		// @TODO: For Test ONLY!
		//T_LOG("TEST! SpriteRender: FixtureBase: Initializing SpriteRender...");
		//ReInit_SpriteRender();
		//T_LOG("TEST! SpriteRender: FixtureBase: Initializing SpriteRender DONE");

		T_LOG("GlovFixtureBase: Construction DONE");
	}

	GlobFixture::~GlobFixture()
	{
		// @TODO
	}
} // Test::ISpr::Spr