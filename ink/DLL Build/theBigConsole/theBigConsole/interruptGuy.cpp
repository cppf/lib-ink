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
 
 #include "interruptGuy.h"


// Export with no name mangling
#ifdef __cplusplus
extern "C" {
#endif




// Data
INTERRUPTS	inkInterruptsList;






// Starts the inkInterruptsList Service (Release)
THREAD_HANDLE __cdecl inkStartInterruptService(UINT inkInterruptsListSize, UINT Type, UINT wnd_stack)
{
	HANDLE	ret;

	inkInterruptsList.Size = inkInterruptsListSize;
	inkInterruptsList.hAccel = (HACCEL)0;
	inkInterruptsList.Type = Type;
	inkInterruptsList.List = (_INTERRUPT)inkMemInitAlloc(sizeof(void*) * inkInterruptsList.Size);
	if(wnd_stack)return((inkInterruptsList.Thread = inkCreateThread(inkWaiter, NULL, 0, wnd_stack, 0)));
	return(NULL);
}

// Stops the inkInterruptsList Service (Release)
void __cdecl inkStopInterruptService(int nExitCode)
{
	DWORD	xt;
	PostQuitMessage(nExitCode);
	GetExitCodeThread(inkInterruptsList.Thread, &xt);
	while(xt == STILL_ACTIVE)
	{
		SwitchToThread();
		GetExitCodeThread(inkInterruptsList.Thread, &xt);
	}
	inkCloseThread(inkInterruptsList.Thread);
	inkMemFree(inkInterruptsList.List);
}

// Helps switch between Wait and NoWait inkInterruptsList mode (Release)
void __cdecl inkWaiter(void* no_use)
{
	UNREFERENCED_PARAMETER(no_use);
	int		ret;

	inkInterruptsList.Change = FALSE;
	while(1)
	{
		if(!(inkInterruptsList.Type & _INTERRUPT_NOWAIT_MODE)) ret = inkWaitHandler();
		else ret = inkNoWaitHandler();
		if(!inkInterruptsList.Change)break;
		inkInterruptsList.Change = FALSE;
	}
}

// Windows Message Handler that waits for messages (Release)
int __cdecl inkWaitHandler(void)
{
	MSG		Msg;

	while(GetMessage(&Msg, NULL, 0, 0))
	{
		if(inkInterruptsList.hAccel) TranslateAccelerator(Msg.hwnd, inkInterruptsList.hAccel, &Msg);
		if(inkInterruptsList.Type & _INTERRUPT_TEXT_MODE) TranslateMessage(&Msg);
		if(inkInterruptsList.Function[0]) (*((_LPMSG_CALL)inkInterruptsList.Function[0]))(&Msg);
		DispatchMessage(&Msg);
		if(inkInterruptsList.Change)
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
			(*((_VOID_CALL)inkInterruptsList.Function[1]))();
		}
		if(Msg.message == WM_QUIT) break;
		if(inkInterruptsList.hAccel) TranslateAccelerator(Msg.hwnd, inkInterruptsList.hAccel, &Msg);
		if(inkInterruptsList.Type & _INTERRUPT_TEXT_MODE) TranslateMessage(&Msg);
		if(inkInterruptsList.Function[0]) (*((_LPMSG_CALL)inkInterruptsList.Function[0]))(&Msg);
		DispatchMessage(&Msg);
		if(inkInterruptsList.Change)
			break;
	}
	return(Msg.wParam);
}

// Windows Message Handling Procedure (Release)
_WPROC inkWndProc(_MSG_PARAMS)
{
	int		(*Intrpt)(_MSG_PROTO_PARAMS);

	if(Msg < inkInterruptsList.Size)
	{
		Intrpt = inkInterruptsList.List[Msg];
		if(Intrpt) return((*Intrpt)(_MSG_CALL_PARAMS));
		return(DefWindowProc(_MSG_CALL_PARAMS));
	}
	return(DefWindowProc(_MSG_CALL_PARAMS));
}

// Select the inkInterruptsList Mode (use the flags) (Release)
DLL_API BOOL __cdecl inkSelectinkInterruptsListMode(UINT Type)
{
	if(Type < 8)
	{
		if((inkInterruptsList.Type & _INTERRUPT_NOWAIT_MODE) != (Type & _INTERRUPT_NOWAIT_MODE)) inkInterruptsList.Change = TRUE;
		inkInterruptsList.Type = Type;
		return(TRUE);
	}
	return(FALSE);
}

// Sets the functions to be called when there is a msg and when there are no msgs
DLL_API BOOL __cdecl inkSetCallFunctions(void (*msg_func)(MSG*), void (*nomsg_func)())
{
	inkInterruptsList.Function[0] = msg_func;
	inkInterruptsList.Function[1] = nomsg_func;
	return(TRUE);
}

// Set the inkInterruptsList Accelerator table for menus
DLL_API BOOL __cdecl inkSetinkInterruptsListAccelertorTable(HACCEL hAccelTable)
{
	inkInterruptsList.hAccel = hAccelTable;
	return(TRUE);
}

// Gives the current inkInterruptsList Function for a given Message
DLL_API void* __cdecl inkGetinkInterruptsListFunction(UINT Msg)
{
	if(Msg < inkInterruptsList.Size)
		return(inkInterruptsList.List[Msg]);
	return(NULL);
}

// Sets the inkInterruptsList Function for a given message
DLL_API BOOL __cdecl inkSetinkInterruptsListFunction(UINT Msg, int (*func)(_MSG_PROTO_PARAMS))
{
	if(Msg < inkInterruptsList.Size)
	{
		inkInterruptsList.List[Msg] = func;
		return(TRUE);
	}
	return(FALSE);
}





#ifdef __cplusplus
}
#endif

#endif


