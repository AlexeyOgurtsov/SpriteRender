#pragma once

#include <windows.h> // UINT
#include <string>

namespace Test
{
	/**
	* Do NOT initialize sprite render when resetting environment.
	*/
	constexpr UINT RESET_NO_SPRITE_RENDER = 1 << 0; 

	/**
	* Do NOT setup default render state.
	*/
	constexpr UINT RESET_NO_DEFAULT_RS = 2 << 0;

	std::string EnvResetFlagsToString(UINT InFlags);
} // Test