#pragma once

#include "APP_VERSION.h"

namespace Dv::Core
{
	struct AppInfo
	{
		const char*              Name;
		AppVersion               Version;

		constexpr AppInfo() : Name("") {}
		constexpr AppInfo(const char* pInName, const AppVersion& InVersion) :
			Version(InVersion),
			Name(pInName) {}
	};
	template<class Strm>
	Strm& operator<<(Strm& strm, const AppInfo& InValue);
	void AppendAppInfoToString(std::string* pOutString, const AppInfo& InValue);
	std::string ToString(const AppInfo& InValue);

	/**********************************************************************
	* implementation
	**********************************************************************/
	template<class Strm>
	Strm& operator<<(Strm& strm, const AppInfo& InValue)
	{
		return strm << ToString(InValue);
	}
} // Dv::MicroEngine::Core

namespace boost::serialization
{
	template<class Ar>
	void serialize(Ar& ar, const  Dv::Core::AppInfo& InInfo, unsigned int Version)
	{
		ar & InInfo.Name;
		ar & InInfo.Version;
	}
} // boost::serialization