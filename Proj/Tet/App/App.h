#pragma once

#include <windows.h>

namespace Tet
{
	class App
	{
	public:
		App();
		~App();
	
		void Run(LPSTR lpCmdLine);
		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static App& Get();

	private:
	};
} // Tet