/****************************************************************************
 *                                                                          *
 * File    : win32.h                                                        *
 *                                                                          *
 * Purpose : Some general Win32 macros for C++                              *
 *                                                                          *
 * History : Date      Part of PAC Game Engine                              *
 *           9/7/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_win32_h_

#define	_win32_h_		1
 
 
#define	WIN32_DEFAULT_LIBS
#include	<windows.h>
#include	<windowsx.h>
#include	<commctrl.h>
#include	<tchar.h>
#include	<windef.h>
#include	<wingdi.h>
#include	<gl/gl.h>
#include	<gl/glu.h>
#define		NELEMS(a)					(sizeof(a) / sizeof((a)[0]))
#define		WPROC						static LRESULT WINAPI
#define		_WPROC						static LRESULT CALLBACK
#define		ENTRY						int PASCAL
#define		PROC						static int
#define		_HINSTANCE					HANDLE
#define		_MAIN_PARAMS				HINSTANCE hInstance, HINSTANCE hPrevInstance, TCHAR* lpszCmdLine, int nCmdShow
#define		_MAIN_PROTO					HINSTANCE, HINSTANCE, TCHAR*, int
#define		_MSG_PARAMS					HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam		
#define		_MSG_PROTO					HWND, UINT, WPARAM, LPARAM


#endif

