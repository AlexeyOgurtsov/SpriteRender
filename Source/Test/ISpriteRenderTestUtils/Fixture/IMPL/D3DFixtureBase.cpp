#include "../D3DFixtureBase.h"
#include "../../Utils/TestUtils.h"
#include "../../IMPL/Environment.h"

namespace Test
{
	namespace 
	{
		IMPL::Environment* GetEnv() { return IMPL::Environment::Get(); }
	} // anonymous

D3DFixtureBase::D3DFixtureBase()
{
}

D3DFixtureBase::~D3DFixtureBase()
{
}

void D3DFixtureBase::SetupTest(const char* InTestName)
{
	SetupTestBase(InTestName, RESET_NO_SPRITE_RENDER);
}

ID3D11Device* D3DFixtureBase::GetDev()
{
	return GetEnv()->GetD3DDevice()->GetDev();
}

ID3D11DeviceContext* D3DFixtureBase::GetDevCon()
{
	return GetEnv()->GetD3DDevice()->GetDevCon();
}

IDXGISwapChain* D3DFixtureBase::GetSwapChain()
{
	return GetEnv()->GetD3DDevice()->GetSwapChain();
}

} // Test