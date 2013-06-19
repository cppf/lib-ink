/****************************************************************************
 *                                                                          *
 * File    : windows.cpp                                                    *
 *                                                                          *
 * Purpose : The Window module of pacEngine                                 *
 *                                                                          *
 * History : Date        Part of pacEngine                                  *
 *           07/12/2011  Subhajit Sahu                                      *
 *                                                                          *
 ****************************************************************************/

/* Include Files */
#include <windows.h>
#include "win32.h"
#include "macro.h"



#define	DISPLAY_FULL_SCREEN							0x01
#define	DISPLAY_USE_GRAPHICS_API					0x02

#define	ITEM_TYPE_FILE_NAME							0x01
#define	ITEM_TYPE_RESOURCE_NAME						0x02
#define	ITEM_TYPE_RESOURCE_NUMBER					0x03
#define	ITEM_TYPE_OEM_NUMBER						0x04
#define	ITEM_TYPE_OEM_IMAGE							0x05
#define	ITEM_RGBCOLOR								0x06
#define	ITEM_TYPE_OEM_COLOR							0x07

// SetupDiEnumDeviceInfo - Get GPU Info (SetupAPI - Windows Driver Development (Driver Install))


// Item Holder
#define		_ITEM_HOLDER	\
	UINT	Type;\
	UINT	Data;



// An Item
typedef struct _ITEM
{
	_ITEM_HOLDER
}ITEM;

// The Display Settings
typedef struct	_PACKET_DISPLAY_SETTINGS
{
	POINT			Location;
	POINT			Resolution;
	DWORD			BitsPerPixel;
	DWORD			Flags;
}PACKET_DISPLAY_SETTINGS;

// Window Settings (Private Structure)
typedef struct	_PACKET_WINDOW_SETTINGS
{
	HWND						hWnd;
	HDC							hDC;
	DWORD						Style;
	DWORD						ExStyle;
	HMENU						hMenu;
	HBITMAP						hBackImage;
	TCHAR*						ClassName;
	PACKET_DISPLAY_SETTINGS		Display;
}PACKET_WINDOW_SETTINGS;
#define	PAC_WIN_HANDLE			PACKET_WINDOW_SETTINGS*

typedef struct	_PACKET_WINDOW_CREATE
{
	TCHAR*						ClassName;
	BOOL						Resize;
	BOOL						NoClose;
	void*						Procedure;
	ITEM						Icon;
	ITEM						Cursor;
	ITEM						Background;
	ITEM						Menu;
	UINT						ExStyle;
	TCHAR*						Name;
	UINT						Style;
	PACKET_DISPLAY_SETTINGS		Display;
}PACKET_WINDOW_CREATE;



// External Variables
static UINT						pacError;
static TCHAR*					pacErrorString;

// Global Variables
static HDC						pacWindowHDC;
static HWND						pacWindowHWND;
static HINSTANCE				pacWindowHInstance;
static BOOL						pacWindowMsgWait;
static BOOL						pacWindowMsgWaiting;
static UINT						pacWindowMsgSize = 1024;
static void*					pacWindowMsgControl[1024];
static BOOL						pacWindowMsgTranslate = TRUE;
static HANDLE					pacMainThread;
static DWORD					pacMainThreadID;




// Function Declarations
PROC	pacWindowProcClose(_MSG_PARAMS);



// Sets the title of a Window
// Returns FALSE if Window Title could not be set
BOOL	pacWindowSetTitle(PAC_WIN_HANDLE win, TCHAR* text)
{
	return(SetWindowText(win->hWnd, text));
}


// Gets the title of a Window
// Returns FALSE if Window has no Title
BOOL	pacWindowGetTitle(PAC_WIN_HANDLE win, TCHAR* text, int size)
{
	return(GetWindowText(win->hWnd, text, size) != 0);
}


// Get the Current Display Settings
void	pacWindowGetDisplaySettings(PACKET_DISPLAY_SETTINGS *pac, PAC_WIN_HANDLE win)
{
	CopyMemory(pac, &(win->Display), sizeof(PACKET_DISPLAY_SETTINGS));
}


// Change the Basic display Settings of a Window
BOOL	pacWindowSetDisplaySettings(PAC_WIN_HANDLE win, PACKET_DISPLAY_SETTINGS *disp)
{
	DEVMODE*					DevMode;
	PIXELFORMATDESCRIPTOR*		pfd;
	RECT						rect;
	int							iPixelFormat;
	
	pfd = (PIXELFORMATDESCRIPTOR*)pacStructInitAlloc(pfd);
	pfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd->nVersion = 1;
	pfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd->iPixelType = PFD_TYPE_RGBA;
	pfd->iLayerType = PFD_MAIN_PLANE;

	if((disp->Flags) & DISPLAY_FULL_SCREEN)		// Full Screen Mode Selected
	{
		DevMode = (DEVMODE*)pacStructInitAlloc(DevMode);
		if(disp->Resolution.x > 0) win->Display.Resolution.x = disp->Resolution.x;
		if(disp->Resolution.y > 0) win->Display.Resolution.y = disp->Resolution.y;
		if(disp->BitsPerPixel > 0) win->Display.BitsPerPixel = disp->BitsPerPixel;
		rect.left = 0;
		rect.top = 0;
		rect.right = win->Display.Resolution.x;
		rect.bottom = win->Display.Resolution.y;
		AdjustWindowRectEx(&rect, win->Style, NULL, win->ExStyle);
		SetWindowPos(win->hWnd, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOCOPYBITS | SWP_SHOWWINDOW);
		win->Display.Location.x = 0;
		win->Display.Location.y = 0;
		DevMode->dmSize = sizeof(*DevMode);
		DevMode->dmPelsWidth = win->Display.Resolution.x;
		DevMode->dmPelsHeight = win->Display.Resolution.y;
		DevMode->dmBitsPerPel = win->Display.BitsPerPixel;
		DevMode->dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		iPixelFormat = ChangeDisplaySettings(DevMode, CDS_FULLSCREEN);
		if((win->Display.Flags) & DISPLAY_USE_GRAPHICS_API)
		{
			pfd->dwFlags |=  PFD_SUPPORT_OPENGL;
			pfd->cDepthBits = 32;
		}
		pfd->cColorBits = (BYTE)win->Display.BitsPerPixel;
		iPixelFormat = ChoosePixelFormat(win->hDC, pfd);
		SetPixelFormat(win->hDC, iPixelFormat, pfd);
		pacStructFree(DevMode);
	}
	else						// Window Mode Selected
	{
		if(disp->Resolution.x > 0) win->Display.Resolution.x = disp->Resolution.x;
		if(disp->Resolution.y > 0) win->Display.Resolution.y = disp->Resolution.y;
		if(disp->BitsPerPixel > 0) win->Display.BitsPerPixel = disp->BitsPerPixel;
		win->Display.Location.x = disp->Location.x;
		win->Display.Location.y = disp->Location.y;
		win->Display.Flags = disp->Flags;
		rect.left = win->Display.Location.x;
		rect.top = win->Display.Location.y;
		rect.right = rect.left + (win->Display.Resolution.x);
		rect.bottom = rect.top + (win->Display.Resolution.y);
		AdjustWindowRectEx(&rect, win->Style, NULL, win->ExStyle);
		SetWindowPos(win->hWnd, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOCOPYBITS | SWP_SHOWWINDOW);
		if((win->Display.Flags) & DISPLAY_USE_GRAPHICS_API)
		{
			pfd->dwFlags |=  PFD_SUPPORT_OPENGL;
			pfd->cDepthBits = 32;
		}
		pfd->cColorBits = (BYTE)win->Display.BitsPerPixel;
		iPixelFormat = ChoosePixelFormat(win->hDC, pfd);
		SetPixelFormat(win->hDC, iPixelFormat, pfd);
	}
	pacStructFree(pfd);
	return(TRUE);
}


// The Main Window Handling Procedure
// The Processing done can be dynamically controlled at runtime,
// beacuse of the use of Lookup table 'pacWindowMsgControl' whose
// size is expected to be 4KB memory which is pretty decent memory demand
// Additonally this procedure can be made to wait while its features are
// being changed. In such a case it will send all data to Default Windows Procedure
// Use 'pacWindowMsgWait' to ask this to wait. 'pacWindowMsgWaiting' tells if it
// has started waiting. Clear 'pacWindowMsgWait' to ask it to stop waiting
// Use 'pacWindowMsgControl' to make the changes
_WPROC		pacWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if((!pacWindowMsgWait) && (!pacWindowMsgWaiting))
	{
		if(uMsg <= pacWindowMsgSize)
		{
			if(pacWindowMsgControl[uMsg])
			{
				return(((int(*)(_MSG_PROTO))(pacWindowMsgControl + uMsg))(hWnd, uMsg, wParam, lParam));
			}
		}
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}
	else if(pacWindowMsgWait && (!pacWindowMsgWaiting))
	{
		pacWindowMsgWaiting = TRUE;
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}
	else if(pacWindowMsgWait && pacWindowMsgWaiting)
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	pacWindowMsgWaiting = FALSE;
	return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}


// The Main Windows Message Loop
// Passes Messages to Window Procedure
// Translates KEY msgs' to CHAR msgs' if 'pacWindowMsgTranslate'
// Starts the Main Function thread
// Returns Exit Code
int			pacWindowMsgLoop(void* main_func, void* data, UINT stack_sz)
{
	MSG		msg;
	
	// Make default WM handlers
	pacMainThread = CreateThread(NULL, stack_sz, (LPTHREAD_START_ROUTINE)main_func, data, 0, &pacMainThreadID);
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(pacWindowMsgTranslate)
			TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}


// Get the Windows Message Control Procedure for a Message
 void*		pacWindowGetMsgControl(UINT msg)
 {
	 return(pacWindowMsgControl[msg]);
 }
 

 // Get the Windows Message Control Procedure for a Message
 BOOL		pacWindowSetMsgControl(UINT msg, void* func)
 {
	 if(msg <= pacWindowMsgSize)
	 {
		 while(pacWindowMsgWait);
		 pacWindowMsgWait = TRUE;
		 while(!pacWindowMsgWaiting);
		 pacWindowMsgControl[msg] = func;
		 pacWindowMsgWait = FALSE;
		 return(TRUE);
	 }
	 return(FALSE);
 }
 





PAC_WIN_HANDLE		pacWindowCreate(PACKET_WINDOW_CREATE* pac)
{
	TCHAR*						win_name;
	WNDCLASSEX*					wc;
	PAC_WIN_HANDLE				win;
	static TCHAR*				default_class = _T("pacWindowClassDefault");
	
	wc = (WNDCLASSEX*)pacStructInitAlloc(wc);
	win = (PAC_WIN_HANDLE)pacStructInitAlloc(win);
	win->ClassName = _T("pacWindowClassDefault");
	win->Display.Resolution.x = 40;
	win->Display.Resolution.y = 20;
	win->Display.BitsPerPixel = 32;
	if(pac->Display.Resolution.x) win->Display.Resolution.x = pac->Display.Resolution.x;
	if(pac->Display.Resolution.y) win->Display.Resolution.y = pac->Display.Resolution.y;
	if(pac->Display.BitsPerPixel) win->Display.BitsPerPixel = pac->Display.BitsPerPixel;
	if(pac->ClassName)
	{
		win->ClassName = pacStrAlloc(lstrlen(pac->ClassName) + 1);
		lstrcpy(win->ClassName, pac->ClassName);
	}
	else
	{
		win->ClassName = pacStrAlloc(lstrlen(default_class) + 1);
		lstrcpy(win->ClassName, default_class);
	}
	// First Check if the Window of the Given Class Name already exists
	if(!GetClassInfoEx(pacWindowHInstance, pac->ClassName, wc))
	{
		wc->cbSize = sizeof(WNDCLASSEX);
		wc->style = CS_PARENTDC | CS_DBLCLKS | ((pac->Resize)?(CS_HREDRAW|CS_VREDRAW):0) | ((pac->NoClose)?CS_NOCLOSE:0);
		wc->lpfnWndProc = (WNDPROC)pacWindowProcedure;
		wc->cbClsExtra = 0;
		wc->cbWndExtra = 0;
		wc->hInstance = pacWindowHInstance;
		wc->lpszClassName = win->ClassName;
		if(pac->Procedure) wc->lpfnWndProc = (WNDPROC)pac->Procedure;
		
		// Deal with Window Icon
		switch(pac->Icon.Type)
		{
		case	ITEM_TYPE_FILE_NAME:
			wc->hIcon = (HICON)LoadImage(NULL, (LPCTSTR)(pac->Icon.Data), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_RESOURCE_NAME:
			wc->hIcon = (HICON)LoadImage(pacWindowHInstance, (LPCTSTR)(pac->Icon.Data), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_RESOURCE_NUMBER:
			wc->hIcon = (HICON)LoadImage(pacWindowHInstance, MAKEINTRESOURCE(pac->Icon.Data), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_OEM_IMAGE:
			wc->hIcon = (HICON)LoadImage(NULL, (LPCTSTR)(pac->Icon.Data), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
			break;
		}
		
		// Deal with Window Cursor
		switch(pac->Cursor.Type)
		{
		case	ITEM_TYPE_FILE_NAME:
			wc->hCursor = (HCURSOR)LoadImage(NULL, (LPCTSTR)(pac->Cursor.Data), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_RESOURCE_NAME:
			wc->hCursor = (HCURSOR)LoadImage(pacWindowHInstance, (LPCTSTR)(pac->Cursor.Data), IMAGE_CURSOR, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_RESOURCE_NUMBER:
			wc->hCursor = (HCURSOR)LoadImage(pacWindowHInstance, MAKEINTRESOURCE(pac->Cursor.Data), IMAGE_CURSOR, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_OEM_IMAGE:
			wc->hCursor = (HCURSOR)LoadImage(NULL, (LPCTSTR)(pac->Cursor.Data), IMAGE_CURSOR, 0, 0, LR_LOADTRANSPARENT);
			break;
		}
		
		// Deal with Window Background
		switch(pac->Background.Type)
		{
		case	ITEM_TYPE_FILE_NAME:
			win->hBackImage = (HBITMAP)LoadImage(NULL, (LPCTSTR)(pac->Background.Data), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wc->hbrBackground = CreatePatternBrush(win->hBackImage);
			break;
		
		case	ITEM_TYPE_RESOURCE_NAME:
			win->hBackImage = (HBITMAP)LoadImage(pacWindowHInstance, (LPCTSTR)(pac->Background.Data), IMAGE_BITMAP, 0, 0, 0);
			wc->hbrBackground = CreatePatternBrush(win->hBackImage);
			break;
		
		case	ITEM_TYPE_RESOURCE_NUMBER:
			win->hBackImage = (HBITMAP)LoadImage(pacWindowHInstance, MAKEINTRESOURCE(pac->Background.Data), IMAGE_BITMAP, 0, 0, 0);
			wc->hbrBackground = CreatePatternBrush(win->hBackImage);
			break;
		
		case	ITEM_TYPE_OEM_NUMBER:
			win->hBackImage = (HBITMAP)LoadImage(NULL, (LPCTSTR)(pac->Background.Data), IMAGE_BITMAP, 0, 0, 0);
			wc->hbrBackground = CreatePatternBrush(win->hBackImage);
			break;	
		
		case	ITEM_RGBCOLOR:
			wc->hbrBackground = CreateSolidBrush((COLORREF)(pac->Background.Data));
			break;
		
		case	ITEM_TYPE_OEM_COLOR:
			wc->hbrBackground = (HBRUSH)(pac->Background.Data);
			break;
		}
		
		if(!RegisterClassEx(wc))
		{
			pacError = 0;
			pacErrorString = _T("Registration of Window failed");
			return(NULL);
		}
	}
	
	// Create the Window Now
	switch(pac->Menu.Type)
	{
	case	ITEM_TYPE_RESOURCE_NAME:
		wc->lpszMenuName = (LPCTSTR)(pac->Menu.Data);
		break;
	
	case	ITEM_TYPE_RESOURCE_NUMBER:
		wc->lpszMenuName = MAKEINTRESOURCE(pac->Menu.Data);
		break;
	}
	win->hMenu = LoadMenu(pacWindowHInstance, wc->lpszMenuName);

	// Complete the Window Settings Structure
	win->ExStyle = pac->ExStyle;
	win->Style = pac->Style;
	win_name = pac->Name;
	if(!win_name) win_name = _T("pacWindowName");

	// Create a Temporary Window
	win->hWnd = CreateWindowEx(win->ExStyle, win->ClassName, win_name, win->Style, win->Display.Location.x, win->Display.Location.y, 
	win->Display.Resolution.x, win->Display.Resolution.y, NULL, win->hMenu, pacWindowHInstance, NULL);
	win->hDC = GetDC(win->hWnd);

	// Temporary
	//pacWindowMsgControl[WM_CLOSE] = pacWindowProcClose;

	// Change the Display settings to required form
	pacWindowSetDisplaySettings(win, &(pac->Display));
	return(win);
}



BOOL		pacWindowDestroy(PAC_WIN_HANDLE win)
{
	ReleaseDC(win->hWnd, win->hDC);
	DestroyWindow(win->hWnd);
	if(win->hMenu)DestroyMenu(win->hMenu);
	if(win->hBackImage)DeleteObject(win->hBackImage);
	if(win->ClassName)UnregisterClass(win->ClassName, pacWindowHInstance);
	GlobalFree(win->ClassName);
	GlobalFree(win);
	return(TRUE);
}



PROC	pacWindowProcClose(_MSG_PARAMS)
{
	PostQuitMessage(0);
	return(0);
}


/*
 PACKET*	pacWindowCreateSettingsPacket(UINT win_type)
 {
	PACKET_WINDOW_CREATE*	pac;
	
	pac = pacMemAlloc(sizeof(PACKET_WINDOW_CREATE));
	pac->PacketSize = sizeof(PACKET_WINDOW_CREATE);
	pac->DataType = ;
	pac->DataSize = sizeof(PACKET_WINDOW_CREATE) - sizeof(PACKET);
	
	switch(win_type)
	{
	case	WINDOW_TYPE_GAME_FULL_SCREEN:
		pac->WindowResize = FALSE;
		pac->WindowCloseDisabled = FALSE;
		pac->WindowProcedure = pacWindowProcedure;
		pac->WindowIcon.Type = 0;
		pac->WindowIcon.Data = 0;
		pac->WindowCursor.Type = 0;
		pac->WindowCursor.Data = 0;
		pac->WindowBackground.Type = 0;
		pac->WindowBackground.Data = 0;
		pac->WindowMenu.Type = 0;
		pac->WindowMenu.Data = 0;
		pac->WindowClassName = _T("pacWindow");
		pac->WindowExStyle 
		break;
	}
	
	pac->PacketType = ;
 }
 
 */
 