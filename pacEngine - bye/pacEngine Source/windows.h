/****************************************************************************
 *                                                                          *
 * File    : windows.h                                                      *
 *                                                                          *
 * Purpose : Windows handling routines for C                                *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           9/7/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_windows_h_

#define	_windows_h_		1



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




// ----Externals-----------------------------------------------------
extern	_HINSTANCE			ink_hInstance;
extern	LPSTR				ink_CmdLine;
extern int					ink_CmdShow;
extern HDC					ink_MainHDC;
extern HWND					ink_MainHWND;
extern MSG					inkWindowMsg;





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









#endif

