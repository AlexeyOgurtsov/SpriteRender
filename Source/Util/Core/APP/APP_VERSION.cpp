#include "APP_VERSION.h"
#include "../BYTES/Bytes_Hash.h"

namespace Dv::Core
{
	bool TryParseAppVersionPart(AppVersionPart* pOutResult, const std::string& InString, size_t* pOutParsedLength)
	{
		try
		{
			size_t HighLength;
			unsigned long high = std::stoul(InString, &HighLength);
			if(high > std::numeric_limits<uint16_t>::max()) { return false; }
			pOutResult->High = static_cast<uint16_t>(high);
			size_t LowLength;
			unsigned int low = std::stoi(InString.substr(HighLength+1), &LowLength); // +1, because we skip the point;
			if(low > std::numeric_limits<uint16_t>::max()) { return false; }
			pOutResult->Low = static_cast<uint16_t>(low);
			if(pOutParsedLength != nullptr)
			{
				*pOutParsedLength = LowLength + HighLength + 1;
			}
			return true;
		}
		catch(const std::out_of_range&)
		{
			return false;
		}
		catch(const std::invalid_argument&)
		{
			return false;
		}
	}	

	bool TryParseAppVersion(AppVersion* pOutResult, const std::string& InString, size_t* pOutParsedLength)
	{
		size_t HighLength;
		if( ! TryParseAppVersionPart(&pOutResult->High, InString, &HighLength)) { return false; }
		size_t LowLength;
		if( ! TryParseAppVersionPart(&pOutResult->Low, InString.substr(HighLength+1), &LowLength) ) { return false; }
		if(pOutParsedLength != nullptr)
		{
			*pOutParsedLength = HighLength + LowLength + 1;
		}
		return true;
	}

	void AppendAppVersionPartToString(std::string* pOutString, const AppVersionPart& InValue)
	{
		pOutString->append(std::to_string(InValue.High));
		pOutString->append(".");
		pOutString->append(std::to_string(InValue.Low));
	}

	void AppendAppVersionToString(std::string* pOutString, const AppVersion& InValue)
	{
		AppendAppVersionPartToString(pOutString, InValue.High);
		pOutString->append(".");
		AppendAppVersionPartToString(pOutString, InValue.Low);
	}
} // Dv::Core

namespace std
{
	size_t hash<Dv::Core::AppVersionPart>::operator() (const Dv::Core::AppVersionPart& InValue) const
	{
		return Dv::Core::Calc_RotatingHash(&InValue, sizeof(InValue));
	}

	size_t hash<Dv::Core::AppVersion>::operator() (const Dv::Core::AppVersion& InValue) const
	{
		return Dv::Core::Calc_RotatingHash(&InValue, sizeof(InValue));
	}
} // Dv::MicroEngine::Core