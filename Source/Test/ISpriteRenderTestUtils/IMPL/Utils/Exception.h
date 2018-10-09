#pragma once

#include <Windows.h> // HRESULT
#include <string>

namespace Test::IMPL
{
	class Exception
	{
	public:
		Exception(const std::string& InMessage);
	
		const std::string&               GetMessage         () const          { return _message; }

	private:
		std::string                      _message;
	};

	class HRException : public Exception
	{
	public:
		HRException(HRESULT InHR, const std::string& InMessage);

		HRESULT                          GetHR              () const          { return _hr; }

	private:
		std::string                      _message;
		HRESULT                          _hr;
	};
} // Test::IMPL