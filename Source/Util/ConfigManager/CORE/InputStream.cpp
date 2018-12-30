#include "InputStream.h"
#include <boost/assert.hpp>
#include <string> // stof
#include <fstream>

namespace Dv::ConfigManager
{
	InputStream::InputStream(const char* pInData, int InLength) :
		_pData(pInData), _length(InLength)
	{
		BOOST_ASSERT(_pData);
	}

	bool InputStream::IsEOF() const
	{
		return _loc.OffsetFromStart >= _length;
	}

	ELexemParseCode InputStream::Peek(char *pOutChar) const
	{
		if (IsEOF())
		{
			return ELexemParseCode::Eof;
		}
		*pOutChar = *GetCurrPtr();
		return ELexemParseCode::Success;
	}

	void InputStream::SeekBack(const StreamLocation& InLoc)
	{
		BOOST_ASSERT(InLoc.OffsetFromStart <= _length);
		_loc = InLoc;
	}

	char InputStream::Peek() const
	{
		BOOST_ASSERT_MSG(false == IsEOF(), "InputStream::Peek: may not BE called when EOF is reached");
		return *GetCurrPtr();
	}

	ELexemParseCode SkipToEndOfLine(InputStream* pStream)
	{
		while (true)
		{
			if (pStream->IsEOF())
			{
				return ELexemParseCode::Eof;
			}

			char c;
			pStream->Read(&c, false);
			if (c == '\n')
			{
				return ELexemParseCode::Success;
			}
		}
	}

	ELexemParseCode InputStream::Read(char *pOutChar, bool bWhitespaceIsEnd)
	{
		if (IsEOF())
		{
			return ELexemParseCode::Eof;
		}

		if (IsWhitespace(Peek()))
		{
			if (bWhitespaceIsEnd)
			{
				return ELexemParseCode::NonLexemChar;
			}
		}
		*pOutChar = *GetCurrPtr();
		Skip();
		return ELexemParseCode::Success;
	}

	ELexemParseCode InputStream::Skip()
	{
		if (IsEOF())
		{
			return ELexemParseCode::Eof;
		}

		AdvanceStreamLocation(&_loc, *GetCurrPtr() == '\n');
		return ELexemParseCode::Success;
	}

	ELexemParseCode InputStream::Skip(int NumCharsToSkip)
	{
		ELexemParseCode result;
		int i = 0;
		while (true)
		{
			if (i >= NumCharsToSkip)
			{
				break;
			}
			result = Skip();
			if (result != ELexemParseCode::Success)
			{
				return result;
			}
			i++;
		}
		return result;
	}

	void InputStream::StartLexem()
	{
		while (true)
		{
			if (IsEOF())
			{
				return;
			}
			if (false == IsWhitespace(Peek()))
			{
				return;
			}
			Skip();
		}
	}

	ELexemParseCode ParseFloat(InputStream* pStream, float *pOutFloat, StreamLocation* pOutErrorLoc)
	{
		*pOutErrorLoc = pStream->GetLoc();
		ELexemParseCode isFloatRes = IsFloat(pStream->GetCurrPtr(), pStream->CountRestToRead());
		if (isFloatRes != ELexemParseCode::Success)
		{
			return isFloatRes;
		}
		size_t numParsed = 0;
		*pOutFloat = std::stof(pStream->GetCurrPtr(), &numParsed);
		pStream->Skip(numParsed);
		return ELexemParseCode::Success;
	}

	ELexemParseCode ParseQuotedString(InputStream* pStream, std::string_view* pOutIdentifier, StreamLocation* pOutErrorLoc)
	{
		*pOutErrorLoc = pStream->GetLoc();

		auto skipQuoteResult = SkipChar('\"', pStream);
		if (skipQuoteResult != ELexemParseCode::Success)
		{
			return skipQuoteResult;
		}
		skipQuoteResult = ParseUntil('\"', pStream, pOutIdentifier);
		pStream->Skip();
		return skipQuoteResult;
	}

	ELexemParseCode SkipUntil(char InChar, InputStream* pStream)
	{
		std::string_view lexem;
		return ParseUntil(InChar, pStream, &lexem);
	}

	ELexemParseCode ParseUntil(char InChar, InputStream* pStream, std::string_view* pOutLexem)
	{
		const char* pLexemStart = pStream->GetCurrPtr();
		while (true)
		{
			*pOutLexem = GetLexemStringView(pStream, pLexemStart);

			if (pStream->IsEOF())
			{
				return ELexemParseCode::Eof;
			}

			char c = pStream->Peek();
			if (c == InChar)
			{
				return ELexemParseCode::Success;
			}
			pStream->Skip();
		}
	}

	ELexemParseCode ParseIdentifier(InputStream* pStream, std::string_view* pOutIdentifier, StreamLocation* pOutErrorLoc)
	{
		*pOutErrorLoc = pStream->GetLoc();

		char c;
		if (pStream->Peek(&c) == ELexemParseCode::Eof)
		{
			return ELexemParseCode::Eof;
		}

		if (false == IsIdentifierStartChar(c))
		{
			return ELexemParseCode::NonLexemChar;
		}

		const char* pLexemStart = pStream->GetCurrPtr();
		while (true)
		{
			*pOutIdentifier = GetLexemStringView(pStream, pLexemStart);
			if (pStream->IsEOF())
			{
				return ELexemParseCode::Eof;
			}
			if (false == IsIdentifierChar(pStream->Peek()))
			{
				// We have found a non-lexem character; 
				// That's means it's the end of the the lexem.
				return ELexemParseCode::Success;
			}
			pStream->Skip();
		}
	}

	ELexemParseCode SkipChar(char InChar, InputStream* pStream)
	{
		if(pStream->IsEOF()) 
		{
			return ELexemParseCode::Eof;
		}
		if (pStream->Peek() != InChar)
		{
			return ELexemParseCode::NonLexemChar;
		}
		pStream->Skip();
		return ELexemParseCode::Success;
	}

	ELexemParseCode SkipPrefix(std::string_view InPrefix, InputStream* pStream)
	{
		int numCharsSeen = 0;
		while (true)
		{
			if (numCharsSeen >= InPrefix.length())
			{
				return ELexemParseCode::Success;
			}
			if (pStream->IsEOF())
			{
				return ELexemParseCode::Eof;
			}
			
			if(pStream->Peek() != InPrefix[numCharsSeen])
			{
				return ELexemParseCode::NonLexemChar;
			}
			pStream->Skip();
			numCharsSeen++;
		}
	}

	ELexemParseCode ParseInt(InputStream* pStream, int *pOutInt, StreamLocation* pOutErrorLoc)
	{
		*pOutErrorLoc = pStream->GetLoc();

		const auto locAtStart = pStream->GetLoc();
		bool bMinus = ParseOptionalNeg(pStream);
		if (false == isdigit(pStream->Peek()))
		{
			return ELexemParseCode::NonLexemChar;
		}
		*pOutInt = 0;
		while (true)
		{
			char c;
			const auto locBeforeRead = pStream->GetLoc();;
			auto readResult = pStream->Read(&c);
			bool bNotLexemChar = (false == IsDecDigit(c));
			bool bEndOfLexem =
				readResult == ELexemParseCode::Eof ||
				readResult == ELexemParseCode::NonLexemChar ||
				bNotLexemChar;
			if (bEndOfLexem)
			{
				pStream->SeekBack(locBeforeRead);
				if (bMinus)
				{
					*pOutInt = -*pOutInt;
				}
				return ELexemParseCode::Success;
			}
			int charValue = GetDigitValue(c);
			*pOutInt *= 10;
			*pOutInt += charValue;
		}
	}

	ELexemParseCode ParseHex(InputStream* pStream, int* pOutInt, StreamLocation* pOutErrorLoc)
	{
		*pOutErrorLoc = pStream->GetLoc();

		bool bMinus = ParseOptionalNeg(pStream);
		if (SkipPrefix("0x", pStream) == ELexemParseCode::Eof)
		{
			return ELexemParseCode::Eof;
		}
		*pOutInt = 0;
		while (true)
		{
			char c;
			auto streamLoc = pStream->GetLoc();
			auto readResult = pStream->Read(&c);
			bool bNotLexemChar = (false == IsHexDigit(c));
			bool bEndOfLexem =
				readResult == ELexemParseCode::Eof ||
				readResult == ELexemParseCode::NonLexemChar ||
				bNotLexemChar;
			if (bEndOfLexem)
			{
				if (bNotLexemChar)
				{
					pStream->SeekBack(streamLoc);
				}
				if (bMinus)
				{
					*pOutInt = -*pOutInt;
				}
				return ELexemParseCode::Success;
			}
			int charValue = GetHexValue(c);
			*pOutInt <<= 4;
			*pOutInt += charValue;
		}
	}

	ELexemParseCode ParseOctal(InputStream* pStream, int* pOutInt, StreamLocation* pOutErrorLoc)
	{
		*pOutErrorLoc = pStream->GetLoc();

		bool bMinus = ParseOptionalNeg(pStream);

		if (pStream->IsEOF())
		{
			return ELexemParseCode::Eof;
		}

		if (pStream->Peek() != '\0')
		{
			return ELexemParseCode::NonLexemChar;
		}

		pStream->Skip();
		*pOutInt = 0;
		while (true)
		{
			char c;
			auto streamLoc = pStream->GetLoc();
			auto readResult = pStream->Read(&c);
			bool bNotLexemChar = (false == IsOctalDigit(c));
			bool bEndOfLexem = 
				readResult == ELexemParseCode::Eof || 
				readResult == ELexemParseCode::NonLexemChar ||
				bNotLexemChar;
			if (bEndOfLexem)
			{
				if (bNotLexemChar)
				{
					pStream->SeekBack(streamLoc);
				}
				if (bMinus)
				{
					*pOutInt = -*pOutInt;
				}
				return ELexemParseCode::Success;
			}
			int charValue = GetDigitValue(c);
			*pOutInt <<= 3;
			*pOutInt += charValue;
		}
	}

	bool ParseOptionalNeg(InputStream* pStream)
	{
		return ParseOptionalChar('-', pStream);
	}

	bool ParseOptionalChar(char InChar, InputStream* pStream)
	{
		if (pStream->IsEOF())
		{
			return false;
		}

		if (pStream->Peek() == InChar)
		{
			pStream->Skip();
			return true;
		}

		return false;
	}

	bool ParseOptionalString(std::string_view InString, InputStream* pStream)
	{
		int numCharsSeen = 0;
		auto streamLoc = pStream->GetLoc();
		while (true)
		{
			if (numCharsSeen >= InString.length())
			{
				return true;
			}
			if (pStream->IsEOF() || pStream->Peek() != InString[numCharsSeen])
			{
				pStream->SeekBack(streamLoc);
				return false;
			}
			pStream->Skip();
			numCharsSeen++;
		}
		return true;
	}

	std::string_view GetLexemStringView(InputStream* pStream, const char* pLexemStart)
	{
		return std::string_view(pLexemStart, pStream->GetCurrPtr() - pLexemStart);
	}

	bool IsIdentifierStartChar(char InChar)
	{
		return (InChar == '_') || isalpha(InChar);
	}

	bool IsIdentifierChar(char InChar)
	{
		return IsDecDigit(InChar) || IsIdentifierStartChar(InChar);
	}

	bool IsOctalDigit(char InChar)
	{
		return InChar >= '0' && InChar <= '7';
	}

	bool IsWhitespace(char InChar)
	{
		if (InChar == '\t')
		{
			return true;
		}
		if (InChar == ' ')
		{
			return true;
		}
		if (InChar == '\r')
		{
			return true;
		}
		if (InChar == '\n')
		{
			return true;
		}
		return false;
	}

	int GetDigitValue(char InChar)
	{
		return InChar - '0';
	}

	bool IsDecDigit(char InChar)
	{
		return InChar >= '0' && InChar <= '9';
	}

	bool IsHexDigit(char InChar)
	{
		if (IsDecDigit(InChar))
		{
			return true;
		}
		char upperChar = toupper(InChar);
		return upperChar >= 'A' && upperChar <= 'F';
	}

	ELexemParseCode IsFloat(const char* pString, int InLength)
	{
		int i = 0;
		int positiveStartIndex = 0;
		if (InLength == 0) { return ELexemParseCode::Eof; }
		if (*pString == '-') 
		{
			i++;
			positiveStartIndex = i;
		}
		// Search for the point (terminates on the point's position)
		ELexemParseCode findPointResult = FindPointFromPosition(&i, pString, InLength);
		if (findPointResult != ELexemParseCode::Success)
		{
			return findPointResult;
		}

		BOOST_ASSERT(pString[i] == '.');
		// If out digit prefix length is not empty, 
		// then it must be a correct digit sequence, otherwise it's not a float.
		int digitPrefixLength = (i - positiveStartIndex);
		if (digitPrefixLength > 0)
		{
			ELexemParseCode digitsBeforeResult = IsDigitSequence(pString + positiveStartIndex, digitPrefixLength, MAX_FLOAT_DIGITS_BEFORE_POINT);
			return digitsBeforeResult;
		}
	
		// Skip the point;
		i++;
		int restOfTheStringLength = InLength - i;
		// If our digit prefix is empty, we must check digits after the point
		ELexemParseCode digitsAfterResult = IsDigitSequence(pString + i, restOfTheStringLength, MAX_FLOAT_DIGITS_AFTER_POINT);
		return digitsAfterResult;
	}

	ELexemParseCode IsDigitSequence(const char* pStringStart, int InLength, int InMaxLength)
	{
		int i = 0;
		while (true)
		{
			if (i >= InLength)
			{
				return ELexemParseCode::Success;
			}
			if (i >= InMaxLength)
			{
				return ELexemParseCode::LexemTooBig;
			}
			if (false == IsDecDigit(pStringStart[i]))
			{
				return ELexemParseCode::NonLexemChar;
			}
			i++;
		}
		return ELexemParseCode::Success;
	}

	ELexemParseCode FindPointFromPosition(int *pInOutIndex, const char* pString, int InLength)
	{
		while (true)
		{
			if (*pInOutIndex >= InLength)
			{
				return ELexemParseCode::Eof;
			}
			char c = pString[*pInOutIndex];
			if (c == '.')
			{
				return ELexemParseCode::Success;
			}
			if (false == isdigit(c))
			{
				return ELexemParseCode::NonLexemChar;
			}
			(*pInOutIndex)++;
		}
	}

	int GetHexValue(char InChar)
	{
		if (InChar <= '9')
		{
			return GetDigitValue(InChar);
		}
		else
		{
			char upperChar = toupper(InChar);
			return upperChar - 'A';
		}
	}

	bool TryOpenInputStream(InputStream* pOutStream, std::string_view InFilename, std::string* pOutText)
	{
		std::ifstream fstrm { InFilename.data(), std::ios::in };
		if (false == fstrm.is_open()) { return false; }
		pOutText->reserve(1024 * 100);
		while (true)
		{
			char c;
			fstrm.read(&c, 1);
			if (fstrm.eof()) { break; }
			pOutText->push_back(c);
		}
		*pOutStream = InputStream( pOutText->c_str(), pOutText->length() );
		return true;
	}
} // Dv::ConfigManager