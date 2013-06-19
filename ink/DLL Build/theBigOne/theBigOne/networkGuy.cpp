/****************************************************************************
 *                                                                          *
 * File    : interruptGuy.cpp                                               *
 *                                                                          *
 * Purpose : The functions for inkEngine related to message handling.       *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           31/1/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


 #ifndef	_interruptGuy_cpp_
 #define	_interruptGuy_cpp_		1
 
#include "stdafx.h"
#include "theBigOne.h"
#include "win32.h"
#include "macro.h"
#include "threadGuy.cpp"


// Export with no name mangling
#ifdef __cplusplus
extern "C" {
#endif



// Some Internal Definitions
#define	_INTERRUPT			int (**)(_MSG_PROTO_PARAMS)
#define	_VOID_CALL			void (*)()
#define	_LPMSG_CALL			void (*)(LPMSG)



// Structures
typedef struct _INTERRUPTS
{
	UINT				Size;
	UINT				Type;
	BOOL				Change;
	void*				Function[2];
	HACCEL				hAccel;
	int					(**List)(_MSG_PROTO_PARAMS);
}INTERRUPTS;



// Flags
#define	_INTERRUPT_NOWAIT_MODE			1
#define	_INTERRUPT_TEXT_MODE			2




// Data
INTERRUPTS	Interrupt;



// Function Declarations
int __cdecl inkStartInterruptService(UINT InterruptSize, UINT Type);
void __cdecl inkStopInterruptService(void);
int __cdecl inkWaiter(void);
int __cdecl inkWaitHandler(void);
int __cdecl inkNoWaitHandler(void);
_WPROC inkWndProc(_MSG_PARAMS);
DLL_API BOOL __cdecl inkSelectInterruptMode(UINT Type);
DLL_API BOOL __cdecl inkSetCallFunctions(void (*msg_func)(MSG*), void (*nomsg_func)());
DLL_API BOOL __cdecl inkSetInterruptAccelertorTable(HACCEL hAccelTable);
DLL_API void* __cdecl inkGetInterruptFunction(UINT Msg);
DLL_API BOOL __cdecl inkSetInterruptFunction(UINT Msg, int (*func)(_MSG_PROTO_PARAMS));



// Starts the Interrupt Service (Release)
int __cdecl inkStartInterruptService(UINT InterruptSize, UINT Type)
{
	Interrupt.Size = InterruptSize;
	Interrupt.hAccel = (HACCEL)0;
	Interrupt.Type = Type;
	Interrupt.List = (_INTERRUPT)inkMemInitAlloc(sizeof(void*) * Interrupt.Size);
	return(inkWaiter());
}

// Stops the Interrupt Service (Release)
void __cdecl inkStopInterruptService(void)
{
	inkMemFree(Interrupt.List);
}

// Helps switch between Wait and NoWait Interrupt mode (Release)
int __cdecl inkWaiter(void)
{
	int		ret;

	Interrupt.Change = FALSE;
	while(1)
	{
		if(!(Interrupt.Type & _INTERRUPT_NOWAIT_MODE)) ret = inkWaitHandler();
		else ret = inkNoWaitHandler();
		if(!Interrupt.Change)break;
		Interrupt.Change = FALSE;
	}
	return(ret);
}

// Windows Message Handler that waits for messages (Release)
int __cdecl inkWaitHandler(void)
{
	MSG		Msg;

	while(GetMessage(&Msg, NULL, 0, 0))
	{
		if(Interrupt.hAccel) TranslateAccelerator(Msg.hwnd, Interrupt.hAccel, &Msg);
		if(Interrupt.Type & _INTERRUPT_TEXT_MODE) TranslateMessage(&Msg);
		if(Interrupt.Function[0]) (*((_LPMSG_CALL)Interrupt.Function[0]))(&Msg);
		DispatchMessage(&Msg);
		if(Interrupt.Change)
			break;
	}
	return(Msg.wParam);
}

// Windows Message Handler that doesn't wait for messages (Release)
int __cdecl inkNoWaitHandler(void)
{
	MSG		Msg;
	
	while(1)
	{
		while(!PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			(*((_VOID_CALL)Interrupt.Function[1]))();
		}
		if(Msg.message == WM_QUIT) break;
		if(Interrupt.hAccel) TranslateAccelerator(Msg.hwnd, Interrupt.hAccel, &Msg);
		if(Interrupt.Type & _INTERRUPT_TEXT_MODE) TranslateMessage(&Msg);
		if(Interrupt.Function[0]) (*((_LPMSG_CALL)Interrupt.Function[0]))(&Msg);
		DispatchMessage(&Msg);
		if(Interrupt.Change)
			break;
	}
	return(Msg.wParam);
}

// Windows Message Handling Procedure (Release)
_WPROC inkWndProc(_MSG_PARAMS)
{
	int		(*Intrpt)(_MSG_PROTO_PARAMS);

	if(Msg < _INTERRUPT_SIZE)
	{
		Intrpt = Interrupt.List[Msg];
		if(Intrpt) return((*Intrpt)(_MSG_CALL_PARAMS));
		return(DefWindowProc(_MSG_CALL_PARAMS));
	}
	return(DefWindowProc(_MSG_CALL_PARAMS));
}

// Select the Interrupt Mode (use the flags) (Release)
DLL_API BOOL __cdecl inkSelectInterruptMode(UINT Type)
{
	if(Type < 8)
	{
		if((Interrupt.Type & _INTERRUPT_NOWAIT_MODE) != (Type & _INTERRUPT_NOWAIT_MODE)) Interrupt.Change = TRUE;
		Interrupt.Type = Type;
		return(TRUE);
	}
	return(FALSE);
}

// Sets the functions to be called when there is a msg and when there are no msgs
DLL_API BOOL __cdecl inkSetCallFunctions(void (*msg_func)(MSG*), void (*nomsg_func)())
{
	Interrupt.Function[0] = msg_func;
	Interrupt.Function[1] = nomsg_func;
	return(TRUE);
}

// Set the Interrupt Accelerator table for menus
DLL_API BOOL __cdecl inkSetInterruptAccelertorTable(HACCEL hAccelTable)
{
	Interrupt.hAccel = hAccelTable;
	return(TRUE);
}

// Gives the current Interrupt Function for a given Message
DLL_API void* __cdecl inkGetInterruptFunction(UINT Msg)
{
	if(Msg < Interrupt.Size)
		return(Interrupt.List[Msg]);
	return(NULL);
}

// Sets the Interrupt Function for a given message
DLL_API BOOL __cdecl inkSetInterruptFunction(UINT Msg, int (*func)(_MSG_PROTO_PARAMS))
{
	if(Msg < Interrupt.Size)
	{
		Interrupt.List[Msg] = func;
		return(TRUE);
	}
	return(FALSE);
}





#ifdef __cplusplus
}
#endif

#endif


