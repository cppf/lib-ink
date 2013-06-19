/****************************************************************************
 *                                                                          *
 * File    : threadGuy.cpp                                                  *
 *                                                                          *
 * Purpose : Defines the functions for inkEngine related to threading.      *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           31/1/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_threadGuy_cpp_
#define	_threadGuy_cpp_		1
 
#include "threadGuy.h"


// Export with no name mangling
#ifdef __cplusplus
extern "C" {
#endif


// Global Data
THREAD_LIST		inkThreadsList;



// Starts the Threading Service (Release)
THREAD_HANDLE __cdecl inkStartThreadService(HANDLE calling_thread, UINT State, UINT MsgQueueSize, UINT ListBuffering)
{
	// Create inkThreadsList
	inkThreadsList.Buffer = ListBuffering;
	inkThreadsList.Number = 0;
	inkThreadsList.Size = 1 + inkThreadsList.Buffer;
	inkThreadsList.Data = (THREAD_HANDLE*)inkMemAlloc(sizeof(THREAD_HANDLE*) * inkThreadsList.Size);

	// Add the Calling Thread
	if(calling_thread)return(inkAddThread(calling_thread, State, MsgQueueSize));
	return(NULL);
}

// Stop the Threading Service (Release)
void __cdecl inkStopThreadService(THREAD_HANDLE calling_thread, DWORD dwExitCode, UINT timeout)
{
	UINT		i;
	DWORD		ret;
	MSG_DATA	msg;

	// Create _TM_QUIT message to signal the threads that threading
	// service (also, the game engine) is about to stop. The threads
	// must now stop by through a return statement. After the given
	// Timeout, the threads will be forcibly stopped.
	msg.Type = _TM_QUIT;								// Thread Message - Quit
	msg.Param[0] = (UINT)dwExitCode;					// Error Code in Param[0]
	msg.Param[1] = timeout;								// Timeout in milliseconds in Param[1]
	msg.Param[2] = 0;

	// Remove the calling thread (it must not be stopped)
	if(calling_thread)inkRemoveThread(calling_thread);

	// Post _TM_QUIT message to all other threads
	for(i=0; i<inkThreadsList.Number; i++)
		inkPostMessage(inkThreadsList.Data[i], &msg);

	// Wait for Timeout milliseconds
	Sleep(timeout);

	// Forcibly Stop still running threads
	for(i=0; i<inkThreadsList.Number; i++)
	{
		GetExitCodeThread(inkThreadsList.Data[i]->Handle, &ret);
		if(ret == STILL_ACTIVE)
		{
			SuspendThread(inkThreadsList.Data[i]->Handle);
			TerminateThread(inkThreadsList.Data[i]->Handle, dwExitCode);
		}
	}

	// Clear out allocated memory
	for(i=0; i<inkThreadsList.Number; i++)
	{
		inkMemFree(inkThreadsList.Data[i]->Msg.Data);
		inkStructFree(inkThreadsList.Data[i]);
	}
	inkMemFree(inkThreadsList.Data);
}

// Returns the address of Threads List managed by inkEngine (Advanced, Release)
DLL_API THREAD_LIST* __cdecl inkGetThreadsList(void)
{
	return(&inkThreadsList);
}

// Gives the number of Threads existing (Release)
DLL_API UINT __cdecl inkNumberOfThreads(void)
{
	return(inkThreadsList.Number);
}

// Adds an existing Thread to this List; Then it also has a Message Queue (Release)
DLL_API THREAD_HANDLE __cdecl inkAddThread(HANDLE Handle, UINT State, UINT MsgQueueSize)
{
	THREAD_HANDLE thread;

	thread = (THREAD_HANDLE)inkStructInitAlloc(thread);
	thread->State = State;
	thread->Handle = Handle;
	if(MsgQueueSize)
	{
		thread->Msg.Size = sizeof(MSG_DATA)<<(MsgQueueSize-1);
		thread->Msg.Data = (MSG_DATA*)inkMemAlloc(thread->Msg.Size);
		thread->Msg.Size--;
		if(inkThreadsList.Number >= inkThreadsList.Size)
		{
			inkThreadsList.Size += 1 + inkThreadsList.Buffer;
			inkThreadsList.Data = (THREAD_HANDLE*)inkMemReAlloc(inkThreadsList.Data, sizeof(THREAD_HANDLE) * inkThreadsList.Size);
		}
		inkThreadsList.Data[inkThreadsList.Number] = thread;
		inkThreadsList.Number++;
	}
	return(thread);
}

// Removes a Thread from the list, and return the thread handle (Release)
DLL_API HANDLE __cdecl inkRemoveThread(THREAD_HANDLE thread)
{
	HANDLE	ret;
	UINT	i, j;

	ret = thread->Handle;
	for(i=0; i<inkThreadsList.Number; i++)
	{
		if(inkThreadsList.Data[i] == thread)
		{
			for(j=i+1; j<inkThreadsList.Number; j++)
				inkThreadsList.Data[j-1] = inkThreadsList.Data[j];
			inkThreadsList.Number--;
			break;
		}
	}
	inkMemFree(thread->Msg.Data);
	inkStructFree(thread);
	return(ret);
}

// Creates Thread with a Message Queue (Release)
DLL_API THREAD_HANDLE __cdecl inkCreateThread(void* lpStartRoutine, void* lpParameter, DWORD dwCreationFlags, UINT StackSize, UINT MsgQueueSize)
{
	THREAD_HANDLE	thread;

	thread = NULL;
	inkStructInitAlloc(thread);
	thread->State = dwCreationFlags;
	if(MsgQueueSize)
	{
		thread->Msg.Size = sizeof(MSG_DATA)<<(MsgQueueSize-1);
		thread->Msg.Data = (MSG_DATA*)inkMemAlloc(thread->Msg.Size);
		thread->Msg.Size--;
		if(inkThreadsList.Number >= inkThreadsList.Size)
		{
			inkThreadsList.Size += 1 + inkThreadsList.Buffer;
			inkThreadsList.Data = (THREAD_HANDLE*)inkMemReAlloc(inkThreadsList.Data, sizeof(THREAD_HANDLE) * inkThreadsList.Size);
		}
		inkThreadsList.Data[inkThreadsList.Number] = thread;
		inkThreadsList.Number++;
	}
	if(thread->State == _THREAD_SUSPENDED) dwCreationFlags = CREATE_SUSPENDED;
	thread->Handle = CreateThread(NULL, sizeof(UINT)*StackSize, (LPTHREAD_START_ROUTINE)lpStartRoutine, lpParameter, dwCreationFlags, NULL);
	return(thread);
}

// Gives the Handle of a Thread (Release)
DLL_API HANDLE __cdecl inkGetThreadHandle(THREAD_HANDLE thread)
{
	return(thread->Handle);
}

// Gives the State of a Thread (Release)
DLL_API UINT __cdecl inkGetThreadState(THREAD_HANDLE thread)
{
	return(thread->State);
}

// Gives the Priority of a Thread (Release)
DLL_API int __cdecl inkGetThreadPriority(THREAD_HANDLE thread)
{
	return(GetThreadPriority(thread->Handle));
}

// Sets the Priority of a Thread (Release)
DLL_API BOOL __cdecl inkSetThreadPriority(THREAD_HANDLE thread, int nPriority)
{
	return(SetThreadPriority(thread->Handle, nPriority));
}

// Set a Thread's State (Release)
DLL_API BOOL __cdecl inkSetThreadState(THREAD_HANDLE thread, UINT state)
{
	thread->State = state;
	return(TRUE);
}

// Suspends a Thread (Release)
DLL_API BOOL __cdecl inkSuspendThread(THREAD_HANDLE thread)
{
	while(thread->State >= _THREAD_CRITICALJOB)
		SwitchToThread();

	if(thread->State >= _THREAD_ACTIVE)
	{
		SuspendThread(thread->Handle);
		return(TRUE);
	}
	return(FALSE);
}

// Resumes a Thread (Release)
DLL_API BOOL __cdecl inkResumeThread(THREAD_HANDLE thread)
{
	if(thread->State == _THREAD_SUSPENDED)
	{
		ResumeThread(thread->Handle);
		return(TRUE);
	}
	return(FALSE);
}

// Stops a Thread properly (Blocking, Release)
DLL_API BOOL __cdecl inkStopThreadB(THREAD_HANDLE thread, DWORD dwExitCode, UINT timeout)
{
	MSG_DATA	data;
	UINT		count;

	count = 0;
	if(thread->State >= _THREAD_SUSPENDED)
	{
		data.Type = _TM_STOP;
		data.Param[0] = (UINT)dwExitCode;
		inkPostMessage(thread, &data);
		while(thread->State > _THREAD_STOPPED)
		{
			SwitchToThread();
			count++;
			if(count >= timeout) return(FALSE);
		}
		return(TRUE);
	}
	return(FALSE);
}

// Stops a Thread properly (Non-Blocking, Release)
DLL_API BOOL __cdecl inkStopThreadN(THREAD_HANDLE thread, DWORD dwExitCode)
{
	MSG_DATA	data;

	if(thread->State >= _THREAD_SUSPENDED)
	{
		data.Type = _TM_STOP;
		data.Param[0] = (UINT)dwExitCode;
		inkPostMessage(thread, &data);
		return(TRUE);
	}
	return(FALSE);
}

// Terminates a Thread improperly (Blocking, Release)
DLL_API BOOL __cdecl inkTerminateThreadB(THREAD_HANDLE thread, DWORD dwExitCode, UINT timeout)
{
	DWORD	exitcode;
	UINT	count;

	count = 0;
	if(thread->State >= _THREAD_SUSPENDED)
	{
		SuspendThread(thread->Handle);
		TerminateThread(thread->Handle, dwExitCode);
		do
		{
			SwitchToThread();
			GetExitCodeThread(thread->Handle, &exitcode);
			count++;
			if(count >= timeout) return(FALSE);
		}
		while(exitcode == STILL_ACTIVE);
		return(TRUE);
	}
	return(FALSE);
}

// Terminates a Thread improperly (Non-Blocking, Release)
DLL_API BOOL __cdecl inkTerminateThreadN(THREAD_HANDLE thread, DWORD dwExitCode)
{
	if(thread->State >= _THREAD_SUSPENDED)
	{
		SuspendThread(thread->Handle);
		TerminateThread(thread->Handle, dwExitCode);
		return(TRUE);
	}
	return(FALSE);
}

// Closes a Thread after it has been Stopped / Terminated (Release)
DLL_API BOOL __cdecl inkCloseThread(THREAD_HANDLE thread)
{
	UINT	i, j;

	if(thread->State <= _THREAD_STOPPED)
	{
		CloseHandle(thread->Handle);
		for(i=0; i<inkThreadsList.Number; i++)
		{
			if(inkThreadsList.Data[i] == thread)
			{
				for(j=i+1; j<inkThreadsList.Number; j++)
					inkThreadsList.Data[j-1] = inkThreadsList.Data[j];
				inkThreadsList.Number--;
				break;
			}
		}
		inkMemFree(thread->Msg.Data);
		inkStructFree(thread);
	}
	return(FALSE);
}

// Posts a message to a Thread (Release)
DLL_API BOOL __cdecl inkPostMessage(THREAD_HANDLE thread, MSG_DATA* data)
{
	if((thread->State >= _THREAD_SUSPENDED) && (thread->Msg.Number <= thread->Msg.Size))
	{
		while(thread->State == _THREAD_USEMSGQUEUE)		// may not be required (check)
			SwitchToThread();
		thread->Msg.Data->Type = data->Type;
		thread->Msg.Data->Param[0] = data->Param[0];
		thread->Msg.Data->Param[1] = data->Param[1];
		thread->Msg.Data->Param[2] = data->Param[2];
		thread->Msg.Rear = (thread->Msg.Rear + 1) & thread->Msg.Size;
		thread->Msg.Number++;
		inkResumeThread(thread);
		return(TRUE);
	}
	return(FALSE);
}

// Sends a message to a Thread and waits till the Message is Processed (Release)
DLL_API BOOL __cdecl inkSendMessage(THREAD_HANDLE thread, MSG_DATA* data)
{
	UINT	msgnum;
	BOOL	ret;

	msgnum = thread->Msg.Number;
	ret = inkPostMessage(thread, data);
	if(ret)
	{
		while((thread->Msg.Number >= msgnum) || (thread->State >= _THREAD_ACTIVE))
			SwitchToThread();
		return(TRUE);
	}
	return(FALSE);
}




#ifdef __cplusplus
}
#endif

#endif


