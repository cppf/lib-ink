/*
CONSOLE EMULATOR
----------------
Console emulations in Windows Environment for Debugging Help
Part of inkEngine
*/



#include "stdafx.h"
#include "tINK.h"
#include "win32.h"
#include "macro.h"



#define MAX_LOADSTRING		100
#define	_Console_Columns	60
#define	_Console_Rows		28
#define	_Console_Xstart		5
#define	_Console_Ystart		5
#define _Console_Ystep		25


// Global Variables:
HINSTANCE	hInst;										// current instance
TCHAR		szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR		szWindowClass[MAX_LOADSTRING];				// the main window class name




// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);



#define		_WM_HANDLERS						100000




// ----Global Data---------------------------------------------------
static void* WM_Handler[_WM_HANDLERS];




// ----Externals-----------------------------------------------------
static HDC			ProgramHDC;


// ----Prototypes----------------------------------------------------
PROC	inkStartMessageService(void);
_WPROC	inkMainMsgHandler(_MSG_PARAMS);
void*	inkGetMsgHandler(UINT Msg);
void	inkSetMsgHandler(UINT Msg, void* func);
PROC	Msg_WM_CREATE(_MSG_PARAMS);
PROC	Msg_WM_DESTROY(_MSG_PARAMS);
PROC	Msg_WM_CLOSE(_MSG_PARAMS);


// ----Code----------------------------------------------------------


// Start Windows Message Handlers
PROC	inkStartMessageService(void)
	{
	ZeroMemory(WM_Handler, _WM_HANDLERS*sizeof(void*));
	WM_Handler[WM_CREATE] = Msg_WM_CREATE;
	WM_Handler[WM_DESTROY] = Msg_WM_DESTROY;
	WM_Handler[WM_CLOSE] = Msg_WM_CLOSE;
	return(0);
	}



// Main Windows Message Handler
_WPROC	inkMainMsgHandler(_MSG_PARAMS)
	{
	int (*func)(_MSG_PROTO_PARAMS);
	func = (int(*)(_MSG_PROTO_PARAMS))WM_Handler[Msg];
	if((int)func) return(func(_MSG_CALL_PARAMS));
	return(DefWindowProc(_MSG_CALL_PARAMS));
	}



// Get a Message Handlers Call Address
void*	inkGetMsgHandler(UINT Msg)
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
	ReleaseDC(hWnd, ProgramHDC);   
	PostQuitMessage(0);
	return(0);
	}




static BOOL		inkWindowStopServiceRequest;
static BOOL		inkWindowStopServiceReady;
static HANDLE	inkWindowThread;





// ----Global Prototypes---------------------------------------------
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
PROC	Msg_WM_LBUTTONDOWN_Detect(_MSG_PARAMS);





// ----Constants-----------------------------------------------------
#define			_ink_MODE_Text					1
#define			_ink_MODE_Game					2
#define			_inkMouseHoldPositionX			32768
#define			_inkMouseHoldPositionY			32768
#define			_In_inkInputControlSizeB		256
#define			_In_TextSize					63
#define			_In_TextSizeB					256
#define			_In_GameSize					63
#define			_In_GameSizeB					256
#define			_In_inkInputGameStatusSizeB		32
#define			_In_MouseSize					63
#define			_In_MouseSizeB					256
#define			_In_Key_MouseLeft				0xFF
#define			_In_Key_MouseRight				0xFE
#define			_In_Key_MouseMiddle				0xFD
#define			_In_Key_MouseLeftDblClk			0xFC
#define			_In_Key_MouseRightDblClk		0xFB
#define			_In_Key_MouseMiddleDblClk		0xFA
#define			_In_Key_MouseWheelPlus			0xF9
#define			_In_Key_MouseWheelMinus			0xF8
#define			_In_Key_MouseXmove				0xF7
#define			_In_Key_MouseYmove				0xF6




// ----Macros--------------------------------------------------------


// Insert data to a queue whose size must be a power of 2 ( max = size-1 )
// The macro may or may not be able to insert data to the queue
#define		inkInsertQueue(queue, num, rear, max, data)	\
if(num <= max)\
{\
	queue[rear] = data;\
	rear = (rear+1) & max;\
	num++;\
}


// Delete data from a queue whose size must be a power of 2 ( max = size-1 )
// The macro may or may not be able to delete data from queue
// data may contain garbage value if queue is empty and data is not initialized
#define		inkDeleteQueue(queue, num, front, max, data)	\
if(num)\
{\
data = queue[front];\
front = (front+1) & max;\
num--;\
}\



// ----Structures----------------------------------------------------

typedef struct _MOUSE_SETTINGS
{
	DWORD		ThresholdX;
	DWORD		ThresholdY;
	DWORD		Acceleration;
}MOUSE_SETTINGS;

typedef struct _INT2
{
	int	low;
	int	high;
}INT2;



// Global/Static Data
static UINT				inkInputXcenter;
static UINT				inkInputYcenter;
static UINT				inkInputKeyMode;
static void*			inkInputBaseMem;
static UBYTE*			inkInputControl;
static UBYTE*			inkInputTextBuffer;
static UINT				inkInputTextItems;
static UINT				inkInputTextFront;
static UINT				inkInputTextRear;
static UINT				inkInputTextMax;
static UINT*			inkInputGameBuffer;
static UINT				inkInputGameItems;
static UINT				inkInputGameFront;
static UINT				inkInputGameRear;
static UINT				inkInputGameMax;
static UINT*			inkInputGameStatus;
static INT2*			inkInputMouseBuffer;
static UINT				inkInputMouseItems;
static UINT				inkInputMouseFront;
static UINT				inkInputMouseRear;
static UINT				inkInputMouseMax;




// ----Code----------------------------------------------------------


/*
Class:	inkInput

First of all inkInput::StartService() must be called before any other
function is used.
At the end of the game, inkInput::StopService() must be called before exit.
*/
class inkInput
{
	// Input Handling Functions Declarations
	public:
	static int				StartService(char mode, void* inkInputControl);
	static void				StopService(void);
	static int				GetKey(void);
	static int				GetCodedKey(void);
	static int				GetKeyFromCode(UINT k);
	static INT2				GetMouse(void);
	static void				Flush(void);
	static int				IsKeyPressed(UINT key);
	static void				SetControl(UINT key, UINT control);
	static int				GetControl(UINT key);
	static void				LoadControls(void* ctrl);
	static void				SaveControls(void* ctrl);
	static int				GetPressedKeys(void);
	static int				GetKeyboardDelay(void);
	static void				SetKeyboardDelay(UINT x);
	static int				GetKeyboardSpeed(void);
	static void				SetKeyboardSpeed(UINT x);
	static MOUSE_SETTINGS	GetMouseInformation(void);
	static void				SetMouseInformation(MOUSE_SETTINGS x);
};



// Start the Input Service with Text Mode(1) or Gaming Mode(2)
BOOL	inkInput::StartService(char mode, void* control)
{
	SuspendThread(inkWindowThread);
	if(inkInputKeyMode)inkInput::StopService();	// Already Started ?

	// Text Mode
	if(mode == 1)
	{
		inkInputBaseMem = (void*)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, _In_TextSizeB + _In_MouseSizeB);
		inkInputTextBuffer = (UBYTE*)inkInputBaseMem;
		inkInputMouseBuffer = (INT2*)((char*)inkInputTextBuffer + _In_TextSizeB);
		Flush();
		inkInputKeyMode = 1;
		
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
		ResumeThread(inkWindowThread);
		return(TRUE);
	}
	// Gaming Mode
	else if(mode == 2)
	{
		inkInputBaseMem = (void*)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, _In_inkInputControlSizeB + _In_inkInputGameStatusSizeB + _In_GameSizeB);
		inkInputControl = (UBYTE*)inkInputBaseMem;
		inkInputGameStatus = (UINT*)((char*)inkInputControl + _In_inkInputControlSizeB);
		inkInputGameBuffer = (UINT*)((char*)inkInputGameStatus + _In_inkInputGameStatusSizeB);
		LoadControls(control);
		Flush();
		inkInputKeyMode = 2;
		
		// Set WM_Handler
		WM_Handler[WM_CHAR] = NULL;
		WM_Handler[WM_SYSCHAR] = NULL;
		WM_Handler[WM_KEYDOWN] = Msg_WM_KEYDOWN;
		WM_Handler[WM_SYSKEYDOWN] = Msg_WM_SYSKEYDOWN;
		WM_Handler[WM_KEYUP] = Msg_WM_KEYUP;
		WM_Handler[WM_SYSKEYUP] = Msg_WM_SYSKEYUP;
		WM_Handler[WM_LBUTTONDOWN] = Msg_WM_LBUTTONDOWN_Detect;
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
		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, _inkMouseHoldPositionX, _inkMouseHoldPositionY, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		ResumeThread(inkWindowThread);
		return(TRUE);
	}
	ResumeThread(inkWindowThread);
	return(FALSE);
}



void	inkInput::StopService(void)
{
	if(!inkInputKeyMode)return;
	inkWindowStopServiceRequest = TRUE;		// Request Window Service Thread to allow to stop a service safely
	while(inkWindowStopServiceReady);			// Wait till Windows Service is ready
	SuspendThread(inkWindowThread);			// Suspend Windows Service
	inkInputKeyMode = 0;
	WM_Handler[WM_CHAR] = NULL;
	WM_Handler[WM_SYSCHAR] = NULL;
	WM_Handler[WM_KEYDOWN] = NULL;
	WM_Handler[WM_SYSKEYDOWN] = NULL;
	WM_Handler[WM_KEYUP] = NULL;
	WM_Handler[WM_SYSKEYUP] = NULL;
	WM_Handler[WM_LBUTTONDOWN] = NULL;
	WM_Handler[WM_RBUTTONDOWN] = NULL;
	WM_Handler[WM_MBUTTONDOWN] = NULL;
	WM_Handler[WM_LBUTTONUP] = NULL;
	WM_Handler[WM_RBUTTONUP] = NULL;
	WM_Handler[WM_MBUTTONUP] = NULL;
	WM_Handler[WM_LBUTTONDBLCLK] = NULL;
	WM_Handler[WM_RBUTTONDBLCLK] = NULL;
	WM_Handler[WM_MBUTTONDBLCLK] = NULL;
	WM_Handler[WM_MOUSEWHEEL] = NULL;
	WM_Handler[WM_MOUSEMOVE] = NULL;
	GlobalFree(inkInputBaseMem);
	inkWindowStopServiceRequest = FALSE;
	inkWindowStopServiceReady = FALSE;
	ResumeThread(inkWindowThread);
}


int	inkInput::GetKey(void)
{
	int		k = -1;
	if(inkInputKeyMode == 2)
	{
		inkDeleteQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameFront, inkInputGameMax, k)
		return(inkInputControl[k]);
	}
	inkDeleteQueue(inkInputTextBuffer, inkInputTextItems, inkInputTextFront, inkInputTextMax, k)
	return(k);
}


int		inkInput::GetCodedKey(void)
{
	int		k = -1;
	//MessageBox(NULL, _T("GetCodedKey"), _T("GetCodedKey"), MB_OK);
	if(inkInputKeyMode == 2)
	{
		inkDeleteQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameFront, inkInputGameMax, k)
		return(k);
	}
	inkDeleteQueue(inkInputTextBuffer, inkInputTextItems, inkInputTextFront, inkInputTextMax, k)
	return(k);
}


int		inkInput::GetKeyFromCode(UINT k)
{
	return(inkInputControl[k]);
}


INT2	inkInput::GetMouse(void)
{
	INT2 k;
	k.low = -1;
	k.high = -1;
	if(inkInputKeyMode == 1)
		inkDeleteQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseFront, inkInputMouseMax, k)
	return(k);
}



// Empties all the input buffers
void	inkInput::Flush(void)
{
	BOOL	StopIt;
	if(!(inkWindowStopServiceReady))
	{
		StopIt = TRUE;
		inkWindowStopServiceRequest = TRUE;
		while(inkWindowStopServiceReady);
		SuspendThread(inkWindowThread);
	}
	inkInputTextItems = 0;
	inkInputTextFront = 0;
	inkInputTextRear = 0;
	inkInputGameItems = 0;
	inkInputGameFront = 0;
	inkInputGameRear = 0;
	inkInputMouseItems = 0;
	inkInputMouseFront = 0;
	inkInputMouseRear = 0;
	if(StopIt)
	{
		inkWindowStopServiceRequest = FALSE;
		inkWindowStopServiceReady = FALSE;
		ResumeThread(inkWindowThread);
	}
}




BOOL	inkInput::IsKeyPressed(UINT key)
{
	if(inkInputKeyMode == 2)
		return((inkInputGameStatus[key >> 5] & (1<<(key & 31))) > 0);
	return(FALSE);
}




void	inkInput::SetControl(UINT key, UINT control)
{
	if(inkInputKeyMode == 2)
		inkInputControl[key & 0xFF] = (UCHAR)(control & 0xFF);
}


int		inkInput::GetControl(UINT key)
{
	if(inkInputKeyMode == 2)
		return(inkInputControl[key & 0xFF]);
	return(-1);
}



// Load custom inkInputControls or the default one (NULL)
void	inkInput::LoadControls(void* ctrl)
{
	int i;
	
	if(inkInputKeyMode == 2)
	{
		// Load default inkInputControls ?
		if(ctrl == NULL)
		{
			for(i=0; i<_In_inkInputControlSizeB; i++)
				inkInputControl[i] = i;
			return;
		}
		else
		{
			CopyMemory(inkInputControl, ctrl, _In_inkInputControlSizeB);
		}
	}
}



// Save the current inkInputControls to a memory location
void	inkInput::SaveControls(void* ctrl)
{
	if(inkInputKeyMode == 2)
	{
		CopyMemory(ctrl, inkInputControl, _In_inkInputControlSizeB);
	}
}




int		inkInput::GetPressedKeys(void)
{
	UINT* status;
	int i, j, c;

	if(inkInputKeyMode == 2)
	{
		c = 0;
		status = inkInputGameStatus;
		for(i=0; i<_In_inkInputGameStatusSizeB; i+=4)
			{
		if(status[i] == 0)continue;
			j = 0;
			c = 1;
			while(status[i] != 0)
			{
				if(status[i] & 1)
					inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, inkInputControl[(i<<5)|j]|0x400)
				status[i] = status[i] >> 1;
				j++;
			}
		}
	}
	return(-1);
}



// Get the number of butons on the mouse (0 = no mouse)
// Get the keyboard initial delay time
int		inkInput::GetKeyboardDelay(void)
{
	UINT x;
	LPUINT y;
	y = &x;
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, y, 0);
	return(x);
}



// Set the keyboard initial delay time
void	inkInput::SetKeyboardDelay(UINT x)
{
	SystemParametersInfo(SPI_SETKEYBOARDDELAY, x, NULL, 0);
}



// Get the keyboard speed / repeat rate
int		inkInput::GetKeyboardSpeed(void)
{
	UINT x;
	LPUINT y;
	y = &x;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, y, 0);
	return(x);
}



// Set the keyboard speed / repeat rate
void	inkInput::SetKeyboardSpeed(UINT x)
{
	SystemParametersInfo(SPI_SETKEYBOARDSPEED, x, NULL, 0);
}



// Get mouse x and y threshold values and mouse speed in 3 integer array
MOUSE_SETTINGS	inkInput::GetMouseInformation(void)
{
	MOUSE_SETTINGS x;
	SystemParametersInfo(SPI_GETMOUSE, 0, (UINT*)&x, 0);
	return(x);
}



// Set mouse x and y threshold values and mouse speed in 3 integer array
void	inkInput::SetMouseInformation(MOUSE_SETTINGS x)
{
	SystemParametersInfo(SPI_SETMOUSE, 0, (UINT*)&x, 0);
}



// WM_CHAR Message Handler (Text)
PROC	Msg_WM_CHAR(_MSG_PARAMS)
{
//	MessageBox(NULL, _T("Msg_WM_CHAR"), _T("Msg_WM_CHAR"), MB_OK);
	inkInsertQueue(inkInputTextBuffer, inkInputTextItems, inkInputTextRear, inkInputTextMax, wParam)
	return(0);
}



// WM_SYSCHAR Message Handler (Text)
PROC	Msg_WM_SYSCHAR(_MSG_PARAMS)
{
//	MessageBox(NULL, _T("Msg_WM_SYSCHAR"), _T("Msg_WM_SYSCHAR"), MB_OK);
	inkInsertQueue(inkInputTextBuffer, inkInputTextItems, inkInputTextRear, inkInputTextMax, wParam|0x80)
	return(0);
}



// WM_KEYDOWN Message Handler (Gaming)
PROC	Msg_WM_KEYDOWN(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_KEYDOWN"), _T("Msg_WM_KEYDOWN"), MB_OK);
	l = inkInputControl[wParam];
	k = (lParam & 0xE0000000) | (((lParam & 0x0000FFFF) << 8) | wParam);
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[l >> 5] |= (1 << (l & 31));
	return(0);
}



// WM_SYSKEYDOWN Message Handler (Gaming)
PROC	Msg_WM_SYSKEYDOWN(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_SYSKEYDOWN"), _T("Msg_WM_SYSKEYDOWN"), MB_OK);
	l = inkInputControl[wParam];
	k = (lParam & 0xE0000000) | ((((lParam & 0x0000FFFF) << 8) | wParam) | 0x10000000);
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[wParam >> 5] |= (1 << (wParam & 31));
	return(0);
}



// WM_KEYUP Message Handler (Gaming)
PROC	Msg_WM_KEYUP(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_KEYUP"), _T("Msg_WM_KEYUP"), MB_OK);
	l = inkInputControl[wParam];
	k = (lParam & 0xE0000000) | (((lParam & 0x0000FFFF) << 8) | wParam);
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[l >> 5] &= ~(1 << (l & 31));
	return(0);
}



// WM_SYSKEYUP Message Handler (Gaming)
PROC	Msg_WM_SYSKEYUP(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_SYSKEYUP"), _T("Msg_WM_SYSKEYUP"), MB_OK);
	l = inkInputControl[wParam];
	k = (lParam & 0xE0000000) | ((((lParam & 0x0000FFFF) << 8) | wParam) | 0x10000000);
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[l >> 5] &= ~(1 << (l & 31));
	return(0);
	}



// WM_LBUTTONDOWN Message Handler (Text)
PROC	Msg_WM_LBUTTONDOWN_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_LBUTTONDOWN_Text"), _T("Msg_WM_LBUTTONDOWN_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);
}



// WM_LBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_LBUTTONDOWN_Gaming(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_LBUTTONDOWN_Gaming"), _T("Msg_WM_LBUTTONDOWN_Gaming"), MB_OK);
	l = inkInputControl[_In_Key_MouseLeft];
	k = (0x40000000 & (((inkInputGameStatus[l >> 5] & (1<<(l & 31))) == 0) - 1)) | _In_Key_MouseLeft;
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[l >> 5] |= (1 << (l & 31));
	return(0);
}



// WM_RBUTTONDOWN Message Handler (Text)
PROC	Msg_WM_RBUTTONDOWN_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_RBUTTONDOWN_Text"), _T("Msg_WM_RBUTTONDOWN_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);
}



// WM_RBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_RBUTTONDOWN_Gaming(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_RBUTTONDOWN_Gaming"), _T("Msg_WM_RBUTTONDOWN_Gaming"), MB_OK);
	l = inkInputControl[_In_Key_MouseRight];
	k = (0x40000000 & (((inkInputGameStatus[l >> 5] & (1<<(l & 31))) == 0) - 1)) | _In_Key_MouseRight;
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[l >> 5] |= (1 << (l & 31));
	return(0);
}



// WM_MBUTTONDOWN Message Handler (Text)
PROC	Msg_WM_MBUTTONDOWN_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_MBUTTONDOWN_Text"), _T("Msg_WM_MBUTTONDOWN_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);
}



// WM_MBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_MBUTTONDOWN_Gaming(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_MBUTTONDOWN_Gaming"), _T("Msg_WM_MBUTTONDOWN_Gaming"), MB_OK);
	l = inkInputControl[_In_Key_MouseMiddle];
	k = (0x40000000 & (((inkInputGameStatus[l >> 5] & (1<<(l & 31))) == 0) - 1)) | _In_Key_MouseMiddle;
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[l >> 5] |= (1 << (l & 31));
	return(0);
}



// WM_LBUTTONUP Message Handler (Text)
PROC	Msg_WM_LBUTTONUP_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_LBUTTONUP_Text"), _T("Msg_WM_LBUTTONUP_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);
}



// WM_LBUTTONUP Message Handler (Gaming)
PROC	Msg_WM_LBUTTONUP_Gaming(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_LBUTTONUP_Gaming"), _T("Msg_WM_LBUTTONUP_Gaming"), MB_OK);
	l = inkInputControl[_In_Key_MouseLeft];
	k = 0xC0000000 | _In_Key_MouseLeft;
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[l >> 5] &= ~(1 << (l & 31));
	return(0);
}



// WM_RBUTTONUP Message Handler (Text)
PROC	Msg_WM_RBUTTONUP_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_RBUTTONUP_Text"), _T("Msg_WM_RBUTTONUP_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);
}



// WM_RBUTTONUP Message Handler (Gaming)
PROC	Msg_WM_RBUTTONUP_Gaming(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_RBUTTONUP_Gaming"), _T("Msg_WM_RBUTTONUP_Gaming"), MB_OK);
	l = inkInputControl[_In_Key_MouseRight];
	k = 0xC0000000 | _In_Key_MouseRight;
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[l >> 5] &= ~(1 << (l & 31));
	return(0);
}



// WM_MBUTTONUP Message Handler (Text)
PROC	Msg_WM_MBUTTONUP_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_MBUTTONUP_Text"), _T("Msg_WM_MBUTTONUP_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);
}



// WM_MBUTTONUP Message Handler (Gaming)
PROC	Msg_WM_MBUTTONUP_Gaming(_MSG_PARAMS)
{
	UINT k, l;
//	MessageBox(NULL, _T("Msg_WM_MBUTTONUP_Gaming"), _T("Msg_WM_MBUTTONUP_Gaming"), MB_OK);
	l = inkInputControl[_In_Key_MouseMiddle];
	k = 0xC0000000 | _In_Key_MouseMiddle;
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	inkInputGameStatus[l >> 5] &= ~(1 << (l & 31));
	return(0);
}



// WM_LBUTTONDBLCLK Message Handler (Text)
PROC	Msg_WM_LBUTTONDBLCLK_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_LBUTTONDBLCLK_Text"), _T("Msg_WM_LBUTTONDBLCLK_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);	
}



// WM_LBUTTONDBLCLK Message Handler (Gaming)
PROC	Msg_WM_LBUTTONDBLCLK_Gaming(_MSG_PARAMS)
{
//	MessageBox(NULL, _T("Msg_WM_LBUTTONDBLCLK_Gaming"), _T("Msg_WM_LBUTTONDBLCLK_Gaming"), MB_OK);
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, _In_Key_MouseLeftDblClk)
	return(0);
}



// WM_RBUTTONDBLCLK Message Handler (Text)
PROC	Msg_WM_RBUTTONDBLCLK_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_RBUTTONDBLCLK_Text"), _T("Msg_WM_RBUTTONDBLCLK_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);	
}



// WM_RBUTTONDBLCLK Message Handler (Gaming)
PROC	Msg_WM_RBUTTONDBLCLK_Gaming(_MSG_PARAMS)
{
//	MessageBox(NULL, _T("Msg_WM_RBUTTONDBLCLK_Gaming"), _T("Msg_WM_RBUTTONDBLCLK_Gaming"), MB_OK);
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, _In_Key_MouseRightDblClk)
	return(0);
}



// WM_MBUTTONDBLCLK Message Handler (Text)
PROC	Msg_WM_MBUTTONDBLCLK_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_MBUTTONDBLCLK_Text"), _T("Msg_WM_MBUTTONDBLCLK_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);	
}



// WM_MBUTTONDBLCLK Message Handler (Gaming)
PROC	Msg_WM_MBUTTONDBLCLK_Gaming(_MSG_PARAMS)
{
//	MessageBox(NULL, _T("Msg_WM_MBUTTONDBLCLK_Gaming"), _T("Msg_WM_MBUTTONDBLCLK_Gaming"), MB_OK);
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, _In_Key_MouseMiddleDblClk)
	return(0);
}



// WM_MOUSEWHEEL Message Handler (Text)
PROC	Msg_WM_MOUSEWHEEL_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_MOUSEWHEEL_Text"), _T("Msg_WM_MOUSEWHEEL_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);	
}



// WM_MOUSEWHEEL Message Handler (Gaming)
PROC	Msg_WM_MOUSEWHEEL_Gaming(_MSG_PARAMS)
{
	INT k, l;
//	MessageBox(NULL, _T("Msg_WM_MOUSEWHEEL_Gaming"), _T("Msg_WM_MOUSEWHEEL_Gaming"), MB_OK);
	l = (wParam & 0x80000000)?_In_Key_MouseWheelMinus:_In_Key_MouseWheelPlus;
	k = ((wParam & 0xFFFF0000) >> 8) | l;
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k)
	return(0);
}



// WM_MOUSEMOVE Message Handler (Text)
PROC	Msg_WM_MOUSEMOVE_Text(_MSG_PARAMS)
{
	INT2	k;
//	MessageBox(NULL, _T("Msg_WM_MOUSEMOVE_Text"), _T("Msg_WM_MOUSEMOVE_Text"), MB_OK);
	k.low = wParam;
	k.high = lParam;
	inkInsertQueue(inkInputMouseBuffer, inkInputMouseItems, inkInputMouseRear, inkInputMouseMax, k)
	return(0);	
}



// WM_MOUSEMOVE Message Handler (Gaming)
PROC	Msg_WM_MOUSEMOVE_Gaming(_MSG_PARAMS)
{
	UINT k1, k2, k3, k4;
	UINT delX, delY;
//	MessageBox(NULL, _T("Msg_WM_MOUSEMOVE_Gaming"), _T("Msg_WM_MOUSEMOVE_Gaming"), MB_OK);
	delX = (lParam & 0xFFFF) - inkInputXcenter;
	delY = ((lParam >> 16) & 0xFFFF) - inkInputYcenter;
	k1 = ((delX == 0) - 1) & _In_Key_MouseXmove;
	k2 = ((delY == 0) - 1) & _In_Key_MouseYmove;
	k3 = ((delX & 0xFFFF) << 8) | k1;
	k4 = ((delY & 0xFFFF) << 8) | k2;
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k3)
	inkInsertQueue(inkInputGameBuffer, inkInputGameItems, inkInputGameRear, inkInputGameMax, k4)
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, _inkMouseHoldPositionX, _inkMouseHoldPositionY, 0, 0);
	return(0);
}



// WM_LBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_LBUTTONDOWN_Detect(_MSG_PARAMS)
{
	inkInputXcenter = LOWORD(lParam);
	inkInputYcenter = HIWORD(lParam);
	WM_Handler[WM_LBUTTONDOWN] = Msg_WM_LBUTTONDOWN_Gaming;
	return(0);
}










/*
Class:	inkString

A set of string management functions
*/
class inkString
{
	// String Management Functions Declarations
public:	
	static TCHAR*	strcpy_alloc(TCHAR* &dest, TCHAR* src);
	static TCHAR*	strconv_CharToTchar(TCHAR* dest, char* src);
	static TCHAR*	strconv_WcharToTchar(TCHAR* dest, WCHAR* src);
	static char*	strconv_TcharToChar(char* dest, TCHAR* src);
	static WCHAR*	strconv_TcharToWchar(WCHAR* dest, TCHAR* src);
};

// Allocate Memory at dest, then copy string from src to dest
TCHAR* inkString::strcpy_alloc(TCHAR* &dest, TCHAR* src)
{
	UINT len;
	len = lstrlen(src);
	dest = (TCHAR*)GlobalAlloc(GMEM_FIXED, len*sizeof(TCHAR));
	lstrcpy(dest, src);
	return(dest);
}

// Convert a string from char* to TCHAR*
TCHAR* inkString::strconv_CharToTchar(TCHAR* dest, char* src)
{
	#ifdef UNICODE
		while(*src)
		{
			*dest = (TCHAR)src;
			src++;
			dest++;
		}
	#else
	lstrcpyA(dest, src);
	#endif
	return(dest);
}

// Convert a string from WCHAR* to TCHAR*
TCHAR* inkString::strconv_WcharToTchar(TCHAR* dest, WCHAR* src)
{
	#ifndef UNICODE
		while(*src)
		{
			*dest = (TCHAR)src;
			src++;
			dest++;
		}
	#else
		lstrcpy(dest, src);
	#endif
	return(dest);
}

// Convert a string from TCHAR* to char*
char* inkString::strconv_TcharToChar(char* dest, TCHAR* src)
{
	#ifdef UNICODE
		while(*src)
		{
			*dest = (char)src;
			src++;
			dest++;
		}
	#else
		lstrcpy(dest, src);
	#endif
	return(dest);
}

// Convert a string from TCHAR* to WCHAR*
WCHAR* inkString::strconv_TcharToWchar(WCHAR* dest, TCHAR* src)
{
	#ifndef UNICODE
		while(*src)
		{
			*dest = (WCHAR)src;
			src++;
			dest++;
		}
	#else
		lstrcpy(dest, src);
	#endif
	return(dest);
}













class Console
{
private:
	HWND		hWnd;
	UINT		CursorRow;
	UINT		CursorCol;
	TCHAR		Buffer[_Console_Rows][_Console_Columns];

public:
	Console(HWND hwnd);
	void ShiftUp(void);
	void Clear(void);
	void ClearLine(UINT line);
	void Display(HDC hdc);
	void Locate(UINT row, UINT col);
	void Show(TCHAR* string);
};

Console::Console(HWND hwnd)
{
	hWnd = hwnd;
	CursorRow = 0;
	CursorCol = 0;
	ZeroMemory(Buffer, _Console_Rows * _Console_Columns * sizeof(TCHAR));
}

void Console::ShiftUp(void)
{
	UINT i;
	for(i=1; i<_Console_Rows; i++)
		CopyMemory(Buffer[i-1], Buffer[i], _Console_Columns * sizeof(TCHAR));
	ZeroMemory(Buffer[_Console_Rows-1], _Console_Columns);
}

void Console::Clear(void)
{
	ZeroMemory(Buffer, _Console_Rows * _Console_Columns * sizeof(TCHAR));
}

void Console::ClearLine(UINT line)
{
	ZeroMemory(Buffer[line], _Console_Columns * sizeof(TCHAR));
}

void Console::Display(HDC hdc)
{
	UINT i;
	SetTextAlign(hdc, TA_LEFT);
	SetBkColor(hdc, 0x00FFFFFF);
	SetTextColor(hdc, 0);
	for(i=0; i<_Console_Rows; i++)
		TextOut(hdc, _Console_Xstart, _Console_Ystart + i*_Console_Ystep, Buffer[i], lstrlen(Buffer[i]));
}

void Console::Locate(UINT row, UINT col)
{
	CursorRow = row;
	CursorCol = col;
}

void Console::Show(TCHAR* string)
{
	UINT len, i;
	len = lstrlen(string);
	for(i=0; i<len; i++)
	{
		if(string[i] == _T('\n'))
		{
			CursorRow++;
			CursorCol = 0;
			if(CursorRow >= _Console_Rows)
			{
				CursorRow = _Console_Rows - 1;
				ShiftUp();
			}
		}
		Buffer[CursorRow][CursorCol] = string[i];
		CursorCol++;
		if(CursorCol >= _Console_Columns)
		{
			CursorCol = 0;
			CursorRow++;
			if(CursorRow >= _Console_Rows)
			{
				CursorRow = _Console_Rows - 1;
				ShiftUp();
			}
		}
	}
	InvalidateRect(hWnd, NULL, TRUE);
}



Console*	dos;
HANDLE		MainThread;




void ConsoleCode(void *no_use)
{
	UNREFERENCED_PARAMETER(no_use);
	TCHAR*	buffer;
	UINT	i;
	buffer = (TCHAR*)GlobalAlloc(GMEM_FIXED, 1024);
	inkInput::StartService(2, NULL);

	wsprintf(buffer, _T("Input Service Started\n"));
	dos->Show(buffer);
	while(1)
	{
		i = inkInput::GetCodedKey();
		while(i == -1) i = inkInput::GetCodedKey();
			wsprintf(buffer, _T("(%d)\n"), i & 0xFF);
			dos->Show(buffer);
	}
	inkInput::StopService();
	ExitThread(0);
}





int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MSG msg;
	HACCEL hAccelTable;
	int ta;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TINK, szWindowClass, MAX_LOADSTRING);
	inkStartMessageService();
	MyRegisterClass(hInstance);
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TINK));
	inkWindowThread = GetCurrentThread();
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &inkWindowThread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	MainThread = CreateThread(NULL, 4096, (LPTHREAD_START_ROUTINE)ConsoleCode, NULL, 0, NULL);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		ta = 0;
		if(inkInputKeyMode != 3)
			ta = TranslateAccelerator(msg.hwnd, hAccelTable, &msg);
		if (!ta)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(inkWindowStopServiceRequest)
		{
			inkWindowStopServiceReady = TRUE;
			while(inkWindowStopServiceRequest);
		}
	}
	return((int)msg.wParam);
}







ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TINK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TINK);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return(RegisterClassEx(&wcex));
}






BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	
	hInst = hInstance; // Store instance handle in our global variable
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return FALSE;
	dos = new Console(hWnd);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return(TRUE);
}







LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		dos->Display(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		//return DefWindowProc(hWnd, message, wParam, lParam);
		return(inkMainMsgHandler(hWnd, message, wParam, lParam));
	}
	return(0);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return((INT_PTR)FALSE);
}


