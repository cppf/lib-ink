/****************************************************************************
 *                                                                          *
 * File    : message.h                                                      *
 *                                                                          *
 * Purpose : Win32 Message Service Routines for C                           *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           9/7/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_message_h_

#define	_message_h_		1


// ----Constants-----------------------------------------------------
#define		_WM_HANDLERS						1024



#ifdef __cplusplus
    extern "C" {
#endif



// ----Includes------------------------------------------------------
#include "win32.h"
#include "macro.h"




// ----Prototypes----------------------------------------------------
PROC	inkStartMessageService(void);
_WPROC	inkMainMsgHandler(_MSG_PARAMS);
LPVOID	inkGetMsgHandler(UINT Msg);
void	inkSetMsgHandler(UINT Msg, void* func);
PROC	Msg_WM_CREATE(_MSG_PARAMS);
PROC	Msg_WM_DESTROY(_MSG_PARAMS);
PROC	Msg_WM_CLOSE(_MSG_PARAMS);


#ifdef __cplusplus
    }
#endif




#endif

