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
		virtual ID3D11DeviceContext* GetDevCon() const override;

		// ~Frame resource accessors Begin		
		virtual ID3D11Texture2D* GetRT() const override;
		virtual ID3D11Texture2D* GetDepthStencil() const override;
		// ~Frame resource accessors End
		
	protected:
		std::ofstream& GetLog() const;
		virtual const TesterConfig_D3DDevice& GetD3DConfig() const override;

	private:
		PerTestFixtureBase* TestFixture = nullptr;
	};
} // Test::IMPL