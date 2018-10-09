#include "RenCommandListUtils.h"

namespace Dv
{
namespace Spr
{
namespace Ren
{
namespace IMPL
{

namespace D3D
{

CommandListHandle FinishCommandList(ID3D11DeviceContext* pDevCon)
{
	ID3D11CommandList* pCommandList = nullptr;
	BOOL bRestoreDeferredContextState = FALSE;
	HRESULT hr = pDevCon->FinishCommandList(bRestoreDeferredContextState, &pCommandList);
	if(FAILED(hr))
	{
		throw SpriteRenderHRFailure("FinishCommandList failed", hr);
	}
	return CommandListHandle(pCommandList);
}

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Dv