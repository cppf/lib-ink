/****************************************************************************
 *                                                                          *
 * File    : win32.h                                                        *
 *                                                                          *
 * Purpose : Some general Win32 macros for C++                              *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           9/7/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_win32_h_

#define	_win32_h_		loaded
 
 
#define	WIN32_DEFAULT_LIBS
#include	<windows.h>
#include	<windowsx.h>
#include	<commctrl.h>
#include	<tchar.h>
#include	<windef.h>
#include	<wingdi.h>
#include	<gl/gl.h>
#include	<gl/glu.h>
#define		NELEMS(a)																(sizeof(a) / sizeof((a)[0]))

#define		WPROC																	static LRESULT WINAPI
#define		_WPROC																	static LRESULT CALLBACK
#define		ENTRY																	int PASCAL
#define		PROC																	static int
#define		_HINSTANCE																HANDLE
#define		PROCESS_MSG(t,p)														case t: return(p(hWnd, Msg, wParam, lParam));
#define		MAIN_PARAMS																HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow


#endif

