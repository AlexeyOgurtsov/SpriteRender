#pragma once

#include <iomanip>
#include <string>

namespace Dv::ConfigManager
{
	enum class ELexemParseCode
	{
		Success,
		Eof,
		NonLexemChar,
		LexemTooBig
	};

	inline std::string GetLexemParseCodeString(ELexemParseCode InCode)
	{
		switch (InCode)
		{
		case ELexemParseCode::Success:
			return "<<Success>>";

		case ELexemParseCode::Eof:
			return "<<End of file>>";

		case ELexemParseCode::NonLexemChar:
			return "<<Non-lexem character found>>";

		case ELexemParseCode::LexemTooBig:
			return "<<Lexem is too big>>";

		default:
			break;
		}
		return "<<Unknown lexem parse code>>";
	}

	template<class Strm>
	Strm& operator<<(Strm& strm, ELexemParseCode InCode)
	{
		strm << GetLexemParseCodeString(InCode) << std::endl;
		return strm;
	}

	enum class ELexemType
	{
		Float,
		Int,
		Identifier
	};
} // Dv::ConfigManager