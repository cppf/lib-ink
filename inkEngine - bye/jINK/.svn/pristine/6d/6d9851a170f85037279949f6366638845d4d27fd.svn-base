/****************************************************************************
 *                                                                          *
 * File    : input.h                                                        *
 *                                                                          *
 * Purpose : Handling of all input for C                                    *
 *                                                                          *
 * History : Date      		Part of INK Game Engine                         *
 *           14/07/2011		Subhajit Sahu                                   *
 *                                                                          *
 ****************************************************************************/

#ifndef		_input_h_

#define		_input_h_		1



#ifdef __cplusplus
    extern "C" {
#endif



// ----Includes------------------------------------------------------
#include "win32.h"
#include "macro.h"



// ----Prototypes----------------------------------------------------
int		inkStartInputService(char mode, LPVOID control);
int		inkStopInputService(void);
int		inkGetKey(void);
int2	inkGetMouse(void);
void	inkFlushInput(void);
int		inkIsKeyPressed(int key);
void	inkSetKeyControl(int key, int control);
int		inkGetKeyControl(int key);
void	inkLoadKeyControls(void* ctrl);
void	inkSaveKeyControls(void* ctrl);
int		inkGetPressedKeys(void);
int		inkNumberOfMouseButtons(void);
int		inkMouseButtonsSwapped(void);
int		inkMouseWheelPresent(void);
int		inkGetKeyboardDelay(void);
void	inkSetKeyboardDelay(UINT x);
int		inkGetKeyboardSpeed(void);
void	inkSetKeyboardSpeed(UINT x);
void	inkGetMouseInformation(_LPUINT x);
void	inkSetMouseInformation(_LPUINT x);
PROC	Msg_WM_CHAR(_MSG_PARAMS);
PROC	Msg_WM_SYSCHAR(_MSG_PARAMS);
PROC	Msg_WM_KEYDOWN(_MSG_PARAMS);
PROC	Msg_WM_SYSKEYDOWN(_MSG_PARAMS);
PROC	Msg_WM_KEYUP(_MSG_PARAMS);
PROC	Msg_WM_SYSKEYUP(_MSG_PARAMS);
PROC	Msg_WM_LBUTTONDOWN_Text(_MSG_PARAMS);
PROC	Msg_WM_RBUTTONDOWN_Text(_MSG_PARAMS);
PROC	Msg_WM_MBUTTONDOWN_Text(_MSG_PARAMS);
PROC	Msg_WM_LBUTTONUP_Text(_MSG_PARAMS);
PROC	Msg_WM_RBUTTONUP_Text(_MSG_PARAMS);
PROC	Msg_WM_MBUTTONUP_Text(_MSG_PARAMS);
PROC	Msg_WM_LBUTTONDBLCLK_Text(_MSG_PARAMS);
PROC	Msg_WM_RBUTTONDBLCLK_Text(_MSG_PARAMS);
PROC	Msg_WM_MBUTTONDBLCLK_Text(_MSG_PARAMS);
PROC	Msg_WM_MOUSEWHEEL_Text(_MSG_PARAMS);
PROC	Msg_WM_MOUSEMOVE_Text(_MSG_PARAMS);
PROC	Msg_WM_LBUTTONDOWN_Gaming(_MSG_PARAMS);
PROC	Msg_WM_RBUTTONDOWN_Gaming(_MSG_PARAMS);
PROC	Msg_WM_MBUTTONDOWN_Gaming(_MSG_PARAMS);
PROC	Msg_WM_LBUTTONUP_Gaming(_MSG_PARAMS);
PROC	Msg_WM_RBUTTONUP_Gaming(_MSG_PARAMS);
PROC	Msg_WM_MBUTTONUP_Gaming(_MSG_PARAMS);
PROC	Msg_WM_LBUTTONDBLCLK_Gaming(_MSG_PARAMS);
PROC	Msg_WM_RBUTTONDBLCLK_Gaming(_MSG_PARAMS);
PROC	Msg_WM_MBUTTONDBLCLK_Gaming(_MSG_PARAMS);
PROC	Msg_WM_MOUSEWHEEL_Gaming(_MSG_PARAMS);
PROC	Msg_WM_MOUSEMOVE_Gaming(_MSG_PARAMS);




// ----Constants-----------------------------------------------------
#define			_inkInputMode_Text			1
#define			_inkInputMode_Gaming		2
#define			_inkMouseHoldPositionX		320
#define			_inkMouseHoldPositionY		240
#define			_In_ControlSizeB			256
#define			_In_TextSize				63
#define			_In_TextSizeB				256
#define			_In_GameSize				63
#define			_In_GameSizeB				256
#define			_In_GameStatusSizeB			32
#define			_In_MouseSize				63
#define			_In_MouseSizeB				256
#define			_In_Key_MouseLeft			0xFF
#define			_In_Key_MouseRight			0xFE
#define			_In_Key_MouseMiddle			0xFD
#define			_In_Key_MouseLeftDblClk		0xFC
#define			_In_Key_MouseRightDblClk	0xFB
#define			_In_Key_MouseMiddleDblClk	0xFA
#define			_In_Key_MouseWheelPlus		0xF9
#define			_In_Key_MouseWheelMinus		0xF8
#define			_In_Key_MouseXmove			0xF7
#define			_In_Key_MouseYmove			0xF6




// ----Macros--------------------------------------------------------
#define		inkInsertQueue_Text(k)	\
inkInsertQueue(In_TextBuffer, In_TextItems, In_TextFront, In_TextRear, _In_TextSize, (char)(k));


#define		inkInsertQueue_Text2(k1,k2)	\
inkInsertQueue2(In_TextBuffer, In_TextItems, In_TextFront, In_TextRear, _In_TextSize, (k1), (k2));


#define		inkInsertQueue_Game(k)	\
inkInsertQueue(In_GameBuffer, In_GameItems, In_GameFront, In_GameRear, _In_GameSize, (k));


#define		inkInsertQueue_Mouse(k1,k2)	\
inkInsertQueue2(In_MouseBuffer, In_MouseItems, In_MouseFront, In_MouseRear, _In_MouseSize, (k1), (k2));


#define		inkDeleteQueue_Text(k)	\
inkDeleteQueue(In_TextBuffer, In_TextItems, In_TextFront, In_TextRear, _In_TextSize, k);


#define		inkDeleteQueue_Text2(k1,k2)	\
inkDeleteQueue2(In_TextBuffer, In_TextItems, In_TextFront, In_TextRear, _In_TextSize, k1, k2);


#define		inkDeleteQueue_Game(k)	\
inkDeleteQueue(In_GameBuffer, In_GameItems, In_GameFront, In_GameRear, _In_GameSize, k);


#define		inkDeleteQueue_Mouse(k1,k2)	\
inkDeleteQueue2(In_MouseBuffer, In_MouseItems, In_MouseFront, In_MouseRear, _In_MouseSize, k1, k2);


#define		inkKeyPressed_Game(k)	\
(In_GameStatus[k >> 5] & (1 << (k & 31)))


#define		inkKeySet_Game(k)	\
In_GameStatus[k >> 5] |= (1 << (k & 31));


#define		inkKeyReset_Game(k)	\
In_GameStatus[k >> 5] &= ~(1 << (k & 31));



#ifdef __cplusplus
    }
#endif



#endif


