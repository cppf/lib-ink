/****************************************************************************
 *                                                                          *
 * File    : threadGuy.h                                                    *
 *                                                                          *
 * Purpose : Defines the functions for inkEngine related to threading.      *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           31/1/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_threadGuy_h_
#define	_threadGuy_h_		1
 
#include "stdafx.h"
#include "theBigOne.h"
#include "win32.h"
#include "macro.h"
#include "threadErrorCommon.h"
#include "errorGuy.h"


// Export with no name mangling
#ifdef __cplusplus
extern "C" {
#endif



// Structures
typedef struct _THREAD_LIST
{
	_DATA_LIST(THREAD_HANDLE*)
}THREAD_LIST;




// Thread States
#define	_THREAD_CLOSED				0
#define	_THREAD_STOPPED				1
#define	_THREAD_SUSPENDED			2
#define	_THREAD_ACTIVE				3
#define	_THREAD_USEMSGQUEUE			4
#define	_THREAD_CRITICALJOB			5

// Thread Messages
#define	_TM_QUIT					0x0000
#define	_TM_STOP					0x0001
#define	_TM_ERROR					0x0002


// Global Data
extern THREAD_LIST		ThreadList;



// Function Declarations
THREAD_HANDLE __cdecl inkStartThreadService(HANDLE calling_thread, UINT State, UINT MsgQueueSize, UINT ListBuffering);
void __cdecl inkStopThreadService(THREAD_HANDLE calling_thread, DWORD dwExitCode, UINT timeout);
DLL_API UINT __cdecl inkNumberOfThreads(void);
DLL_API THREAD_HANDLE __cdecl inkAddThread(HANDLE Handle, UINT State, UINT MsgQueueSize);
DLL_API HANDLE __cdecl inkRemoveThread(THREAD_HANDLE thread);
DLL_API THREAD_HANDLE __cdecl inkCreateThread(void* lpStartRoutine, void* lpParameter, DWORD dwCreationFlags, UINT StackSize, UINT MsgQueueSize);
DLL_API HANDLE __cdecl inkGetThreadHandle(THREAD_HANDLE thread);
DLL_API UINT __cdecl inkGetThreadState(THREAD_HANDLE thread);
DLL_API int __cdecl inkGetThreadPriority(THREAD_HANDLE thread);
DLL_API BOOL __cdecl inkSetThreadPriority(THREAD_HANDLE thread, int nPriority);
DLL_API BOOL __cdecl inkSuspendThread(THREAD_HANDLE thread);
DLL_API BOOL __cdecl inkResumeThread(THREAD_HANDLE thread);
DLL_API BOOL __cdecl inkStopThreadB(THREAD_HANDLE thread, DWORD dwExitCode, UINT timeout);
DLL_API BOOL __cdecl inkStopThreadN(THREAD_HANDLE thread, DWORD dwExitCode);
DLL_API BOOL __cdecl inkTerminateThreadB(THREAD_HANDLE thread, DWORD dwExitCode, UINT timeout);
DLL_API BOOL __cdecl inkTerminateThreadN(THREAD_HANDLE thread, DWORD dwExitCode);
DLL_API BOOL __cdecl inkCloseThread(THREAD_HANDLE thread);
DLL_API BOOL __cdecl inkPostMessage(THREAD_HANDLE thread, MSG_DATA* data);
DLL_API BOOL __cdecl inkSendMessage(THREAD_HANDLE thread, MSG_DATA* data);


#ifdef __cplusplus
}
#endif

#endif


