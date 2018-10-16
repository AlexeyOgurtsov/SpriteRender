#pragma once

#include <string_view>
#include <fstream>

namespace Dv
{
namespace Spr
{
namespace QRen
{
namespace Utils
{
	/******************************************************************************
	* Write text to the given file
	*
	* Line numbers will be added, if bNumberLines = true;
	*
	* WARNING!!! We delibarately output into ofstream to avoid templates.
	*
	* Returns: number of outputted lines;
	******************************************************************************/
	int WriteText(std::ofstream& oStrm, std::string_view text, bool bNumberLines);
} // Dv::Spr::QRen::Utils
} // Dv::Spr::QRen
} // Dv::Spr
} // Dv