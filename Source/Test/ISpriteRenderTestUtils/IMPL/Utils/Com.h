#pragma once

#include <memory>

struct IUnknown;

namespace Test::IMPL
{
	struct ComReleaser
	{
		void operator() (IUnknown* pNullOrInterface);
	};

	template<class T>
	using TCom = std::unique_ptr<T, ComReleaser>;
} // Test::IMPL