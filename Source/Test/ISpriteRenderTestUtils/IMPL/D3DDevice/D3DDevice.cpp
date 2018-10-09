#include "D3DDevice.h"
#include "D3DDevice_CONFIG_CONST.h"
#include "D3DDeviceException.h"
#include "ISpriteRenderTestUtils/IMPL/Utils/RenHRUtils.h"
#include "ISpriteRenderTestUtils/Utils/TestUtils.h"
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

D3DDevice::D3DDevice(UINT InRTWidth, UINT InRTHeight, std::ofstream* pInLog, HWND hInWnd, const TesterConfig_D3DDevice& InConfig) :
	_pLog(pInLog)
,	_config(InConfig)
{
	assert(_pLog);
	// SwapChainDesc
	ZeroMemory(&_swapChainDesc, sizeof(_swapChainDesc));
	_swapChainDesc.SampleDesc.Count = SAMPLE_COUNT;
	_swapChainDesc.SampleDesc.Quality = SAMPLE_QUALITY;
	_swapChainDesc.BufferDesc.Width = InRTWidth;
	_swapChainDesc.BufferDesc.Height = InRTHeight;
	_swapChainDesc.BufferDesc.RefreshRate.Numerator = DEFAULT_REFRESH_RATE;
	_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	_swapChainDesc.BufferDesc.Format = DEFAULT_RENDER_TARGET_FORMAT;
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
}

void D3DDevice::_UpdateDS()
{
	T_LOG("D3DDevice::_UpdateDS...");
	T_LOG("NewWidth = " << GetRTWidth() << "; NewHeight=" << GetRTHeight());
	T_LOG("Format: " << GetFormatString(_config.DepthStencilFormat));
	_DS.Reset(GetDev(), GetRTWidth(), GetRTHeight(), _config.DepthStencilFormat, D3D11_USAGE_DEFAULT, /*CpuAccessFlags*/0);
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