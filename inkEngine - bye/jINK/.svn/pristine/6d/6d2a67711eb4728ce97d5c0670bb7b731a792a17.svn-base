/****************************************************************************
 *                                                                          *
 * File    : message.c                                                      *
 *                                                                          *
 * Purpose : Win32 Message Service Routines for C                           *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           9/7/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_message_c_

#define	_message_c_		1


// ----Constants-----------------------------------------------------
#define		_WM_HANDLERS						1024




// ----Global Data---------------------------------------------------
static void* WM_Handler[_WM_HANDLERS];




// ----Includes------------------------------------------------------
#include "win32.h"
#include "macro.h"



// ----Externals-----------------------------------------------------
extern HDC			ProgramHDC;
//extern	inkStartGraphicsService;
//extern	inkStartInputService;


// ----Prototypes----------------------------------------------------
PROC	inkStartMessageService(void);
_WPROC	inkMainMsgHandler(_MSG_PARAMS);
LPVOID	inkGetMsgHandler(UINT Msg);
void	inkSetMsgHandler(UINT Msg, void* func);
PROC	Msg_WM_CREATE(_MSG_PARAMS);
PROC	Msg_WM_DESTROY(_MSG_PARAMS);
PROC	Msg_WM_CLOSE(_MSG_PARAMS);


// ----Code----------------------------------------------------------


// Start Windows Message Handlers
PROC	inkStartMessageService(void)
	{
	ZeroMemory(WM_Handler, _WM_HANDLERS<<2);
	WM_Handler[WM_CREATE] = Msg_WM_CREATE;
	WM_Handler[WM_DESTROY] = Msg_WM_DESTROY;
	WM_Handler[WM_CLOSE] = Msg_WM_CLOSE;
	//inkStartGraphicsService();
	//inkStartInputService();
	return(0);
	}



// Main Windows Message Handler
_WPROC	inkMainMsgHandler(_MSG_PARAMS)
	{
	int (*func)(_MSG_PROTO_PARAMS);
	func = WM_Handler[Msg];
	if((int)func) return(func(_MSG_CALL_PARAMS));
	return(DefWindowProc(_MSG_CALL_PARAMS));
	}



// Get a Message Handlers Call Address
LPVOID	inkGetMsgHandler(UINT Msg)
	{
	return(WM_Handler[Msg]);
	}



// Set a new Message Handler (replacing the old one)
void	inkSetMsgHandler(UINT Msg, void* func)
	{
	WM_Handler[Msg] = func;
	}


// Handle WM_CREATE
PROC	Msg_WM_CREATE(_MSG_PARAMS)
	{
	RECT		rect;
	ProgramHDC = GetDC(hWnd);
	GetClientRect(hWnd, &rect);
//	grpStart(rect.right, rect.bottom);
	return(0);
	}



// Handle WM_DESTROY
PROC	Msg_WM_DESTROY(_MSG_PARAMS)
	{
	return(0);
	}



// Handle WM_CLOSE
PROC	Msg_WM_CLOSE(_MSG_PARAMS)
	{
//	grpStop();
	ReleaseDC(hWnd, ProgramHDC);   
	PostQuitMessage(0);
	return(0);
	}



#endif

