#include "RenDeviceContextUtils.h"

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace IMPL
{

namespace D3D
{

/**
* Tries to create deferred device context and throws an exception if error has been occured.
*/
DeviceContextHandle CreateDeferredContext(ID3D11Device* pInDev)
{
	ID3D11DeviceContext* pDevCon = nullptr;
	HRESULT hr = pInDev->CreateDeferredContext(0, &pDevCon);
	if(FAILED(hr))
	{
		throw SpriteRenderHRFailure("CreateDeferredContext failed", hr);
	}
	return DeviceContextHandle(pDevCon);
}

} // Dv::Spr::QRen::IMPL::D3D

} // Dv::Spr::QRen::IMPL
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv