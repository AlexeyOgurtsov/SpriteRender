#include "RenHRUtils.h"

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
	std::string GetBufferUAVFlagsString(UINT Flags)
	{
		std::string result;

		if (Flags & D3D11_BUFFER_UAV_FLAG_RAW)
		{
			if (result.size() != 0) { result.append("; "); }
			result.append("UAV_FLAG_RAW");
		}

		if (Flags & D3D11_BUFFER_UAV_FLAG_APPEND)
		{
			if (result.size() != 0) { result.append("; "); }
			result.append("UAV_FLAG_APPEND");
		}

		if (Flags & D3D11_BUFFER_UAV_FLAG_COUNTER)
		{
			if (result.size() != 0) { result.append("; "); }
			result.append("UAV_FLAG_COUNTER");
		}

		return result;
	}

	std::string GetCommonHRESULTString(HRESULT hr)
	{
		return GetFormattedHRESULTString(hr, GetCommonHRESULTDescription(hr));
	}


	std::string GetFormattedHRESULTString(HRESULT hr, std::string_view hrDescr)
	{
		std::string failOrSuccessDescr = SUCCEEDED(hr) ? "SUCC" : "FAILED";
		return std::to_string(hr) + std::string("(") +
			failOrSuccessDescr + std::string(":") +
			std::string(hrDescr) + std::string(")");
	}

	std::string GetCommonHRESULTDescription(HRESULT hr)
	{
		switch (hr)
		{
		case E_OUTOFMEMORY:
			return "{E_OUTOFMEMORY}";

		case E_INVALIDARG:
			return "{E_INVALIDARG}";

		case E_NOTIMPL:
			return "{E_NOTIMPL}";

		case E_FAIL:
			return "{E_FAIL}";

		case DXGI_ERROR_DEVICE_HUNG:
			return "{DXGI_ERROR_DEVICE_HUNG}";

		case DXGI_ERROR_DEVICE_REMOVED:
			return "{DXGI_ERROR_DEVICE_REMOVED}";

		case DXGI_ERROR_DEVICE_RESET:
			return "{DXGI_ERROR_DEVICE_RESET}";

		case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
			return "{DXGI_ERROR_DRIVER_INTERNAL_ERROR}";

		case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:
			return "{DXGI_ERROR_FRAME_STATISTICS_DISJOINT}";

		case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE:
			return "{DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE}";

		case DXGI_ERROR_INVALID_CALL:
			return "{DXGI_ERROR_INVALID_CALL}";

		case DXGI_ERROR_MORE_DATA:
			return "{DXGI_ERROR_MORE_DATA}";

		case DXGI_ERROR_NONEXCLUSIVE:
			return "{DXGI_ERROR_NONEXCLUSIVE}";

		case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:
			return "{DXGI_ERROR_NOT_CURRENTLY_AVAILABLE}";

		case DXGI_ERROR_NOT_FOUND:
			return "{DXGI_ERROR_NOT_FOUND}";

		case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED:
			return "{DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED}";

		case DXGI_ERROR_REMOTE_OUTOFMEMORY:
			return "{DXGI_ERROR_REMOTE_OUTOFMEMORY}";

		case DXGI_ERROR_WAS_STILL_DRAWING:
			return "{DXGI_ERROR_WAS_STILL_DRAWING}";

		case DXGI_ERROR_UNSUPPORTED:
			return "{DXGI_ERROR_UNSUPPORTED}";

		case DXGI_ERROR_ACCESS_LOST:
			return "{DXGI_ERROR_ACCESS_LOST}";

		case DXGI_ERROR_WAIT_TIMEOUT:
			return "{DXGI_ERROR_WAIT_TIMEOUT}";

		case DXGI_ERROR_SESSION_DISCONNECTED:
			return "{DXGI_ERROR_SESSION_DISCONNECTED}";

		case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE:
			return "{DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE}";

		case DXGI_ERROR_CANNOT_PROTECT_CONTENT:
			return "{DXGI_ERROR_CANNOT_PROTECT_CONTENT}";

		case DXGI_ERROR_ACCESS_DENIED:
			return "{DXGI_ERROR_ACCESS_DENIED}";

		case DXGI_ERROR_NAME_ALREADY_EXISTS:
			return "{DXGI_ERROR_NAME_ALREADY_EXISTS}";

		case DXGI_ERROR_SDK_COMPONENT_MISSING:
			return "{DXGI_ERROR_SDK_COMPONENT_MISSING}";

		case S_OK:
			return "{S_OK}";

		case S_FALSE:
			return "{S_FALSE}";

		default:
			break;
		}
		return "{UNKNOWN}";
	}

	std::string GetCreateDeviceHRESULTDescription(HRESULT hr)
	{
		return GetCommonHRESULTDescription(hr);
	}
	std::string GetDevceHRESULTString(HRESULT hr)
	{
		return GetFormattedHRESULTString(hr, GetCreateDeviceHRESULTDescription(hr));
	}

	std::string GetPrimTopologyString(D3D11_PRIMITIVE_TOPOLOGY InPrimTopology)
	{
		switch (InPrimTopology)
		{
		case D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED:
			return "UNDEFINED";

		case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
			return "TRIANGLESTRIP";

		case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
			return "TRIANGLELIST";

		case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST:
			return "POINTLIST";

		case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
			return "LINELIST";

		case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
			return "LINESTRIP";

		default:
			break;
		}
		return "UNKNOWN";
	}

	std::string GetUsageFlags(D3D11_USAGE InUsage)
	{
		switch (InUsage)
		{
		case D3D11_USAGE_DEFAULT:
			return "DEFAULT";

		case D3D11_USAGE_DYNAMIC:
			return "DYNAMIC";

		case D3D11_USAGE_IMMUTABLE:
			return "IMMUTABLE";

		case D3D11_USAGE_STAGING:
			return "STAGING";

		default:
			break;
		}
		return "UNKNOWN";
	}

	std::string GetCPUAccessFlagsString(UINT InFlags)
	{
		std::string result;
		if (InFlags & D3D11_CPU_ACCESS_READ)
		{
			if (false == result.empty()) { result.append(" | "); }
			result.append("CPU_READ");
		}

		if (InFlags & D3D11_CPU_ACCESS_WRITE)
		{
			if (false == result.empty()) { result.append(" | "); }
			result.append("CPU_WRITE");
		}
		return result;
	}

	std::string GetBindFlagsString(UINT InFlags)
	{
		std::string result;
		if (InFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			if (false == result.empty()) { result.append(" | "); }
			result.append("DEPTH_STENCIL");
		}

		if (InFlags & D3D11_BIND_STREAM_OUTPUT)
		{
			if (false == result.empty()) { result.append(" | "); }
			result.append("STREAM_OUTPUT");
		}

		if (InFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (false == result.empty()) { result.append(" | "); }
			result.append("UNORDERED_ACCESS");
		}

		if (InFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (false == result.empty()) { result.append(" | "); }
			result.append("RENDER_TARGET");
		}

		if (InFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (false == result.empty()) { result.append(" | "); }
			result.append("SHADER_RESOURCE");
		}

		if (InFlags & D3D11_BIND_INDEX_BUFFER)
		{
			if (false == result.empty()) { result.append(" | "); }
			result.append("INDEX");
		}

		if (InFlags & D3D11_BIND_VERTEX_BUFFER)
		{
			if (false == result.empty()) { result.append(" | "); }
			result.append("VERTEX");
		}
		return result;
	}

	std::string GetAddrModeString(D3D11_TEXTURE_ADDRESS_MODE InAddrMode)
	{
		switch (InAddrMode)
		{
		case D3D11_TEXTURE_ADDRESS_WRAP:
			return "D3D11_TEXTURE_ADDRESS_WRAP";

		case D3D11_TEXTURE_ADDRESS_MIRROR:
			return "D3D11_TEXTURE_ADDRESS_MIRROR";

		case D3D11_TEXTURE_ADDRESS_CLAMP:
			return "D3D11_TEXTURE_ADDRESS_CLAMP";

		case D3D11_TEXTURE_ADDRESS_BORDER:
			return "D3D11_TEXTURE_ADDRESS_BORDER";

		default:
			break;
		}
		return "{UNKNOWN}";
	}

	std::string GetFormatString(DXGI_FORMAT InFormat)
	{
		switch (InFormat)
		{
		case DXGI_FORMAT_UNKNOWN:
			return "{DXGI_FORMAT_UNKNOWN}";

		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
			return "{R32G32B32A32_TYPELESS}";

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return "{R32G32B32A32_FLOAT}";

		case DXGI_FORMAT_R32G32B32A32_UINT:
			return "{R32G32B32A32_UINT}";

		case DXGI_FORMAT_R32G32B32A32_SINT:
			return "{R32G32B32A32_SINT}";

		case DXGI_FORMAT_R32G32B32_TYPELESS:
			return "{R32G32B32_TYPELESS}";

		case DXGI_FORMAT_R32G32B32_FLOAT:
			return "{R32G32B32_FLOAT}";

		case DXGI_FORMAT_R32G32B32_UINT:
			return "{R32G32B32_UINT}";

		case DXGI_FORMAT_R32G32B32_SINT:
			return "{R32G32B32_SINT}";

		case DXGI_FORMAT_R16G16B16A16_TYPELESS:
			return "{R16G16B16A16_TYPELESS}";

		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return "{R16G16B16A16_FLOAT}";

		case DXGI_FORMAT_R16G16B16A16_UNORM:
			return "{R16G16B16A16_UNORM}";

		case DXGI_FORMAT_R16G16B16A16_UINT:
			return "{R16G16B16A16_UINT}";

		case DXGI_FORMAT_R16G16B16A16_SNORM:
			return "{R16G16B16A16_SNORM}";

		case DXGI_FORMAT_R16G16B16A16_SINT:
			return "{R16G16B16A16_SINT}";

		case DXGI_FORMAT_R32G32_TYPELESS:
			return "{R32G32_TYPELESS}";

		case DXGI_FORMAT_R32G32_FLOAT:
			return "{R32G32_FLOAT}";

		case DXGI_FORMAT_R32G32_UINT:
			return "{R32G32_UINT}";

		case DXGI_FORMAT_R32G32_SINT:
			return "{R32G32_SINT}";

		case DXGI_FORMAT_R32_TYPELESS:
			return "{R32_TYPELESS}";

		case DXGI_FORMAT_R32_FLOAT:
			return "{R32_FLOAT}";

		case DXGI_FORMAT_R32_UINT:
			return "{R32_UINT}";

		case DXGI_FORMAT_R32_SINT:
			return "{R32_SINT}";

		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
			return "{R8G8B8A8_TYPELESS}";

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return "{R8G8B8A8_UNORM}";

		case DXGI_FORMAT_R8G8B8A8_SNORM:
			return "{R8G8B8A8_SNORM}";

		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return "{R8G8B8A8_UNORM_SRGB}";

		case DXGI_FORMAT_R8G8B8A8_UINT:
			return "{R8G8B8A8_UINT}";

		case DXGI_FORMAT_R8G8B8A8_SINT:
			return "{R8G8B8A8_SINT}";

		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			return "{D32_FLOAT_S8X24_UINT}";

		case DXGI_FORMAT_D32_FLOAT:
			return "{D32_FLOAT}";

		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			return "{D24_UNORM_S8_UINT}";

		case DXGI_FORMAT_D16_UNORM:
			return "{D16_UNORM}";

		default:
			break;
		}

		return "{UNKNOWN}";
	}

} // Dv::Spr::Ren::IMPL::D3D
} // Dv::Spr::Ren::IMPL
} // Dv::Spr::Ren
} // Dv::Spr
} // Spr