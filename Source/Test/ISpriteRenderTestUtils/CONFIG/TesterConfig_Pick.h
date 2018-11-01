#pragma once

#include <string>
#include <windows.h>

namespace Test
{
	/**
	* Configuration of the pick buffer.
	*/
	struct TesterConfig_Pick
	{
		bool bEnabled = false;

		/**
		* Buffer width: if zero, than user render target's dimensions.
		*/
		UINT BufferWidth = 0;

		/**
		* Buffer width: if zero, than user render target's dimensions.
		*/
		UINT BufferHeight = 0;
	};
} // Test