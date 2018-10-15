#include "FrameCheckContext.h"
#include "../IMPL/Environment.h"
#include "../Fixture/PerTestFixtureBase.h"

namespace Test::IMPL
{
	namespace
	{
		Environment* GetEnv() { return Environment::Get(); }
	} // anonymous

	FrameCheckContext::FrameCheckContext(PerTestFixtureBase* InTestFixture) :
		TestFixture(InTestFixture)
	{
		T_LOG("FrameCheckContext ctor for TEST '" << GetTestName() <<  "' ...");
		BOOST_ASSERT(TestFixture);
		IMPL::UpdateCopyBuffers(GetLog(), GetEnv()->GetD3DDevice());
		T_LOG("FrameCheckContext ctor DONE");
	}

	FrameCheckContext::~FrameCheckContext()
	{
		T_LOG("FrameCheckContext::~DTOR...");
		GetFixture()->OnFrameCheckContextReleased(this);
		T_LOG("FrameCheckContext::~DTOR DONE");
	}

	const char* FrameCheckContext::GetTestName() const
	{
		return GetFixture()->GetTestName().c_str();
	}

	std::ofstream& FrameCheckContext::GetLog() const
	{
		return GetEnv()->GetMainLog();
	}

	ID3D11Texture2D* FrameCheckContext::GetRT() const
	{
		return GetEnv()->GetD3DDevice()->GetCopyBuffer_RT();
	}

	ID3D11Texture2D* FrameCheckContext::GetDepthStencil() const
	{
		return GetEnv()->GetD3DDevice()->GetCopyBuffer_DepthStencil();
	}

	ID3D11DeviceContext* FrameCheckContext::GetDevCon() const
	{
		return GetEnv()->GetD3DDevice()->GetDevCon();
	}

	const TesterConfig_D3DDevice& FrameCheckContext::GetD3DConfig() const
	{
		return GetEnv()->GetD3DDevice()->GetConfig();
	}
} // Test::IMPL