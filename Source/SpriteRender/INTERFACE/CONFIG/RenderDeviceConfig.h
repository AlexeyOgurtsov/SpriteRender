#pragma once

#include <d3d11.h>
#include <boost/assert.hpp>

namespace Dv
{
namespace Spr
{
namespace Ren
{

/**
* Pointers to the d3d11 device interfaces to be use.
*/
	struct SRenderDeviceConfig
	{
		/**
		* d3d11 device to be used.
		*/
		ID3D11Device* pDev;

		/**
		* Immediate device context to be used.
		*/
		ID3D11DeviceContext* pImmDevCon;

		SRenderDeviceConfig() :
			pDev{ nullptr }
		,	pImmDevCon{ nullptr } {}
		SRenderDeviceConfig(ID3D11Device* pInDev, ID3D11DeviceContext* pInImmDevCon) :
			pDev{pInDev}
		,	pImmDevCon{pInImmDevCon} {}

};

/**
* Checks that config is valid.
* (meaning that the subsystem will work with the given properties)
*/
inline void AssertDeviceConfigValid(const SRenderDeviceConfig& InConfig)
{
	BOOST_ASSERT_MSG(InConfig.pDev, "SpriteRender config is invalid: device pointer must be valid");
	BOOST_ASSERT_MSG(InConfig.pImmDevCon, "SpriteRender config is invalid: immediate device context must be valid");
	BOOST_ASSERT_MSG(InConfig.pImmDevCon->GetType() == D3D11_DEVICE_CONTEXT_IMMEDIATE, "SpriteRender config is invalid: immediate device context must be passed");
}

} // Dv::Spr::Ren
} // DV::Spr
} // Dv