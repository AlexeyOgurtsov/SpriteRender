#pragma once

#include "Test/QSpriteRender/QGlobFixture.h"
#include "Test/QSpriteRender/Config/DefaultGlobalConfig.h"

namespace Test::ISpr::QSpr
{

	QGlobFixture::QGlobFixture() :
		QSprGlobFixtureBase{}
	{
		T_LOG("QGlobFixture: Construction...");

		// @TODO: For Test ONLY!
		//T_LOG("TEST! QSpriteRender: FixtureBase: Initializing SpriteRender...");
		//ReInit_SpriteRender();
		//T_LOG("TEST! QSpriteRender: FixtureBase: Initializing SpriteRender DONE");

		T_LOG("QGlobFixture: Construction DONE");
	}

	QGlobFixture::~QGlobFixture()
	{
		// @TODO
	}
} // Test::ISpr::QSpr