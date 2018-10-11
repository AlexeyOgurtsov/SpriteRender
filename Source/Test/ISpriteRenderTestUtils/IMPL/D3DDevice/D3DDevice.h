#pragma once

#include <d3d11.h>
#include <memory>
#include <fstream>
#include "ISpriteRenderTestUtils/CONFIG/D3DDeviceConfig.h"
#include "ISpriteRenderTestUtils/IMPL/Utils/Com.h"
#include "ISpriteRenderTestUtils/IMPL/Utils/DepthStencil.h"

namespace Test
{
	struct TesterConfig_D3DDevice;
} // Test

namespace Test::IMPL
{
	class D3DDevice
	{
	public:
		virtual ~D3DDevice() = default;
		D3DDevice(UINT InRTWidth, UINT InRTHeight, std::ofstream* pInLog, HWND hInWnd, const TesterConfig_D3DDevice& InConfig);

		const TesterConfig_D3DDevice& GetConfig() const { return _config; }

		/**
		* Starts a new frame (clears old buffers contents).
		*/
		void BeginFrame(int InLocalFrameIndex, std::ofstream& InLog);

		/**
		* Ends a frame (Presents contents of the buffer)
		*/
		void EndFrame(int InLocalFrameIndex, std::ofstream& InLog);

		void                                  Tick(float InDeltaSecs);

		HRESULT                               GetHRCreate() const { return _HRCreate; }
		HRESULT                               GetHRGetBuffer() const { return _HRGetBuffer; }

		IDXGISwapChain*                       GetSwapChain() const { return _pSwapChain.get(); }
		ID3D11Device*                         GetDev() const { return _pDev.get(); }
		ID3D11DeviceContext*                  GetDevCon() const { return _pDevCon.get(); }

		ID3D11Texture2D*                      GetSwapChainBuffer() const { return _pSwapChainBuffer.get(); }
		ID3D11RenderTargetView*               GetSwapChainBufferRTView() const { return _pSwapChainBufferRTView.get(); }
		
		ID3D11DepthStencilView*               GetDepthStencilView () const { return _DS.GetView(); }
		ID3D11Texture2D*                      GetDepthStencil() const { return _DS.GetTex(); }

		const D3D11_VIEWPORT&                 GetViewport() const { return _viewport; }
		void                                  ResizeViewport(unsigned int InWidth, unsigned int InHeight);

		ID3D11Texture2D*                      GetCopyBuffer_RT() const { return _pCopyBuffer_RT.get(); }
		ID3D11Texture2D*                      GetCopyBuffer_DepthStencil() const { return _pCopyBuffer_DepthStencil.get(); }

		unsigned int                          GetRTWidth() const;
		unsigned int                          GetRTHeight() const;

		void                                  Notify_MainWindowAspectChanged(unsigned int InNewWidth, unsigned int InNewHeight);

	private:
		std::ofstream& GetLog() const { return *_pLog; }
		std::ofstream* _pLog = nullptr;

		HRESULT                                                      _HRCreate = E_FAIL;
		DXGI_SWAP_CHAIN_DESC                                         _swapChainDesc;
		std::unique_ptr<IDXGISwapChain, ComReleaser>          _pSwapChain;
		std::unique_ptr<ID3D11Device, ComReleaser>            _pDev;
		std::unique_ptr<ID3D11DeviceContext, ComReleaser>     _pDevCon;

		// Render target
		HRESULT                                                      _HRGetBuffer = E_FAIL;
		HRESULT                                                      _HRCreateRTView = E_FAIL;
		std::unique_ptr<ID3D11Texture2D, ComReleaser>				 _pSwapChainBuffer;
		std::unique_ptr<ID3D11RenderTargetView, ComReleaser>		 _pSwapChainBufferRTView;
		D3D11_VIEWPORT                                               _viewport;

		void                                                         _UpdateDS();
		DepthStencil                                                 _DS;
		
		void InitializeCopyBuffers();
		std::unique_ptr<ID3D11Texture2D, ComReleaser>                _pCopyBuffer_RT;
		std::unique_ptr<ID3D11Texture2D, ComReleaser>                _pCopyBuffer_DepthStencil;

		TesterConfig_D3DDevice                                       _config;

		struct SFrameRenderState 
		{
			/**
			* Was BeginFrame called?
			*/
			bool bFrameStarted                                   = false;

			/**
			* Local index of the frame
			*/
			int LocalFrameIndex                                  = -1;

			/**
			* Global frame index (since time the device is running)	
			*/
			int GlobalFrameIndex                                 = 0;

			void BeginFrame(int InLocalFrameIndex);
			void EndFrame(int InLocalFrameIndex);
		} FrameRender;
	};

	/**
	* Updates the state of copy buffers based on the current state of real buffers.
	*/
	void UpdateCopyBuffers(std::ofstream& InLog, D3DDevice* pD3D);

	/**
	* Clears depth-stencils, render-targets etc (@see Clear).
	* Plus purges and modifiable resources (textures), if any exists.
	*/
	void ClearAndPurgeDynamic(std::ofstream& InLog, D3DDevice* pD3D);

	/**
	* Clears depth-stencils, render-targets etc.
	*/
	void Clear(std::ofstream& InLog, D3DDevice* pD3D);
} // Test::IMPL