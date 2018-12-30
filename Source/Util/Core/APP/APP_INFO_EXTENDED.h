#pragma once

#include "APP_INFO.h"
#include <string_view>
#include <boost/serialization/string.hpp>

namespace Dv::Core
{
	struct AppInfoExtended
	{
		AppInfo                 Info;
	
		/**
		* Brief description of the AppInfo
		*/
		std::string             Description;

		AppInfoExtended() : Description("") {}
		AppInfoExtended(const AppInfo& InInfo, std::string_view InDescription) :
			Info(InInfo)
		,	Description(InDescription) {}
	};
	void AppendAppInfoExtendedToString(std::string* pOutString, const AppInfoExtended& InValue);
	std::string ToString(const AppInfoExtended& InValue);
	template<class Strm>
	Strm& operator<<(Strm& strm, const AppInfoExtended& InValue);

	template<class Strm>
	Strm& operator<<(Strm& strm, const AppInfoExtended& InValue)
	{
		strm << ToString(InValue);
		return strm;
	}
} // Dv::Core

namespace boost::serialization
{
	template<class Ar>
	void serialize(Ar& ar, const Dv::Core::AppInfoExtended& InInfo,  unsigned int Version)
	{
		ar & InInfo.Info;
		ar & InInfo.Description;
	}
} // boost::serialization