#pragma once

#include <windows.h>
#include "AppEnvironment.h"

namespace Tet
{
	class App
	{
	public:
		App();
		~App();
	
		void Run(LPSTR lpCmdLine, const AppEnvironment& InEnvironment);
		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static App& Get();

	private:
		AppEnvironment Environment;
	};
} // Tet