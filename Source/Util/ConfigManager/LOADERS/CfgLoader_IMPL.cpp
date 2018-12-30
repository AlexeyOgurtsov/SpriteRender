#include "CfgLoader_IMPL.h"

namespace Dv::ConfigManager
{
	const IValueType* GetNullOrValueType(std::string_view InName, const CfgManager* pCfg)
	{
		auto pTypeLib = pCfg->GetTypeLib();
		auto it = pTypeLib->find(std::string(InName));
		if(it == pTypeLib->cend())
		{
			return nullptr;
		}
		return it->second.get();
	}

	CfgLoadResult StartLexem(InputStream* pStream)
	{
		CfgLoadResult result{};// Initialize with success result

		while (true)
		{
			pStream->StartLexem();
			if (pStream->IsEOF())
			{
				result.LexCode = ELexemParseCode::Eof;
				return result;
			}

			// Check whether comment and get a comment type
			char commStart[2];

			// Check, whether the first character is a comment start
			StreamLocation locCommentCandidateStarts = pStream->GetLoc();
			pStream->Read(&(commStart[0]));
			if (commStart[0] != '/')
			{
				pStream->SeekBack(locCommentCandidateStarts);
				return result;
			}

			ELexemParseCode commentStartParseRes = pStream->Read(&(commStart[1]));
			const bool bItsComment =
				(commentStartParseRes != ELexemParseCode::Eof) &&
				((commStart[1] == '/') || (commStart[1] == '*'));
			if (false == bItsComment)
			{
				// We tried to read the comment start, but it's failed,
				// so the end of stream is not reached because we push back the character.
				pStream->SeekBack(locCommentCandidateStarts);
				result.LexCode = ELexemParseCode::Success;
				return result;
			}

			// We found a comment, let's just skip it
			if (commStart[1] == '/')
			{
				SkipToEndOfLine(pStream);
				continue;
			}

			// Here we have an asterisk-style comment,
			// Let's find its end
			while (true)
			{
				BOOST_ASSERT(commStart[1] == '*');
				result.LexCode = SkipUntil('*', pStream);
				if (result.LexCode != ELexemParseCode::Success)
				{
					result.Code = ECfgLoadResultCode::CommentNotClosed;
					return result;
				}

				pStream->Skip(); // Skip the asterisk (*)
				if (pStream->IsEOF())
				{
					result.Code = ECfgLoadResultCode::CommentNotClosed;
					return result;
				}
				if(pStream->Peek() == '/')
				{
					pStream->Skip();
					break;
				}
				// We have not found an end of comment, so let's get a next char
				// WARNING!! We should NOT skip this character - it may be an asterisk*!!!
			}
		}

		return result;
	}

	CfgLoadResult ParseIdentifier_SkipWhitespaces(InputStream* pStream, std::string_view* pOutIdentifier)
	{
		CfgLoadResult result = StartLexem(pStream);
		if (result.IsFailed())
		{
			return result;
		}
		if (pStream->IsEOF())
		{
			return result;
		}

		result.LexCode = ParseIdentifier(pStream, pOutIdentifier, &result.SourceLoc.Loc);
		if (result.LexCode != ELexemParseCode::Success)
		{
			result.Code = ECfgLoadResultCode::LexemError;
			return result;
		}
		return result;
	}
} // Dv::ConfigManager