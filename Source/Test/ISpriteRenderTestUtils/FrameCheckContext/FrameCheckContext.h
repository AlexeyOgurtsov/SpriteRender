#pragma once

#include "../IFrameCheckContext.h"
#include "../Utils/TestUtils.h"

namespace Test::IMPL
{
	class FrameCheckContext : public IFrameCheckContext
	{
	public:
		FrameCheckContext(PerTestFixtureBase* InTestFixture);
		~FrameCheckContext();

		virtual const char* GetTestName() const override;
		virtual PerTestFixtureBase* GetFixture() const override { return TestFixture; }
		
	protected:
		std::ofstream& GetLog() const;

	private:
		PerTestFixtureBase* TestFixture = nullptr;
	};
} // Test::IMPL