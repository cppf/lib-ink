// pacEngine Window.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "pacEngine Window.h"
#include "win32.h"
#include "macro.h"
#include "window.cpp"





#define MAX_LOADSTRING 100





// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name






// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void				my_main(void*);








int APIENTRY _tWinMain(_MAIN_PARAMS)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpszCmdLine);

	PACKET_WINDOW_CREATE*	winc;
	PAC_WIN_HANDLE			win;


	pacWindowHInstance = hInstance;
	winc = (PACKET_WINDOW_CREATE*)pacStructInitAlloc(winc);
	winc->Style = WS_POPUP | WS_VISIBLE;
	winc->ExStyle = WS_EX_APPWINDOW;
	winc->Background.Type = ITEM_TYPE_OEM_COLOR;
	winc->Background.Data = COLOR_GRADIENTACTIVECAPTION;
	winc->Display.Location.x = 70;
	winc->Display.Location.y = 70;
	winc->Display.Resolution.x = 1366;
	winc->Display.Resolution.y = 768;
	winc->Display.BitsPerPixel = 32;
	winc->Display.Flags = DISPLAY_FULL_SCREEN | DISPLAY_USE_GRAPHICS_API;
	win = pacWindowCreate(winc);
	return(pacWindowMsgLoop(my_main, NULL, 4096));
}



void	my_main(void* no_use)
{
	UNREFERENCED_PARAMETER(no_use);

	while(1);
}