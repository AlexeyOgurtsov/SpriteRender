#pragma once

#include "SourceLocation.h"

namespace Dv::ConfigManager
{
	void AdvanceStreamLocation(StreamLocation* pStreamLocation, bool bInNewLine)
	{
		pStreamLocation->OffsetFromStart++;
		if (bInNewLine)
		{
			pStreamLocation->Line++;
			pStreamLocation->CharNumber = 0;
		}
		else
		{
			pStreamLocation->CharNumber++;
		}
	}
} // Dv::ConfigManager