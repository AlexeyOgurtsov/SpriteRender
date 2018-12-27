#pragma once

#include <string>

class Exception
{
public:
	Exception(const std::string& InMsg) : msg{InMsg} {}
	
	const std::string& GetMsg() const { return msg; }

private:
	std::string msg;
};