#pragma once

#include "ISpriteRenderTestUtils/IMPL/Utils/Exception.h"

namespace Test::IMPL
{
	class D3DDeviceException : public HRException
	{
	public:
		D3DDeviceException(HRESULT InHR, const std::string& InMessage);
	};

	inline D3DDeviceException::D3DDeviceException(HRESULT InHR, const std::string& InMessage) :
		HRException(InHR, InMessage)
	{
	}
} // Test::IMPL
