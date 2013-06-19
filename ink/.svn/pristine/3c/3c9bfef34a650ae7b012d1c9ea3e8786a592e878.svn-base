/****************************************************************************
 *                                                                          *
 * File    : errorGuy.cpp                                                   *
 *                                                                          *
 * Purpose : The functions for inkEngine related to error handling.         *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           31/1/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


 #ifndef	_errorGuy_cpp_
 #define	_errorGuy_cpp_		1

#include "errorGuy.h"


// Export with no name mangling
#ifdef __cplusplus
extern "C" {
#endif



// Some Internal Definitions




// Structures

// What the flags mean?
// wait for error handler to complete / let program continue on error
// do not call error handler / call error handler
// do not pass msg to thread / pass msg to thread




// a pointer to this structure shall be in create thread
// this structure will be created only if error handling is required

// Flags
/*
#define	_ERROR_CONTINUEONERROR			1
#define	_ERROR_NOTIFYERRORHANDLER		2
#define	_ERROR_NOTIFYTHREAD				4
*/

// seterrorflags()
// saveerror()


// Data


// Function Declarations
HANDLE	hFile;



// Starts the Interrupt Service (Release)
void __cdecl inkStartErrorService(TCHAR* errfile)
{
	hFile = CreateFile(errfile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}
/*
int __cdecl inkStartErrorService(UINT errqsize, UINT logqsize, HANDLE errfile, HANDLE logfile, void* globalerrhandler)
{
	ErrorQueue.Size = errqsize + 1;
	ErrorQueue.Number = 0;
	ErrorQueue.Front = 0;
	ErrorQueue.Rear = 0;
	ErrorQueue.Data = (ERROR_DATA*)inkMemAlloc(ErrorQueue.Size * sizeof(ERROR_DATA));
	LogQueue.Size = logqsize + 1;
	LogQueue.
}
*/



// Stops the Interrupt Service (Release)
void __cdecl inkStopErrorService(void)
{
	CloseHandle(hFile);
}

// Sets the Error flags
/*
DLL_API BOOL __cdecl inkSetErrorFlags(THREAD_HANDLE thread, UINT flags)
{
	if(thread->Error)
	{
		thread->Error->Flags = flags;
		return(TRUE);
	}
	return(FALSE);
}
*/

DLL_API BOOL __cdecl inkLogString(TCHAR* str)
{
	DWORD	x;
	WriteFile(hFile, str, _TS(lstrlen(str)), &x, NULL);
	return(TRUE);
}


/*
DLL_API BOOL __cdecl inkLogString(THREAD_HANDLE thread, TCHAR* str)
{
	if(thread->Error)			// is error handling enabled for that thread?
	{
		if(thread->Error->Log.Queue.Data)		// is Log enabled?
		{
			_inkInsertQueue(
			thread->Error->Log.Queue.Data = (UINT)inkStrCpyAllocW(str);
			thread->
		}
	}
	return(FALSE);
}


//_Check_return_opt_ _CRTIMP int __cdecl fprintf(_Inout_ FILE * _File, _In_z_ _Printf_format_string_ const char * _Format, ...);
DLL_API BOOL _cdecl inkLog(TCHAR* , ...)

// Save the Log in the Log queue, to be saved to the Log file later (Release)
DLL_API BOOL __cdecl inkLogString(TCHAR*)
{
	// put log string into the log queue
	// if queue becomes full, then fush log to file
}
*/

#ifdef __cplusplus
}
#endif


#endif


