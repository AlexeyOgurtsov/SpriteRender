#pragma once

#include "SourceLocation.h"
#include "InputStreamLexTypes.h"

namespace Dv::ConfigManager
{
	/****************************************************************************************
	* Input stream:
	* - Reads from memory only;
	* - Counts location in the stream;
	****************************************************************************************/
	class InputStream
	{
	public:
		InputStream() = default;
		InputStream(const char* pInData, int InLength);

		__forceinline const StreamLocation&             GetLoc         () const { return _loc; }
		__forceinline const char*                       GetCurrPtr     () const { return _pData + _loc.OffsetFromStart; }

		__forceinline bool                              IsEOF          () const;

		__forceinline int                               CountRestToRead() const { return _length - _loc.OffsetFromStart; }

		ELexemParseCode                                 Peek           (char *pOutChar) const;
		// Returns current character, checking with assert that EOF is not yet reached.
		char                                            Peek           () const;

		// Seeks back to the given stream location
		void                                            SeekBack       (const StreamLocation& InLoc);

		// Skips current character (May return EOF code)
		ELexemParseCode                                 Skip           ();
		ELexemParseCode                                 Skip           (int NumCharsToSkip);

		// Seeks to the next non-whitespace location
		void                                           StartLexem();

		// Reads lexem character:
		// - Eof, if EOF is found;
		// - NonLexemChar, if non-whitespace character is found (for example, whitespace).
		ELexemParseCode                                 Read           (char *pOutChar, bool bWhitespaceIsEnd = true);

	private:
		int                         _length = 0;
		const char*                 _pData = nullptr;
		StreamLocation              _loc;
	};

	bool TryOpenInputStream(InputStream* pOutStream, std::string_view InFilename, std::string* pOutText);

	// Parse float: 
	// Tries to parse float, but only THAT contains point!!!
	ELexemParseCode               ParseFloat          (InputStream* pStream, float *pOutFloat, StreamLocation* pOutErrorLoc);

	// Parses decimal integer:
	// 0, 35, -4, -0
	ELexemParseCode               ParseInt            (InputStream* pStream, int *pOutInt, StreamLocation* pOutErrorLoc);

	// Parses hexadecimal number:
	// 0xFF, 0xF, 0x0, -0xFF
	ELexemParseCode               ParseHex            (InputStream* pStream, int* pOutInt, StreamLocation* pOutErrorLoc);

	// Parser octal number:
	ELexemParseCode               ParseOctal          (InputStream* pStream, int* pOutInt, StreamLocation* pOutErrorLoc);

	// Parse neg:
	// If there's a negative sign in the stream, reflect it with the bool result, 
	// otherwise nevers touchs the stream location.
	// (Should NEVER return EOF)
	bool                          ParseOptionalNeg    (InputStream* pStream);

	// If there's a given char in the stream, then reflect it with the bool result,
	// otherwise never touches the stream location.
	bool                         ParseOptionalChar   (char InChar, InputStream* pStream);

	// If there's a given char, reflect with with the true bool result,
	// otherwise never touches the stream location.
	bool                         ParseOptionalString (std::string_view InString, InputStream* pStream);

	// Skips the current character, if it's equal to the given.
	// Otherwise, returns an error code.
	ELexemParseCode              SkipChar(char InChar, InputStream* pStream);
	ELexemParseCode              SkipPrefix(std::string_view InPrefix, InputStream* pStream);

	// Parser identifier:
	// _A, _123, _A123, Ab etc.
	ELexemParseCode           ParseIdentifier     (InputStream* pStream, std::string_view* pOutIdentifier, StreamLocation* pOutErrorLoc);

	// Parses until the given character is found.
	// The given character is NOT included in the resulting lexem and remains in the stream.
	// May return EOF;
	ELexemParseCode           ParseUntil          (char InChar, InputStream* pStream, std::string_view* pOutLexem);

	// Parse quoted string:
	// "test", "Test string", etc.
	// Always parsed WITHOUT brackets
	ELexemParseCode           ParseQuotedString   (InputStream* pStream, std::string_view* pOutIdentifier, StreamLocation* pOutErrorLoc);

	bool IsIdentifierStartChar(char InChar);
	bool IsIdentifierChar(char InChar);
	bool IsDecDigit(char InChar);
	bool IsOctalDigit(char InChar);
	bool IsHexDigit(char InChar);
	int GetDigitValue(char InChar);
	int GetHexValue(char InChar);
	bool IsWhitespace(char InChar);
	ELexemParseCode IsDigitSequence(const char* pStringStart, int InLength, int InMaxLength);
	// Checks that the given number is float:
	// - only point-containing values are considered float: 
	// .5, 1, 3;
	// - minus is supported
	constexpr int MAX_FLOAT_DIGITS_BEFORE_POINT = 8;
	constexpr int MAX_FLOAT_DIGITS_AFTER_POINT = 8;
	ELexemParseCode IsFloat(const char* pString, int InLength);
	// Find the point character in the string;
	// return false, if NOT found;
	// WARNING!!! Initial index value matters!!!
	ELexemParseCode FindPointFromPosition(int *pInOutIndex, const char* pString, int InLength);

	std::string_view          GetLexemStringView  (InputStream* pStream, const char* pLexemStart);

	// Skips to the end of line:
	// WARNING!!! The newline character is also skipped!!!
	ELexemParseCode SkipToEndOfLine(InputStream* pStream);
	ELexemParseCode SkipUntil(char InChar, InputStream* pStream);
} // Dv::ConfigManager