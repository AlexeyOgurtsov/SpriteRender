#include "D3DDevice.h"
#include "D3DDevice_CONFIG_CONST.h"
#include "D3DDeviceException.h"
#include "ISpriteRenderTestUtils/IMPL/Utils/RenHRUtils.h"
#include "ISpriteRenderTestUtils/Utils/TestUtils.h"
#include "../Utils/RenTextureUtils.h"
#include "../Utils/Textures.h"
#include <fstream>
#include <iomanip>
#include <string_view>
#include <string>
#include <cassert>

namespace Test::IMPL
{
template<class ExType> void D3DDevice_HandleHR(D3DDevice* pDevice, HRESULT InHr, const std::string& InMessage);

std::string GetFeatureLevelString(D3D_FEATURE_LEVEL featureLevel);
std::string GetDeviceFlagsString(UINT InDeviceFlags);
std::string GetSingleDeviceFlagString(UINT InDeviceFlag);
std::string GetSwapEffectString(DXGI_SWAP_EFFECT InSwapEffect);

namespace
{
	void LogSwapChain(std::ofstream& InLog, const DXGI_SWAP_CHAIN_DESC& InDesc)
	{
		T_LOG_TO(InLog, std::left << std::setw(32) << "bWindowed: " << (InDesc.Windowed ? "TRUE" : "FALSE"));
		T_LOG_TO(InLog, std::left << std::setw(32) << "OutputWindow: " << InDesc.OutputWindow);
		T_LOG_TO(InLog, std::left << std::setw(32) << "Width:" << InDesc.BufferDesc.Width);
		T_LOG_TO(InLog, std::left << std::setw(32) << "Height:" << InDesc.BufferDesc.Height);
		T_LOG_TO(InLog, std::left << std::setw(32) << "RefreshRate:" << InDesc.BufferDesc.RefreshRate.Numerator << "/" << InDesc.BufferDesc.RefreshRate.Denominator);
		T_LOG_TO(InLog, std::left << std::setw(32) << "Format: " << GetFormatString(InDesc.BufferDesc.Format));
		T_LOG_TO(InLog, std::left << std::setw(32) << "SwapEffect: " << GetSwapEffectString(InDesc.SwapEffect));
		T_LOG_TO(InLog, std::left << std::setw(32) << "SampleDesc.Count: " << InDesc.SampleDesc.Count);
		T_LOG_TO(InLog, std::left << std::setw(32) << "SampleDesc.Quality: " << InDesc.SampleDesc.Quality);
		T_LOG_TO(InLog, std::left << std::setw(32) << "Flags: " << InDesc.Flags);
	}

	void LogConfig_CopyBuffers(std::ofstream& InLog, const TesterConfig_D3DDevice_CopyBuffers& InDesc)
	{
		T_LOG_TO(InLog, "D3DDevice: Logging CopyBuffers config...");
		T_LOG_TO(InLog, "bEnableRT: " << (InDesc.bEnableRT ? "TRUE" : "false"));
		T_LOG_TO(InLog, "bEnableDepthStencil: " << (InDesc.bEnableDepthStencil ? "TRUE" : "false"));
		T_LOG_TO(InLog, "D3DDevice: Logging CopyBuffers config DONE");
	}

	void LogConfig(std::ofstream& InLog, const TesterConfig_D3DDevice& InDesc)
	{
		T_LOG_TO(InLog, "D3DDevice: Logging config...");
		// @TODO:
		LogConfig_CopyBuffers(InLog, InDesc.CopyBuffers);
		T_LOG_TO(InLog, "D3DDevice: Logging config DONE");
	}

	void ClearCopyBuffers(std::ofstream& InLog, D3DDevice* pD3D)
	{
		T_LOG_TO(InLog, "D3DDevice: Clearing COPY buffers...");
		const TesterConfig_D3DDevice_CopyBuffers& Cfg = pD3D->GetConfig().CopyBuffers;

		if(Cfg.bEnableRT)
		{
			if(ID3D11Texture2D* pCopyTex = pD3D->GetCopyBuffer_RT())
			{
				T_LOG_TO(InLog, "Clearing RT Copy...");
				// WARNING!!! Because it's STAGING resource, we cannot use D3D11_MAP_WRITE_DISCARD!!!
				ZeroMainMip_ByMap(pD3D->GetDevCon(), pCopyTex, D3D11_MAP_WRITE);
				T_LOG_TO(InLog, "Clearing RT Copy DONE");
			}
			else
			{
				T_LOG_TO(InLog, "RT Copy buffer is nullptr");
			}
		}

		if(Cfg.bEnableDepthStencil)
		{
			if(ID3D11Texture2D* pCopyTex = pD3D->GetCopyBuffer_DepthStencil())
			{
				T_LOG_TO(InLog, "Clearing Depth-Stencil copy...");
				// WARNING!!! Because it's STAGING resource, we cannot use D3D11_MAP_WRITE_DISCARD!!!
				ZeroMainMip_ByMap(pD3D->GetDevCon(), pCopyTex, D3D11_MAP_WRITE);
				T_LOG_TO(InLog, "Clearing Depth-Stencil Copy DONE");
			}
			else
			{
				T_LOG_TO(InLog, "Depth-Stencil Copy buffer is nullptr");
			}
		}

		T_LOG_TO(InLog, "D3DDevice: Clearing COPY buffers DONE");
	}
} // anonymous


void UpdateCopyBuffers(std::ofstream& InLog, D3DDevice* pD3D)
{
	T_LOG_TO(InLog, "D3DDevice: Updating copy buffers...");

	if(ID3D11Texture2D* pCopyTex = pD3D->GetCopyBuffer_RT())
	{
		T_LOG_TO(InLog, "Updating RT copy...");
		BOOST_ASSERT_MSG(pD3D->GetSwapChainBuffer(), "UpdateCopyBuffers: RT must already exist");
		pD3D->GetDevCon()->CopyResource(pCopyTex, pD3D->GetSwapChainBuffer());
		T_LOG_TO(InLog, "Updating RT copy DONE");
	}

	if(ID3D11Texture2D* pCopyTex = pD3D->GetCopyBuffer_DepthStencil())
	{
		T_LOG_TO(InLog, "Updating Depth-Stencil copy...");
		BOOST_ASSERT_MSG(pD3D->GetDepthStencilView(), "UpdateCopyBuffers: Depth stencil view must already exist");
		pD3D->GetDevCon()->CopyResource(pCopyTex, pD3D->GetDepthStencil());
		T_LOG_TO(InLog, "Updating Depth-Stencil copy DONE");
	}

	T_LOG_TO(InLog, "D3DDevice: Updating copy buffers DONE");
}

void ClearAndPurgeDynamic(std::ofstream& InLog, D3DDevice* pD3D)
{
	T_LOG_TO(InLog, "D3DDevice: Clearing and purging...");
	BOOST_ASSERT(pD3D);
	Clear(InLog, pD3D);
	// @TODO: Should we purge anything here?
	T_LOG_TO(InLog, "D3DDevice: Clearing and purging DONE");
}

void Clear(std::ofstream& InLog, D3DDevice* pD3D)
{
	T_LOG_TO(InLog, "D3DDevice: Clearing...");

	const TesterConfig_D3DDevice& Cfg = pD3D->GetConfig();

	BOOST_ASSERT(pD3D);
	if (ID3D11RenderTargetView* pView = pD3D->GetSwapChainBufferRTView())
	{
		T_LOG_TO(InLog, "D3DDevice: Render target will be cleared");
		pD3D->GetDevCon()->ClearRenderTargetView(pView, Cfg.RenderTarget.GetClearColorFloat());
	}

	if (ID3D11DepthStencilView* pView = pD3D->GetDepthStencilView())
	{
		T_LOG_TO(InLog, "D3DDevice: DepthStencil is to be cleared");
		pD3D->GetDevCon()->ClearDepthStencilView(pView, Cfg.DepthStencil.GetClearFlags(), Cfg.DepthStencil.ClearZ, Cfg.DepthStencil.ClearStencil);
	}	

	ClearCopyBuffers(InLog, pD3D);

	T_LOG_TO(InLog, "D3DDevice: Clearing DONE");
}

D3DDevice::D3DDevice(UINT InRTWidth, UINT InRTHeight, std::ofstream* pInLog, HWND hInWnd, const TesterConfig_D3DDevice& InConfig) :
	_pLog(pInLog)
,	_config(InConfig)
{
	assert(_pLog);

	T_LOG_TO(*pInLog, "D3DDevice: ctor...");
	LogConfig(*pInLog, InConfig);

	// SwapChainDesc
	ZeroMemory(&_swapChainDesc, sizeof(_swapChainDesc));
	_swapChainDesc.SampleDesc.Count = SAMPLE_COUNT;
	_swapChainDesc.SampleDesc.Quality = SAMPLE_QUALITY;
	_swapChainDesc.BufferDesc.Width = InRTWidth;
	_swapChainDesc.BufferDesc.Height = InRTHeight;
	_swapChainDesc.BufferDesc.RefreshRate.Numerator = DEFAULT_REFRESH_RATE;
	_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	_swapChainDesc.BufferDesc.Format = GetConfig().RenderTarget.ClearColor.GetFormat();
	_swapChainDesc.BufferDesc.ScanlineOrdering = DEFAULT_SCANLINE_ORDER;
	_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_swapChainDesc.BufferCount = 1;
	_swapChainDesc.OutputWindow = hInWnd;
	_swapChainDesc.Windowed = WINDOWED;
	_swapChainDesc.SwapEffect = DEFAULT_SWAP_EFFECT;
	_swapChainDesc.Flags = 0;	

	// Create device
	ID3D11Device*                   pTempDev = nullptr;
	ID3D11DeviceContext*            pTempDevCon = nullptr;
	IDXGISwapChain*                 pTempSwapChain = nullptr;

	D3D_FEATURE_LEVEL             chosenFeatureLevel = static_cast<D3D_FEATURE_LEVEL>(0);
	D3D_FEATURE_LEVEL             featureLevel = DEFAULT_FEATURE_LEVEL;
	const UINT                    deviceFlags = DEFAULT_DEVICE_FLAGS;
	const D3D_DRIVER_TYPE         driverType = D3D_DRIVER_TYPE_HARDWARE;
	const UINT                    sdkVersion = D3D11_SDK_VERSION;

	// Calling D3D11CreateDeviceAndSwapChain
	T_LOG("D3DDevice ctor: D3D11CreateDeviceAndSwapChain...");
	T_LOG(std::left << std::setw(32) << "Feature level:" << GetFeatureLevelString(featureLevel));
	T_LOG(std::left << std::setw(32) << "Device flags:" << GetDeviceFlagsString(deviceFlags));
	T_LOG(std::left << std::setw(32) << "SDK Version: " << sdkVersion);
	if (D3D11_SDK_VERSION == sdkVersion)
	{
		T_LOG("SDK version equals D3D11_SDK_VERSION");
	}
	T_LOG("SwapChain:");
	LogSwapChain(GetLog(), _swapChainDesc);
	_HRCreate = D3D11CreateDeviceAndSwapChain
	(
		nullptr, driverType, NULL, deviceFlags,
		&featureLevel, 1,
		sdkVersion,
		&_swapChainDesc,
		&pTempSwapChain, &pTempDev, &chosenFeatureLevel, &pTempDevCon
	);
	T_LOG("D3DDevice ctor: D3D11CreateDeviceAndSwapChain RETURNED");
	T_LOG(std::left << std::setw(64) << "HR: " << GetDevceHRESULTString(_HRCreate));
	T_LOG(std::left << std::setw(64) << "Chosen feature level: " << GetFeatureLevelString(chosenFeatureLevel));

	// Logging result of the D3D11CreateDeviceAndSwapChain

	// Handling D3D11CreateDeviceAndSwapChain
	_pDev.reset(pTempDev);
	_pDevCon.reset(pTempDevCon);
	_pSwapChain.reset(pTempSwapChain);
	D3DDevice_HandleHR<D3DDeviceException>(this, _HRCreate, "D3D11CreateDeviceAndSwapChain");

	// Create swap chain buffer
	T_LOG("Getting swap chain buffer...");
	ID3D11Texture2D* pTempSwapChainBuffer = nullptr;
	_HRGetBuffer = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pTempSwapChainBuffer);
	T_LOG("HR: " << GetCommonHRESULTString(_HRGetBuffer));
	_pSwapChainBuffer.reset(pTempSwapChainBuffer);
	D3DDevice_HandleHR<D3DDeviceException>(this, _HRGetBuffer, "Getting swap chain buffer");
	T_LOG("Getting swap chain buffer DONE");

	// Create RT view
	T_LOG("Creating render target view for swap chain buffer...");
	ID3D11RenderTargetView* pTempRTView = nullptr;
	_HRCreateRTView = _pDev->CreateRenderTargetView(_pSwapChainBuffer.get(), nullptr, &pTempRTView);
	T_LOG("HR: " << GetCommonHRESULTString(_HRCreateRTView));
	_pSwapChainBufferRTView.reset(pTempRTView);
	D3DDevice_HandleHR<D3DDeviceException>(this, _HRCreateRTView, "CreateRenderTargetView for swap chain buffer");
	T_LOG("Creating render target view for swap chain buffer DONE");

	// Create viewport
	_viewport.MinDepth = 0.0F;
	_viewport.MaxDepth = 1.0F;
	_viewport.TopLeftX = 0.0F;
	_viewport.TopLeftY = 0.0F;
	ResizeViewport(InRTWidth, InRTHeight);

	// Create Depth-stencil
	_UpdateDS();

	InitializeCopyBuffers();

	T_LOG_TO(*pInLog, "D3DDevice: ctor DONE");
}

void D3DDevice::InitializeCopyBuffers()
{
	T_LOG("D3DDevice::InitializeCopyBuffers...");
	const TesterConfig_D3DDevice_CopyBuffers& Cfg = GetConfig().CopyBuffers;

	if (Cfg.bEnableRT)
	{
		T_LOG("Creating RT Copy buffer...");
		_pCopyBuffer_RT = CreateStagingTexture2D_Uninitialized
		(
			GetDev(),
			GetRTWidth(), GetRTHeight(),
			GetConfig().RenderTarget.ClearColor.GetFormat(),
			0, // BindFlags
			D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE
		);
		T_LOG("Creating RT Copy buffer DONE");
	}

	if (Cfg.bEnableDepthStencil)
	{
		T_LOG("Creating Depth-Stencil Copy buffer...");
		_pCopyBuffer_DepthStencil = CreateStagingTexture2D_Uninitialized
		(
			GetDev(),
			GetRTWidth(), GetRTHeight(),
			GetConfig().DepthStencil.Format,
			0, // BindFlags
			D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE
		);
		T_LOG("Creating Depth-Stencil Copy buffer DONE");
	}

	T_LOG("D3DDevice::InitializeCopyBuffers DONE");
}

void D3DDevice::_UpdateDS()
{
	T_LOG("D3DDevice::_UpdateDS...");
	T_LOG("NewWidth = " << GetRTWidth() << "; NewHeight=" << GetRTHeight());
	T_LOG("Format: " << GetFormatString(_config.DepthStencil.Format));
	_DS.Reset(GetDev(), GetRTWidth(), GetRTHeight(), _config.DepthStencil.Format, D3D11_USAGE_DEFAULT, /*CpuAccessFlags*/0);
	T_LOG("D3DDevice::_UpdateDS DONE");
}

void D3DDevice::Notify_MainWindowAspectChanged(unsigned int InNewWidth, unsigned int InNewHeight)
{
	// TODO: Use ResizeBuffers here as described in the DX11 manual:
	// - Release view;
	// - Call ResizeBuffers;
	// - Create and set a new view;

	ResizeViewport(InNewWidth, InNewHeight);
}

void D3DDevice::ResizeViewport(unsigned int InWidth, unsigned int InHeight)
{
	T_LOG("D3DDevice: resizing viewport....");
	_viewport.Width = static_cast<float>(InWidth);
	_viewport.Height = static_cast<float>(InHeight);
	T_LOG(std::setw(32) << "Width: " << InWidth);
	T_LOG(std::setw(32) << "Height: " << InHeight);
	T_LOG("D3DDevice: resizing viewport RETURNS");
}

unsigned int D3DDevice::GetRTWidth() const
{
	D3D11_TEXTURE2D_DESC desc;
	_pSwapChainBuffer->GetDesc(&desc);
	return desc.Width;
}

unsigned int D3DDevice::GetRTHeight() const
{
	D3D11_TEXTURE2D_DESC desc;
	_pSwapChainBuffer->GetDesc(&desc);
	return desc.Height;
}

void D3DDevice::Tick(float InDeltaSecs)
{
	// Empty now
}


void D3DDevice::BeginFrame(int InLocalFrameIndex, std::ofstream& InLog)
{
	T_LOG_TO(InLog, "-- D3DDevice: FRAME " << InLocalFrameIndex << " (global index: " << FrameRender.GlobalFrameIndex << ")");
	FrameRender.BeginFrame(InLocalFrameIndex);
	Clear(InLog, this);	
}

void D3DDevice::EndFrame(int InLocalFrameIndex, std::ofstream& InLog)
{
	T_LOG_TO(InLog, "D3DDevice: EndFrame " <<  InLocalFrameIndex);

	T_LOG_TO(InLog, "D3DDevice: Presenting frame (IDXGISwapChain::Present)");
	GetSwapChain()->Present(0, 0);

	FrameRender.EndFrame(InLocalFrameIndex);

	T_LOG_TO(InLog, "-- D3DDevice: END OF FRAME DONE" << InLocalFrameIndex);
}

void D3DDevice::SFrameRenderState::BeginFrame(int InLocalFrameIndex)
{
	BOOST_ASSERT_MSG( ! bFrameStarted, "D3DDevice::SFrameRenderState::BeginFrame: frame is already started" );
	bFrameStarted = true;
	LocalFrameIndex = InLocalFrameIndex;
}

void D3DDevice::SFrameRenderState::EndFrame(int InLocalFrameIndex)
{
	BOOST_ASSERT_MSG(bFrameStarted, "D3DDevice::SFrameRenderState::EndFrame: frame is NOT started");
	BOOST_ASSERT_MSG(LocalFrameIndex == InLocalFrameIndex, "D3DDevice::SFrameRenderState::EndFrame: inconsistent local frame index");
	GlobalFrameIndex++;
	LocalFrameIndex = -1;
}


template<class ExType>
void D3DDevice_HandleHR(D3DDevice* pDevice, HRESULT InHr, const std::string& InMessage)
{
	std::string FailMessage = InMessage + ": HR FAILED";
	if (FAILED(InHr))
	{
		throw ExType(InHr, FailMessage);
	}
}

inline std::string GetFeatureLevelString(D3D_FEATURE_LEVEL featureLevel)
{
	switch (featureLevel)
	{
	case D3D_FEATURE_LEVEL_11_0:
		return "{11_0}";

	case D3D_FEATURE_LEVEL_11_1:
		return "{11_1}";

	case D3D_FEATURE_LEVEL_10_0:
		return "{10_0}";

	case D3D_FEATURE_LEVEL_10_1:
		return "{10_1}";

	default:
		break;
	}
	return "{UNKNOWN FEATURE LEVEL}";
}

inline std::string GetDeviceFlagsString(UINT InDeviceFlags)
{
	std::string result_string;
	auto numSetFlags = 0;
	for (auto flagIndex = 0; flagIndex < 32; flagIndex++)
	{
		auto flagValue = InDeviceFlags & (1 << flagIndex);
		if (flagValue)
		{
			if (numSetFlags > 0)
			{
				result_string.append(" | ");
			}
			result_string.append(GetSingleDeviceFlagString(flagValue));
			numSetFlags++;
		}
	}
	return result_string;
}

inline std::string GetSingleDeviceFlagString(UINT InDeviceFlag)
{
	switch (InDeviceFlag)
	{
	case D3D11_CREATE_DEVICE_SINGLETHREADED:
		return "{SINGLE_THREADED}";

	case D3D11_CREATE_DEVICE_DEBUG:
		return "{DEBUG}";

	case D3D11_CREATE_DEVICE_SWITCH_TO_REF:
		return "{SWITCH_TO_REF}";

	case D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS:
		return "{PREVENT_INTERNAL_THREADING_OPTIMIZATIONS}";

	case D3D11_CREATE_DEVICE_BGRA_SUPPORT:
		return "{BGRA_SUPPORT}";

	case D3D11_CREATE_DEVICE_DEBUGGABLE:
		return "{DEBUGGABLE}";

	case D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY:
		return "{PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY}";

	case D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT:
		return "{DISABLE_GPU_TIMEOUT}";

	case D3D11_CREATE_DEVICE_VIDEO_SUPPORT:
		return "{VIDEO_SUPPORT}";

	default:
		break;
	}
	return "{UNKNOWN_FLAG}";
}

inline std::string GetSwapEffectString(DXGI_SWAP_EFFECT InSwapEffect)
{
	switch (InSwapEffect)
	{
	case DXGI_SWAP_EFFECT_DISCARD:
		return "{DXGI_SWAP_EFFECT_DISCARD}";

	case DXGI_SWAP_EFFECT_SEQUENTIAL:
		return "{DXGI_SWAP_EFFECT_SEQUENTIAL}";

	case DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL:
		return "{DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL}";

	case DXGI_SWAP_EFFECT_FLIP_DISCARD:
		return "{DXGI_SWAP_EFFECT_FLIP_DISCARD";

	default:
		break;
	}

	return "{UNKNOWN}";
}

} // Test::IMPL