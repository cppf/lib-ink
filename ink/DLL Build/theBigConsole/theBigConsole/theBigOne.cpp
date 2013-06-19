/****************************************************************************
 *                                                                          *
 * File    : theBigOne.cpp                                                  *
 *                                                                          *
 * Purpose : Defines the exported functions for the DLL application.        *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           03/2/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/
#include "stdafx.h"
#include "theBigOne.h"
#include "win32.h"
#include "macro.h"
#include "threadGuy.h"
#include "errorGuy.h"
#include "interruptGuy.h"

// Export with no name mangling
#ifdef __cplusplus
extern "C" {
#endif


int inkStartService(void);


TCHAR	inkTextBuffer[1024];


ENTRY WinMain(_MAIN_PARAMS)
{
	return(inkStartService());
}

int inkStartService(void)
{
	int				ret;
	HANDLE			this_process;
	HANDLE			this_thread;
	THREAD_HANDLE	msg_thread;

	this_process = GetCurrentProcess();
	this_thread = GetCurrentThread();
	DuplicateHandle(this_process, this_thread, this_process, &this_thread, 0, TRUE, DUPLICATE_SAME_ACCESS);
	inkStartErrorService(_T("MyLog.log"));
	msg_thread = inkStartThreadService(this_thread, 0, 1024, 32);
	ret = inkStartInterruptService(1024, _INTERRUPT_TEXT_MODE);
	inkStopThreadService(msg_thread, 0, 100);
	inkStopErrorService();
	return(ret);
}

#ifdef __cplusplus
}
#endif

