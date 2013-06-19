/****************************************************************************
 *                                                                          *
 * File    : interruptGuy.h                                                 *
 *                                                                          *
 * Purpose : The functions for inkEngine related to message handling.       *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           31/1/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


 #ifndef	_interruptGuy_h_
 #define	_interruptGuy_h_		1
 
#include "stdafx.h"
#include "theBigOne.h"
#include "win32.h"
#include "macro.h"
#include "threadGuy.h"


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
	THREAD_HANDLE		Thread;
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
extern INTERRUPTS	Interrupt;



// Function Declarations
THREAD_HANDLE __cdecl inkStartInterruptService(UINT inkInterruptsListSize, UINT Type, UINT wnd_stack);
void __cdecl inkStopInterruptService(void);
void __cdecl inkWaiter(void* no_use);
int __cdecl inkWaitHandler(void);
int __cdecl inkNoWaitHandler(void);
_WPROC inkWndProc(_MSG_PARAMS);
DLL_API BOOL __cdecl inkSelectInterruptMode(UINT Type);
DLL_API BOOL __cdecl inkSetCallFunctions(void (*msg_func)(MSG*), void (*nomsg_func)());
DLL_API BOOL __cdecl inkSetInterruptAccelertorTable(HACCEL hAccelTable);
DLL_API void* __cdecl inkGetInterruptFunction(UINT Msg);
DLL_API BOOL __cdecl inkSetInterruptFunction(UINT Msg, int (*func)(_MSG_PROTO_PARAMS));




#ifdef __cplusplus
}
#endif

#endif


