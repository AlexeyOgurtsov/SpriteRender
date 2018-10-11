#pragma once

#include <memory>

namespace Test
{
	class PerTestFixtureBase;

	/**
	* Render result check methods.
	*/
	class IFrameCheckContext
	{
	public:
		~IFrameCheckContext() = default;

		virtual const char* GetTestName() const =0;
		virtual PerTestFixtureBase* GetFixture() const = 0;
		
		/**
		* @TODO: Pixel check
		*/
	};

	using IFrameCheckContextHandle = std::unique_ptr<IFrameCheckContext>;
} // Test