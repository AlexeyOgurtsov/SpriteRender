#pragma once

#include <memory>
#include <d3d11.h>
#include "CONFIG/D3DDeviceConfig.h"

namespace Test
{
	class PerTestFixtureBase;

	/**
	* Render result check methods.
	*/
	class IFrameCheckContext
	{
	public:
		virtual ~IFrameCheckContext() = default;

		virtual const char* GetTestName() const =0;
		virtual PerTestFixtureBase* GetFixture() const = 0;
		virtual ID3D11DeviceContext* GetDevCon() const = 0;

		// ~ Frame resource helper accessors Begin
		DXGI_FORMAT GetRTFormat() const;
		DXGI_FORMAT GetDepthStencilFormat() const;
		const TesterConfig_D3DDevice_RenderTarget& GetRTConfig() const;
		const TesterConfig_D3DDevice_DepthStencil& GetDepthStencilConfig() const;

		/**
		* Width of the render target in texels.
		*/
		UINT GetRTWidth() const;

		/**
		* Height of the render target in texels.
		*/
		UINT GetRTHeight() const;
		// ~ Frame resource helper accessors End

		// ~Frame resource accessors Begin
		/**
		* Texture containing RT Data.
		*
		* Must allow MAP_READ access (so, must be STAGING).
		*/
		virtual ID3D11Texture2D* GetRT() const = 0;

		/**
		* Texture containing Depth Stencil Data.
		*
		* Must allow MAP_READ access (so, must be STAGING).
		*/
		virtual ID3D11Texture2D* GetDepthStencil() const = 0;
		// ~Frame resource accessors End

	protected:
		virtual const TesterConfig_D3DDevice& GetD3DConfig() const = 0;

	private:
		const D3D11_TEXTURE2D_DESC& GetRTDesc() const;
		const D3D11_TEXTURE2D_DESC& GetDepthStencilDesc() const;
		
		mutable D3D11_TEXTURE2D_DESC RTDesc;
		mutable D3D11_TEXTURE2D_DESC DepthStencilDesc;
	};

	using IFrameCheckContextHandle = std::unique_ptr<IFrameCheckContext>;
} // Test