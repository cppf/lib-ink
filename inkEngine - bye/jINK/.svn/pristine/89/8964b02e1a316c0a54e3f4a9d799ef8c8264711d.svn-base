/****************************************************************************
 *                                                                          *
 * File    : windows.c                                                      *
 *                                                                          *
 * Purpose : Windows handling routines for C                                *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           9/7/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_windows_c_

#define	_windows_c_		1



// ----Includes------------------------------------------------------
#include "win32.h"
#include "macro.h"





// ----Format Simplifiers--------------------------------------------
#define		inkMain(params)		\
ENTRY	WinMain(params)


#define		_MAIN_PARAMS	\
HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow


#define		inkInitMain()	\
ink_hInstance=hInst;\
ink_CmdLine=lpszCmdLine;\
ink_CmdShow=nCmdShow;





// ----Prototypes----------------------------------------------------
void	inkStartWindowsService(void);
void	inkSetWindowStyle(UINT style);
void	inkSetWindowIcon(HICON icon);
void	inkSetWindowCursor(HCURSOR cursor);
void	inkSetWindowBackground(HBRUSH bck);
void	inkSetWindowIconSmall(HICON icon);
void	inkSetWindowMenuName(LPCTSTR menu);
void	inkSetWindowClassName(LPCTSTR classname);
void	inkCreateWindowType(void);
void	inkSetWindowExtendedStyle(DWORD style);
void	inkSetWindowDrawStyle(DWORD style);
void	inkSetWindowXstart(int x);
void	inkSetWindowYstart(int y);
void	inkSetWindowDrawMenu(HMENU menu);
HWND	inkCreateWindow(LPCTSTR class, LPCTSTR window, int width, int height);




// ----Externals-----------------------------------------------------
extern 	__WPROC inkMainMsgHandler(_MSG_PARAMS);




// ----Global Data---------------------------------------------------
static	_HINSTANCE			ink_hInstance;
static	LPSTR				ink_CmdLine;
static int					ink_CmdShow;
static HDC					ink_MainHDC;
static HWND					ink_MainHWND;
static UINT					inkWindowStyle;
static HICON				inkWindowIcon;
static HCURSOR				inkWindowCursor;
static HBRUSH				inkWindowBackground;
static HICON				inkWindowIconSmall;
static LPCTSTR				inkWindowMenuName;
static LPCTSTR				inkWindowClassName;
static DWORD				inkWindowDrawExStyle;
static DWORD				inkWindowDrawStyle;
static HMENU				inkWindowDrawMenu;
static int					inkWindowDrawX;
static int					inkWindowDrawY;
static MSG					inkWindowMsg;




// ----Code----------------------------------------------------------

// Start the Windows Service
void	inkStartWindowsService(void)
	{
	inkWindowStyle = CS_BYTEALIGNWINDOW;
	inkWindowIcon = LoadIcon(NULL, IDI_APPLICATION);
	inkWindowCursor = LoadCursor(NULL, IDC_ARROW);
	inkWindowBackground = NULL;
	inkWindowIconSmall = NULL;
	inkWindowMenuName = NULL;
	inkWindowClassName = "inkWindowClass";
	inkWindowDrawExStyle = WS_EX_CLIENTEDGE;
	inkWindowDrawStyle = WS_OVERLAPPEDWINDOW;
	inkWindowDrawMenu = NULL;
	inkWindowDrawX = CW_USEDEFAULT;
	inkWindowDrawY = CW_USEDEFAULT;
	}




// Set the window style
void	inkSetWindowStyle(UINT style)
	{
	inkWindowStyle = style;
	}



// Set the window icon
void	inkSetWindowIcon(HICON icon)
	{
	inkWindowIcon = icon;
	}



// Set the window cursor
void	inkSetWindowCursor(HCURSOR cursor)
	{
	inkWindowCursor = cursor;
	}



// Set the window background
void	inkSetWindowBackground(HBRUSH bck)
	{
	inkWindowBackground = bck;
	}



// Set the window small icon
void	inkSetWindowIconSmall(HICON icon)
	{
	inkWindowIconSmall = icon;
	}



// Set the window small icon
void	inkSetWindowMenuName(LPCTSTR menu)
	{
	inkWindowMenuName = menu;
	}



// Set the window small icon
void	inkSetWindowClassName(LPCTSTR classname)
	{
	inkWindowClassName = classname;
	}



// Register window class
void	inkCreateWindowType(void)
	{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = inkWindowStyle;
	wc.lpfnWndProc = inkMainMsgHandler;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ink_hInstance;
	wc.hIcon = inkWindowIcon;
	wc.hCursor = inkWindowCursor;
	wc.hbrBackground = inkWindowBackground;
	wc.lpszMenuName = inkWindowMenuName;
	wc.lpszClassName = inkWindowClassName;
	wc.hIconSm = inkWindowIconSmall;
	RegisterClassEx(&wc);
	}



// Set the window draw extended style
void	inkSetWindowExtendedStyle(DWORD style)
	{
	inkWindowDrawExStyle = style;
	}



// Set the window draw style
void	inkSetWindowDrawStyle(DWORD style)
	{
	inkWindowDrawStyle = style;
	}



// Set the window x start pixel
void	inkSetWindowXstart(int x)
	{
	inkWindowDrawX = x;
	}



// Set the window y start pixel
void	inkSetWindowYstart(int y)
	{
	inkWindowDrawY = y;
	}



// Set the window draw menu
void	inkSetWindowDrawMenu(HMENU menu)
	{
	inkWindowDrawMenu = menu;
	}



// Create a window and return its window handle
HWND	inkCreateWindow(LPCTSTR class, LPCTSTR window, int width, int height)
	{
	ink_MainHWND = CreateWindowEx(inkWindowDrawExStyle, class, window, inkWindowDrawStyle, inkWindowDrawX,
							inkWindowDrawY, width, height, NULL, inkWindowDrawMenu, ink_hInstance, NULL);
	ShowWindow(ink_MainHWND, ink_CmdShow);
	UpdateWindow(ink_MainHWND);
	return(ink_MainHWND);
	}





// ----Format Simplifiers--------------------------------------------
#define		ink_MainLoop_None		\
while(GetMessage(&inkWindowMsg, NULL, 0, 0))\
{\
TranslateMessage(&inkWindowMsg);\
DispatchMessage(&inkWindowMsg);\
}


#define		ink_MainLoop(code)	\
while(1)\
{while(!PeekMessage(&inkWindowMsg,NULL,0,0,PM_REMOVE));\
if(inkWindowMsg.message==WM_QUIT)break;\
TranslateMessage(&inkWindowMsg);\
DispatchMessage(&inkWindowMsg);\
code(&inkWindowsMsg);\
}






#endif

