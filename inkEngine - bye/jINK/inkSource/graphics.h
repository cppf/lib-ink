/****************************************************************************
 *                                                                          *
 * File    : graphics.h                                                     *
 *                                                                          *
 * Purpose : OpenGL Graphics Routines for C                                 *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           1/9/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_graphics_h_

#define	_graphics_h_		loaded




// ----Includes------------------------------------------------------
#include "win32.h"
#include "macro.h"



// ----Prototypes----------------------------------------------------
PROC	inkStartGraphicsService(void);
PROC	inkStopGraphicsService(void);
PROC	Msg_WM_PAINT(_MSG_PARAMS);
PROC	Msg_WM_SIZE(_MSG_PARAMS);
PROC	Msg_WM_CREATE(_MSG_PARAMS);
PROC	Msg_WM_DESTROY(_MSG_PARAMS);






#endif

