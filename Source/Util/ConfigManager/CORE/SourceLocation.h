#pragma once

#include <string>

namespace Dv::ConfigManager
{
	struct StreamLocation
	{
		int Line = 0; // from zero
		int CharNumber = 0; // from zero
		int OffsetFromStart = 0;

		StreamLocation() = default;
		StreamLocation(int InLine, int InCharNumber, int InOffsetFromStart) :
			Line(InLine), CharNumber(InCharNumber), OffsetFromStart(InOffsetFromStart) {}
	};
	void AdvanceStreamLocation(StreamLocation* pStreamLocation, bool bInNewLine);

	struct SourceLocation
	{
		std::string                  Filename;
		StreamLocation               Loc;

		SourceLocation() = default;
		SourceLocation(std::string_view InFilename, const StreamLocation& InLoc) :
			Filename(InFilename), Loc(InLoc) {}
	};

	template<class Strm>
	Strm& operator<<(Strm& strm, const SourceLocation& InLoc)
	{
		strm << InLoc.Loc << "; filename: " << InLoc.Filename;
		return strm; 
	}

	template<class Strm>
	Strm& operator<<(Strm& strm, const StreamLocation& InLoc)
	{
		strm << "line: " << (InLoc.Line+1) << "; char: " << (InLoc.CharNumber + 1);
		return strm;
	}
} // Dv::ConfigManager