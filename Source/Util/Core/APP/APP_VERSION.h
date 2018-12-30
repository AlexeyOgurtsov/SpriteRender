#pragma once

#include <string>
#include <limits>
#include <functional>

namespace Dv::Core
{
	struct AppVersionPart
	{
		uint16_t High;
		uint16_t Low;

		constexpr AppVersionPart() : High(0), Low(0) {};
		constexpr AppVersionPart(uint16_t InHigh, uint16_t InLow) : 
			High(InHigh)
		,	Low(InLow) {}
	};
	bool TryParseAppVersionPart(AppVersionPart* pOutResult, const std::string& InString, size_t* pOutParsedLength = nullptr);
	void AppendAppVersionPartToString(std::string* pOutString, const AppVersionPart& InValue);
	std::string ToString(const AppVersionPart& InValue);
	template<class Strm> Strm& operator<<(Strm& strm, const AppVersionPart& InValue);
	constexpr bool operator==(const AppVersionPart& Left, const AppVersionPart& Right);
	constexpr bool operator!=(const AppVersionPart& Left, const AppVersionPart& Right);
	constexpr bool operator<(const AppVersionPart& Left, const AppVersionPart& Right);
	constexpr bool operator<=(const AppVersionPart& Left, const AppVersionPart& Right);
	constexpr bool operator>(const AppVersionPart& Left, const AppVersionPart& Right);
	constexpr bool operator>=(const AppVersionPart& Left, const AppVersionPart& Right);

	struct AppVersion
	{
		AppVersionPart              High;
		AppVersionPart              Low;

		constexpr AppVersion() {}
		constexpr AppVersion(const AppVersionPart& InHigh, const AppVersionPart& InLow) : 
			High(InHigh)
		,	Low(InLow) {}
		constexpr AppVersion(uint16_t A, uint16_t B, uint16_t C, uint16_t D) :
			High(A, B)
		,	Low(C, D) {}
	};

	bool TryParseAppVersion(AppVersion* pOutResult, const std::string& InString, size_t* pOutParsedLength = nullptr);
	void AppendAppVersionToString(std::string* pOutString, const AppVersion& InValue);
	std::string ToString(const AppVersion& InValue);
	template<class Strm> Strm& operator<<(Strm& strm, const AppVersion& InValue);
	constexpr bool operator==(const AppVersion& Left, const AppVersion& Right);
	constexpr bool operator!=(const AppVersion& Left, const AppVersion& Right);

	inline std::string ToString(const AppVersionPart& InValue)
	{
		std::string result;
		AppendAppVersionPartToString(&result, InValue);
		return result;
	}

	template<class Strm> Strm& operator<<(Strm& strm, const AppVersionPart& InValue)
	{
		return InValue.High << "." << InValue.Low;
	}

	inline std::string ToString(const AppVersion& InValue)
	{
		std::string result;
		AppendAppVersionToString(&result, InValue);
		return result;
	}

	template<class Strm> Strm& operator<<(Strm& strm, const AppVersion& InValue)
	{
		return strm << InValue.High << "." << InValue.Low;
	}


	inline constexpr bool operator!=(const AppVersionPart& Left, const AppVersionPart& Right)
	{
		return !operator==(Left, Right);
	}

	inline constexpr bool operator==(const AppVersionPart& Left, const AppVersionPart& Right)
	{
		return Left.Low == Right.Low && Left.High == Right.High;
	}

	inline constexpr bool operator<(const AppVersionPart& Left, const AppVersionPart& Right)
	{
		return (Left.High < Right.High) || ((Left.High == Right.High) && (Left.Low < Right.Low));
	}

	inline constexpr bool operator<=(const AppVersionPart& Left, const AppVersionPart& Right)
	{
		return (Left == Right) || (Left < Right);
	}

	inline constexpr bool operator>(const AppVersionPart& Left, const AppVersionPart& Right)
	{
		return (Left.High > Right.High) || ((Left.High == Right.High) && (Left.Low > Right.Low));
	}

	inline constexpr bool operator>=(const AppVersionPart& Left, const AppVersionPart& Right)
	{
		return (Left == Right) || (Left > Right);
	}

	inline constexpr bool operator!=(const AppVersion& Left, const AppVersion& Right)
	{
		return !operator==(Left, Right);
	}

	inline constexpr bool operator==(const AppVersion& Left, const AppVersion& Right)
	{
		return Left.Low == Right.Low && Left.High == Right.High;
	}
} // Dv::Core

namespace boost::serialization 
{
	template<class Ar>
	void serialize(Ar& ar, const Dv::Core::AppVersionPart& InValue, unsigned int version)
	{
		ar & High;
		ar & Low;
	}

	template<class Ar>
	void serialize(Ar& ar, const Dv::Core::AppVersion& InValue, unsigned int version)
	{
		ar & High;
		ar & Low;
	}
}

namespace std
{
	template<>
	class hash<Dv::Core::AppVersionPart>
	{
	public:
		size_t operator() (const Dv::Core::AppVersionPart& InValue) const;
	};

	template<>
	class hash<Dv::Core::AppVersion>
	{
	public:
		size_t operator() (const Dv::Core::AppVersion& InValue) const;
	};
}