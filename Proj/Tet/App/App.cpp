#include "App.h"

namespace Tet
{
	App::App()	
	{
	}

	App::~App()
	{
	}
	
	void App::Run(LPSTR lpCmdLine, const AppEnvironment& InEnvironment)
	{
		Environment = InEnvironment;

		// @TODO
	}

	LRESULT CALLBACK App::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
			break;

		default:
			break;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK App::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return Get().WndProc(hWnd, uMsg, wParam, lParam);
	}

	App& App::Get()
	{
		static App app;
		return app;
	}
} // Tet