#pragma once

#include <windows.h>

namespace Test::IMPL
{
	HWND RegisterClassAndCreateWindow
	(
		HINSTANCE hInstance,
		const char *windowClass,
		WNDPROC wndProc,
		const char *title,
		UINT width, UINT height
	);
} // Test::IMPL