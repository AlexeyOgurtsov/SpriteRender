#include "Window.h"
#include <cassert>

namespace Test::IMPL
{
	HWND RegisterClassAndCreateWindow
	(
		HINSTANCE hInstance,
		const char *windowClass,
		WNDPROC wndProc,
		const char *title,
		UINT width, UINT height
	)
	{
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.lpszClassName = windowClass;
		wc.lpszMenuName = nullptr;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hIcon = nullptr;
		wc.lpfnWndProc = wndProc;
		RegisterClassEx(&wc);

		HWND hWnd = CreateWindowEx
		(
			0,  // ex style
			windowClass, title, WS_OVERLAPPEDWINDOW,
			0, 0, width, height,
			0, 0, hInstance, 0
		);
		ShowWindow(hWnd, true);

		assert(hWnd && "Error: CreateWindowEx returned NULL window handle");
		return hWnd;
	}
} // Test::IMPL