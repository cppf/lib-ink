/****************************************************************************
 *                                                                          *
 * File    : input.c                                                        *
 *                                                                          *
 * Purpose : Handling of all input for C                                    *
 *                                                                          *
 * History : Date      		Part of INK Game Engine                         *
 *           14/07/2011		Subhajit Sahu                                   *
 *                                                                          *
 ****************************************************************************/

#ifndef		_input_c_

#define		_input_c_		1



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



// ----Externals-----------------------------------------------------
extern void*		WM_Handler[];




// ----Global Data---------------------------------------------------
UINT			In_KeyMode;
LPVOID			In_BaseMem;
LPUBYTE			In_Control;
LPUBYTE			In_TextBuffer;
UINT			In_TextItems;
UINT			In_TextFront;
UINT			In_TextRear;
LPUINT			In_GameBuffer;
UINT			In_GameItems;
UINT			In_GameFront;
UINT			In_GameRear;
LPUINT			In_GameStatus;
LPUINT			In_MouseBuffer;
UINT			In_MouseItems;
UINT			In_MouseFront;
UINT			In_MouseRear;


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










// ----Code----------------------------------------------------------



// Start the Input Service with Text Mode(1) or Gaming Mode(2)
int		inkStartInputService(char mode, LPVOID control)
	{
	// Already Started ?
	if(In_KeyMode)inkStopInputService();

	// Text Mode
	if(mode == 1)
		{
		In_BaseMem = (LPVOID)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, _In_TextSizeB + _In_MouseSizeB);
		In_TextBuffer = (_LPUBYTE)In_BaseMem;
		In_MouseBuffer = (_LPUDWORD)((char*)In_TextBuffer + _In_TextSizeB);
		inkFlushInput();
		In_KeyMode = 1;
		
		// Set WM_Handler
		WM_Handler[WM_CHAR] = Msg_WM_CHAR;
		WM_Handler[WM_SYSCHAR] = Msg_WM_SYSCHAR;
		WM_Handler[WM_KEYDOWN] = NULL;
		WM_Handler[WM_SYSKEYDOWN] = NULL;
		WM_Handler[WM_KEYUP] = NULL;
		WM_Handler[WM_SYSKEYUP] = NULL;
		WM_Handler[WM_LBUTTONDOWN] = Msg_WM_LBUTTONDOWN_Text;
		WM_Handler[WM_RBUTTONDOWN] = Msg_WM_RBUTTONDOWN_Text;
		WM_Handler[WM_MBUTTONDOWN] = Msg_WM_MBUTTONDOWN_Text;
		WM_Handler[WM_LBUTTONUP] = Msg_WM_LBUTTONUP_Text;
		WM_Handler[WM_RBUTTONUP] = Msg_WM_RBUTTONUP_Text;
		WM_Handler[WM_MBUTTONUP] = Msg_WM_MBUTTONUP_Text;
		WM_Handler[WM_LBUTTONDBLCLK] = Msg_WM_LBUTTONDBLCLK_Text;
		WM_Handler[WM_RBUTTONDBLCLK] = Msg_WM_RBUTTONDBLCLK_Text;
		WM_Handler[WM_MBUTTONDBLCLK] = Msg_WM_MBUTTONDBLCLK_Text;
		WM_Handler[WM_MOUSEWHEEL] = Msg_WM_MOUSEWHEEL_Text;
		WM_Handler[WM_MOUSEMOVE] = Msg_WM_MOUSEMOVE_Text;
		return(0);
		}
	// Gaming Mode
	else if(mode == 2)
		{
		In_BaseMem = (LPVOID)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, _In_ControlSizeB + _In_GameStatusSizeB + _In_GameSizeB);
		In_Control = (_LPUBYTE)In_BaseMem;
		In_GameStatus = (_LPUINT)((char*)In_Control + _In_ControlSizeB);
		In_GameBuffer = (_LPUINT)((char*)In_GameStatus + _In_GameStatusSizeB);
		inkLoadKeyControls(control);
		inkFlushInput();
		In_KeyMode = 2;
		
		// Set WM_Handler
		WM_Handler[WM_CHAR] = NULL;
		WM_Handler[WM_SYSCHAR] = NULL;
		WM_Handler[WM_KEYDOWN] = Msg_WM_KEYDOWN;
		WM_Handler[WM_SYSKEYDOWN] = Msg_WM_SYSKEYDOWN;
		WM_Handler[WM_KEYUP] = Msg_WM_KEYUP;
		WM_Handler[WM_SYSKEYUP] = Msg_WM_SYSKEYUP;
		WM_Handler[WM_LBUTTONDOWN] = Msg_WM_LBUTTONDOWN_Gaming;
		WM_Handler[WM_RBUTTONDOWN] = Msg_WM_RBUTTONDOWN_Gaming;
		WM_Handler[WM_MBUTTONDOWN] = Msg_WM_MBUTTONDOWN_Gaming;
		WM_Handler[WM_LBUTTONUP] = Msg_WM_LBUTTONUP_Gaming;
		WM_Handler[WM_RBUTTONUP] = Msg_WM_RBUTTONUP_Gaming;
		WM_Handler[WM_MBUTTONUP] = Msg_WM_MBUTTONUP_Gaming;
		WM_Handler[WM_LBUTTONDBLCLK] = Msg_WM_LBUTTONDBLCLK_Gaming;
		WM_Handler[WM_RBUTTONDBLCLK] = Msg_WM_RBUTTONDBLCLK_Gaming;
		WM_Handler[WM_MBUTTONDBLCLK] = Msg_WM_MBUTTONDBLCLK_Gaming;
		WM_Handler[WM_MOUSEWHEEL] = Msg_WM_MOUSEWHEEL_Gaming;
		WM_Handler[WM_MOUSEMOVE] = Msg_WM_MOUSEMOVE_Gaming;
		return(0);
		}
	return(-1);
	}



int		inkStopInputService(void)
	{
	if(!In_KeyMode)return(0);
	In_KeyMode = 0;
	GlobalFree(In_BaseMem);
	return(0);
	}


int		inkGetKey(void)
	{
	int	k;
	if(In_KeyMode == 2)
		{
		inkDeleteQueue_Game(k);
		return(k);
		}
	inkDeleteQueue_Text(k);
	return(k);
	}


int2	inkGetMouse(void)
	{
	int2 k;
	if(In_KeyMode == 1)
		inkDeleteQueue_Mouse(k.loint, k.hiint);
	return(k);
	}



// Empties all the input buffers
void	inkFlushInput(void)
	{
	In_TextItems = 0;
	In_TextFront = 0;
	In_TextRear = 0;
	In_GameItems = 0;
	In_GameFront = 0;
	In_GameRear = 0;
	In_MouseItems = 0;
	In_MouseFront = 0;
	In_MouseRear = 0;
	}




int		inkIsKeyPressed(int key)
	{
	if(In_KeyMode == 2)
		return(inkKeyPressed_Game(key) > 0);
	return(-1);
	}




void	inkSetKeyControl(int key, int control)
	{
	if(In_KeyMode == 2)
		In_Control[key & 0xFF] = (unsigned char)(control & 0xFF);
	}


int		inkGetKeyControl(int key)
	{
	if(In_KeyMode == 2)
		return(In_Control[key & 0xFF]);
	return(-1);
	}



// Load custom controls or the default one (NULL)
void	inkLoadKeyControls(void* ctrl)
	{
	int i;
	
	if(In_KeyMode == 2)
		{
		// Load default controls ?
		if(ctrl == NULL)
			{
			for(i=0; i<_In_ControlSizeB; i++)
				In_Control[i] = i;
			return;
			}
		CopyMemory(In_Control, ctrl, _In_ControlSizeB);
		}
	}



// Save the current controls to a memory location
void	inkSaveKeyControls(void* ctrl)
	{
	if(In_KeyMode == 2)
		CopyMemory(ctrl, In_Control, _In_ControlSizeB);
	}




int		inkGetPressedKeys(void)
	{
	LPUINT status;
	int i, j, c;

	if(In_KeyMode == 2)
		{
		c = 0;
		status = In_GameStatus;
		for(i=0; i<_In_GameStatusSizeB; i+=4)
			{
			if(status[i] == 0)continue;
			j = 0;
			c = 1;
			while(status[i] != 0)
				{
				if(status[i] & 1){ inkInsertQueue_Game(In_Control[(i<<5)|j]|0x400); }
				status[i] = status[i] >> 1;
				j++;
				}
			}
		}
	return(-1);
	}



// Get the number of butons on the mouse (0 = no mouse)
int		inkNumberOfMouseButtons(void)
	{
	return(GetSystemMetrics(SM_CMOUSEBUTTONS));
	}



// Tells if mouse buttons are swapped (non-zero = swapped)
int		inkMouseButtonsSwapped(void)
	{
	return(GetSystemMetrics(SM_SWAPBUTTON));
	}



// Tells if mouse wheel is present (non-zero = present)
int		inkMouseWheelPresent(void)
	{
	return(GetSystemMetrics(SM_MOUSEWHEELPRESENT));
	}



// Get the keyboard initial delay time
int		inkGetKeyboardDelay(void)
	{
	UINT x;
	LPUINT y;
	y = &x;
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, y, 0);
	return(x);
	}



// Set the keyboard initial delay time
void	inkSetKeyboardDelay(UINT x)
	{
	SystemParametersInfo(SPI_SETKEYBOARDDELAY, x, NULL, 0);
	}



// Get the keyboard speed / repeat rate
int		inkGetKeyboardSpeed(void)
	{
	UINT x;
	LPUINT y;
	y = &x;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, y, 0);
	return(x);
	}



// Set the keyboard speed / repeat rate
void	inkSetKeyboardSpeed(UINT x)
	{
	SystemParametersInfo(SPI_SETKEYBOARDSPEED, x, NULL, 0);
	}



// Get mouse x and y threshold values and mouse speed in 3 integer array
void	inkGetMouseInformation(_LPUINT x)
	{
	SystemParametersInfo(SPI_GETMOUSE, 0, x, 0);
	}



// Set mouse x and y threshold values and mouse speed in 3 integer array
void	inkSetMouseInformation(_LPUINT x)
	{
	SystemParametersInfo(SPI_SETMOUSE, 0, x, 0);
	}



// WM_CHAR Message Handler (Text)
PROC	Msg_WM_CHAR(_MSG_PARAMS)
	{
	inkInsertQueue_Text(wParam);
	return(0);
	}



// WM_SYSCHAR Message Handler (Text)
PROC	Msg_WM_SYSCHAR(_MSG_PARAMS)
	{
	inkInsertQueue_Text(wParam|0x80);
	return(0);
	}



// WM_KEYDOWN Message Handler (Gaming)
PROC	Msg_WM_KEYDOWN(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[wParam];
	k2 = (lParam & 0xE0000000) | (((lParam & 0x0000FFFF) << 8) | k1);
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_SYSKEYDOWN Message Handler (Gaming)
PROC	Msg_WM_SYSKEYDOWN(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[wParam];
	k2 = (lParam & 0xE0000000) | ((((lParam & 0x0000FFFF) << 8) | k1) | 0x10000000);
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_KEYUP Message Handler (Gaming)
PROC	Msg_WM_KEYUP(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[wParam];
	k2 = (lParam & 0xE0000000) | (((lParam & 0x0000FFFF) << 8) | k1);
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_SYSKEYUP Message Handler (Gaming)
PROC	Msg_WM_SYSKEYUP(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[wParam];
	k2 = (lParam & 0xE0000000) | ((((lParam & 0x0000FFFF) << 8) | k1) | 0x10000000);
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_LBUTTONDOWN Message Handler (Text)
PROC	Msg_WM_LBUTTONDOWN_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_LBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_LBUTTONDOWN_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[_In_Key_MouseLeft];
	k2 = (0x40000000 & ((inkKeyPressed_Game(k1) == 0) - 1)) | k1;
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_RBUTTONDOWN Message Handler (Text)
PROC	Msg_WM_RBUTTONDOWN_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_RBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_RBUTTONDOWN_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[_In_Key_MouseRight];
	k2 = (0x40000000 & ((inkKeyPressed_Game(k1) == 0) - 1)) | k1;
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_MBUTTONDOWN Message Handler (Text)
PROC	Msg_WM_MBUTTONDOWN_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_MBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_MBUTTONDOWN_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[_In_Key_MouseMiddle];
	k2 = (0x40000000 & ((inkKeyPressed_Game(k1) == 0) - 1)) | k1;
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_LBUTTONUP Message Handler (Text)
PROC	Msg_WM_LBUTTONUP_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_LBUTTONUP Message Handler (Gaming)
PROC	Msg_WM_LBUTTONUP_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[_In_Key_MouseLeft];
	k2 = 0xC0000000 | k1;
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_RBUTTONUP Message Handler (Text)
PROC	Msg_WM_RBUTTONUP_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_RBUTTONUP Message Handler (Gaming)
PROC	Msg_WM_RBUTTONUP_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[_In_Key_MouseRight];
	k2 = 0xC0000000 | k1;
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_MBUTTONUP Message Handler (Text)
PROC	Msg_WM_MBUTTONUP_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_MBUTTONUP Message Handler (Gaming)
PROC	Msg_WM_MBUTTONUP_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = In_Control[_In_Key_MouseMiddle];
	k2 = 0xC0000000 | k1;
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_LBUTTONDBLCLK Message Handler (Text)
PROC	Msg_WM_LBUTTONDBLCLK_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_LBUTTONDBLCLK Message Handler (Gaming)
PROC	Msg_WM_LBUTTONDBLCLK_Gaming(_MSG_PARAMS)
	{
	UINT k;
	k = In_Control[_In_Key_MouseLeftDblClk];
	inkInsertQueue_Game(k);
	return(0);
	}



// WM_RBUTTONDBLCLK Message Handler (Text)
PROC	Msg_WM_RBUTTONDBLCLK_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_RBUTTONDBLCLK Message Handler (Gaming)
PROC	Msg_WM_RBUTTONDBLCLK_Gaming(_MSG_PARAMS)
	{
	UINT k;
	k = In_Control[_In_Key_MouseRightDblClk];
	inkInsertQueue_Game(k);
	return(0);
	}



// WM_MBUTTONDBLCLK Message Handler (Text)
PROC	Msg_WM_MBUTTONDBLCLK_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_MBUTTONDBLCLK Message Handler (Gaming)
PROC	Msg_WM_MBUTTONDBLCLK_Gaming(_MSG_PARAMS)
	{
	UINT k;
	k = In_Control[_In_Key_MouseMiddleDblClk];
	inkInsertQueue_Game(k);
	return(0);
	}



// WM_MOUSEWHEEL Message Handler (Text)
PROC	Msg_WM_MOUSEWHEEL_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_MOUSEWHEEL Message Handler (Gaming)
PROC	Msg_WM_MOUSEWHEEL_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = (wParam>0)?In_Control[_In_Key_MouseWheelPlus]:In_Control[_In_Key_MouseWheelMinus];
	k2 = ((wParam & 0xFFFF0000) >> 8) | k1;
	inkInsertQueue_Game(k2);
	return(0);
	}



// WM_MOUSEMOVE Message Handler (Text)
PROC	Msg_WM_MOUSEMOVE_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_MOUSEMOVE Message Handler (Gaming)
PROC	Msg_WM_MOUSEMOVE_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2, k3, k4;
	int delX, delY;
	delX = (lParam & 0xFFFF) - _inkMouseHoldPositionX;
	delY = (lParam >> 16) - _inkMouseHoldPositionY;
	k1 = ((delX == 0) - 1) & In_Control[_In_Key_MouseXmove];
	k2 = ((delY == 0) - 1) & In_Control[_In_Key_MouseYmove];
	k3 = ((delX & 0xFFFF) << 8) | k1;
	k4 = ((delY & 0xFFFF) << 8) | k2;
	inkInsertQueue_Game(k3);
	inkInsertQueue_Game(k4);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, _inkMouseHoldPositionX, _inkMouseHoldPositionY, 0, 0);
	return(0);
	}







#endif


