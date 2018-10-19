#pragma once

#include "PerTestFixtureBase.h"
#include <d3d11.h>

namespace Test
{
	class D3DFixtureBase : public PerTestFixtureBase
	{
	public:
		D3DFixtureBase();
		~D3DFixtureBase();

		/**
		* Must be called from any test.
		*/
		void SetupTest(const char* InTestName);

		ID3D11Device* GetDev();
		ID3D11DeviceContext* GetDevCon();
		IDXGISwapChain* GetSwapChain();

	private:
	};
} // Test