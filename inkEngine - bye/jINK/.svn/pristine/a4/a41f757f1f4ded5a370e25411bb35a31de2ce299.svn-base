/****************************************************************************
 *                                                                          *
 * File    : thread.cpp                                                     *
 *                                                                          *
 * Purpose : Thread Management & Error Handling Routines for C++            *
 *                                                                          *
 * History : Date       Part of INK Game Engine                             *
 *           5/12/2011  Subhajit Sahu                                       *
 *                                                                          *
 ****************************************************************************/



#ifndef	_thread_cpp_

#define	_thread_cpp_		1




// ----Includes------------------------------------------------------
#include "win32.h"



// ----Constants-----------------------------------------------------
#define		_tACTIVE				0								// Thread will start running as soon as it is created			
#define		_tSUSPENDED				CREATE_SUSPENDED				// Thread wont start running directly
#define		_tMESSAGEQUEUESIZE		4096							// Allocate 4KB space for each Thread's Message Queue (256 messages limit)
#define		_tMESSAGEQUEUEEND		1024							// Number of Max UINTs
#define		_tMESSAGEQUEUENUMMAX	256								// Number of Max Messages



// ----Externals-----------------------------------------------------





// ----Code----------------------------------------------------------


/*
Class:	inkThread

Objects of this class must be created to be able to use the threading functions.
However, first of all, inkThread::StartService() must be called before any other
function is used or object created.
At the end of the game, inkThread::StopService() must be called before exit.
*/
class inkThread
{
	// Global/Static Data
	private:
	static inkThread*	MainThread;						// The Main Thread of the Game
	static inkThread*	ErrorHelperThread;				// Error Helper's Thread Object
	static inkThread*	ErrorHelperData;				// Error Helper's Thread Data
	public:
	static BOOL			ErrorBox;						// Display the Error Box
	static UINT			ErrorBeep;						// Produce Beep Sound on Error
	static UINT			StackSize;						// Stack Size of Threads being created
	static UINT			NumThreads;						// Number of Threads created.

	// Local/Instance Data
	private:
	BOOL		Running;						// Is the Thread still Running(not dead) ?
	BOOL		Active;							// Is the Thread Active(not suspended) ?
	void*		ErrorHandler;					// Error Handler's address for the thread
	UINT*		MessageQueue;					// Message Queue for the thread
	UINT		MessageQueueFront;				// Message Queue's Front Pointer
	UINT		MessageQueueRear;				// Message Queue's Rear Pointer
	UINT		MessageQueueNum;				// Message Queue's Number of messages
	UINT		ErrorCode;						// Error Code of the error that ocurred in the thread
	TCHAR*		ErrorString;					// Error String of the error that ocurred in the thread
	UINT		ErrorAction;					// Action to take as a response to the error
	public:
	HANDLE		Handle;							// Thread Handle
	DWORD		ID;								// Thread ID
	TCHAR*		Description;					// Thread Description / name


	// Thread Management & Error Handling Functions Declarations
	public:
	static void			StartService(void* err_handler);
	static void			StopService(void);
	static inkThread*	CreateMain(TCHAR* thread_descrip, void* func, void* param, DWORD start, void* err_handler);
						inkThread(TCHAR* thread_descrip, void* func, void* param, DWORD start, void* err_handler);
						~inkThread(UINT exit_code);
	void				GiveMessage(UINT msg_type, UINT msg_data1, UINT msg_data2, UINT msg_data3);
	void				RecieveMessage(UINT *msg_type, UINT *msg_data1, UINT *msg_data2, UINT *msg_data3);
	void				Stop(UINT exit_code);
	static void			WaitTillActive(inkThread* thread);
	static void			WaitTillSuspended(inkThread* thread);
	void				Resume(void);
	void				Suspend(void);
	UINT				GetExitCode(void);
	void				Error(UINT err_code);
	void				ErrorStr(char* err_str);
	void				ErrorAct(UINT err_code, UINT act);
	void				ErrorStrAct(char* err_str, UINT act);
	private:
	static void			ErrorTransfer(inkThread *thread_obj);
	static void			ErrorHelper(void *no_use);
};
inkThread*		MainThread;						// The Main Thread of the Game
inkThread*		ErrorHelperThread;				// Error Helper's Thread Object
inkThread*		ErrorHelperData;				// Error Helper's Thread Data
BOOL			ErrorBox;						// Display the Error Box
UINT			ErrorBeep;						// Produce Beep Sound on Error
UINT			StackSize;						// Stack Size of Threads being created
UINT			NumThreads;						// Number of Threads created.



// Start Thread Service
void inkThread::StartService(void* err_handler)
{
	MainThread = NULL;
	ErrorHelperData = NULL;
	ErrorBox = FALSE;
	ErrorBeep = 0;
	NumThreads = 0;
	StackSize = 4096;
	ErrorHelperThread = new inkThread("Error Helper", ErrorHelper, NULL, _tSUSPENDED, NULL);
	return(0);
}


// Stop Thread Service
void inkThread::StopService(void)
{
	ErrorHelperExit = TRUE;
	WaitTillSuspended(ErrorHelperThread);
	ErrorHelperThread->Resume();
	WaitTillSuspended(ErrorHelperThread);
	delete ErrorHelperThread;
	return(0);
}


// Create the Main Thread
inkThread* inkThread::CreateMain(TCHAR* thread_descrip, void* func, void* param, DWORD start, void* err_handler)
{
	inkThread *TheMain = new inkThread(thread_descrip, func, param, start, err_handler);
	inkThread::MainThread = TheMain;
	return(TheMain);
}


// Create a new Thread
inkThread::inkThread(TCHAR* thread_descrip, void* func, void* param, DWORD start, void* err_handler)
{
	void (*MainErrorHandler)(void*);
	Running = FALSE;
	Active = FALSE;
	Handle = (HANDLE)0;
	ID = 0;
	Description = thread_descrip;														// Save the Thread's description
	ErrorHandler = err_handler;															// Save the Error Handler for the Thread
	if(err_handler == NULL) ErrorHandler = MainThread->ErrorHandler;					// Not given, set it to the Main Thread's Error handler
	MessageQueue = (UINT*) GlobalAlloc(GMEM_FIXED, _tMESSAGEQUEUESIZE);					// Allocate memory for Message Queue
	if(MessageQueue = NULL)																// Falied ? , inform this error to Main Thread
	{
		ErrorCode = 0;
		ErrorString = NULL;
		ErrorAct = 0;
		MainErrorHandler = MainThread->ErrorHandler;
		*MainErrorHandler((void*)this);
		return;
	}
	MessageQueueNum = 0;
	MessageQueueFront = 0;
	MessageQueueRear = 0;
	NumThreads++;
	Running = TRUE;
	if(start == _tACTIVE) Active = TRUE;
	Handle = CreateThread(NULL, StackSize, (LPTHREAD_START_ROUTINE)func, param, start, &ID);
}


// Delete an existing Thread (from another thread) (try not to use this unless really req.)
inkThread::~inkThread(UINT exit_code)
{
	if(Running == TRUE)	
	{
		NumThreads--;
		Running = FALSE;
		Active = FALSE;
		TerminateThread(Handle, exit_code);
	}
}


// Give a Message to a thread (susecptible to multithreading deadlocks)
void inkThread::GiveMessage(UINT msg_type, UINT msg_data1, UINT msg_data2, UINT msg_data3)
{
	if(MessageQueueNum < _tMESSAGEQUEUENUMMAX)
	{
		MessageQueueNum++;
		MessageQueue[MessageQueueRear] = msg_type;
		MessageQueueRear++;
		MessageQueue[MessageQueueRear] = msg_data1;
		MessageQueueRear++;
		MessageQueue[MessageQueueRear] = msg_data2;
		MessageQueueRear++;
		MessageQueue[MessageQueueRear] = msg_data3;
		MessageQueueRear++;
		if(MessageQueueRear >= _tMESSAGEQUEUEEND) MessageQueueRear = 0;
	}
}


// Give a Message to a thread (susecptible to multithreading deadlocks)
void inkThread::RecieveMessage(UINT *msg_type, UINT *msg_data1, UINT *msg_data2, UINT *msg_data3)
{
	if(MessageQueueNum > 0)
	{
		MessageQueueNum--;
		*msg_type = MessageQueue[MessageQueueFront];
		MessageQueueFront++;
		*msg_data1 = MessageQueue[MessageQueueFront];
		MessageQueueFront++;
		*msg_data2 = MessageQueue[MessageQueueFront];
		MessageQueueFront++;
		*msg_data3 = MessageQueue[MessageQueueFront];
		MessageQueueFront++;
		if(MessageQueueFront >= _tMESSAGEQUEUEEND) MessageQueueFront = 0;
	}
}


// Stop the current thread (from same thread) (best way to stop a thread)
void inkThread::Stop(UINT exit_code)
{
	NumThreads--;
	Running = FALSE;
	Active = FALSE;
	ExitThread(exit_code);
}


// Wait till the given thread becomes active
void inkThread::WaitTillActive(inkThread* thread)
{
	while(thread->Active == FALSE);
}


// Wait till the given thread becomes suspended
void inkThread::WaitTillSuspended(inkThread* thread)
{
	while(thread->Active == TRUE);
}


// Resume a Thread
void inkThread::Resume(void)
{
	if(Active == FALSE)
	{
		Active = TRUE;
		ResumeThread(Handle);
	}
}


// Suspend a Thread
void inkThread::Suspend(void)
{
	if(Active == TRUE)
	{
		Active = FALSE;
		SuspendThread(Handle);
	}
}


// Get the Exit Code of the Thread
UINT inkThread::GetExitCode(void)
{
	UINT exit_code = 0;
	if(Running = FALSE) GetExitCodeThread(ID, &exit_code);
	return(exit_code);
}


// Report Error Code to the Error Handler
void inkThread::Error(UINT err_code)
{
	ErrorCode = err_code;
	ErrorTransfer(this);
}


// Report Error String to the Error Handler
void inkThread::ErrorStr(char* err_str)
{
	ErrorString = err_str;
	ErrorTransfer(this);
}


// Report Error Code and Suggested Action to Error Handler
void inkThread::ErrorAct(UINT err_code, UINT act)
{
	ErrorCode = err_code;
	ErrorAction = act;
	ErrorTransfer(this);
}


// Report Error String and Suggested Action to Error Handler
void inkThread::ErrorStrAct(char* err_str, UINT act)
{
	ErrorString = err_str;
	ErrorAction = act;
	ErrorTransfer(this);
}


void inkThread::ErrorTransfer(inkThread *thread_obj)
{
	WaitTillSuspended(ErrorHelperThread);
	ErrorHelperData = thread_obj;
	ErrorHelperThread->Resume();
	WaitTillSuspended(ErrorHelperThread);
}


void inkThread::ErrorHelper(void *no_use)
{
	UNREFERENCED_PARAMETER(no_use);
	TCHAR *ErrorDisplay;
	void (*error_handler)(inkThread*);
	
	while(1)
	{
		if(ErrorHelperExit == TRUE) ErrorHelperThread->Stop(0);
		if(ErrorHelperData->ErrorCode == 0) ErrorHelperData->ErrorCode = GetLastError();
		if(ErrorHelperData->ErrorString == NULL) ErrorHelperData->ErrorString = _T("");
		//if(ErrorHelperData->ErrorStr == NULL) readstr from file
		if(ErrorBox == TRUE)
		{
			ErrorDisplay = (TCHAR*) GlobalAlloc(GMEM_FIXED, 512);
			wsprintf(ErrorDisplay, "Error Code: %d\nError String: %s\nError Action: %d\nThread ID: %d\nThread Description: %s\n", 
			ErrorHelperData->ErrorCode, ErrorHelperData->ErrorString, ErrorHelperData->ErrorAction,	ErrorHelperData->ID, ErrorHelperData->Description);
			MessageBox(NULL, ErrorDisplay, _T("Internal Error"), MB_OK|MB_ICONEXCLAMATION);
			GlobalFree(ErrorDisplay);
		}
		if(ErrorBeep != 0) MessageBeep(ErrorBeep);
		error_handler = ErrorHelperData->ErrorHandler;
		*error_handler(ErrorHelperData);
		ErrorHelperThread->Suspend();
	}
}



#endif

