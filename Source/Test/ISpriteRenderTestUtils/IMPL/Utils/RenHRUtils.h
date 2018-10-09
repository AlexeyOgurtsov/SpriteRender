#pragma once

#include <d3d11.h>
#include <string_view>
#include <string>

namespace Test::IMPL
{
	std::string GetBufferUAVFlagsString(UINT Flags);
	// Full hresult string: code + description
	std::string GetCommonHRESULTString(HRESULT hr);
	std::string GetDevceHRESULTString(HRESULT hr);
	std::string GetFormattedHRESULTString(HRESULT hr, std::string_view hrDescr);
	std::string GetCreateDeviceHRESULTDescription(HRESULT hr);
	std::string GetCommonHRESULTDescription(HRESULT hr);
	std::string GetFormatString(DXGI_FORMAT InFormat);
	std::string GetAddrModeString(D3D11_TEXTURE_ADDRESS_MODE InAddrMode);
	std::string GetCPUAccessFlagsString(UINT InFlags);
	std::string GetBindFlagsString(UINT InFlags);
	std::string GetUsageFlags(D3D11_USAGE InUsage);
	std::string GetPrimTopologyString(D3D11_PRIMITIVE_TOPOLOGY InPrimTopology);
} // Test::IMPL