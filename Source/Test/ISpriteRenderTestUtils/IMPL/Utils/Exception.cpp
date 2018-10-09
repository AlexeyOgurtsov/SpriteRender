#include "Exception.h"
#include "RenHRUtils.h"

namespace Test::IMPL
{
	HRException::HRException(HRESULT InHR, const std::string& InMessage) :
		_hr(InHR), Exception(std::string("HR: ") + GetFormattedHRESULTString(InHR, GetCommonHRESULTDescription(InHR)) + std::string("\n") + InMessage)
	{
	}

	Exception::Exception(const std::string& InMessage) :
		_message(InMessage)
	{
	}
} // Test::IMPL
