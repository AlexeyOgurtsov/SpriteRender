#pragma once

#include "../ConfigManager.h"
#include "../CORE/InputStream.h"
#include "CfgLoader_Types.h"

namespace Dv::ConfigManager
{
	/**************************************************************************
	* Get value type with the given name, or return nullptr.
	**************************************************************************/
	const IValueType* GetNullOrValueType(std::string_view InName, const CfgManager* pCfg);

	// This function is to be called to start the lexem:
	// - Skips all whitespaces and comments;
	// - Errors codes:
	// 1) EOF to signal that no next lexem may be!!!;
	// 2) UNCLOSED COMMENT to signal that comment Asterisk-style comment is NOT closed.
	CfgLoadResult StartLexem(InputStream* pStream);

	// Parser identifier:
	// _A, _123, _A123, Ab etc.
	// (WARNING!!! Never puts backs)
	CfgLoadResult ParseIdentifier_SkipWhitespaces(InputStream* pStream, std::string_view* pOutIdentifier);
} // Dv::ConfigManager