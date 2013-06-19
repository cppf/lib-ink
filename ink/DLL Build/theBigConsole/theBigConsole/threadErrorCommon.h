/****************************************************************************
 *                                                                          *
 * File    : threadErrorCommon.h                                            *
 *                                                                          *
 * Purpose : The common requirements for thread and error module.           *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           31/1/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


 #ifndef	_threadErrorCommon_h_
 #define	_threadErrorCommon_h_		1

#include "macro.h"
#include "memoryGuy.h"



// Export with no name mangling
#ifdef __cplusplus
extern "C" {
#endif



// Common Structures
typedef struct _ERROR_DATA
{
	UINT	Code;
	TCHAR*	Location;
	UINT	DetailType;
	TCHAR*	Detail;
}ERROR_DATA;

typedef struct _ERROR_QUEUE
{
	_DATA_QUEUE(ERROR_DATA*)
}ERROR_QUEUE;

typedef struct _LOG_QUEUE
{
	_DATA_QUEUE(UINT);
}LOG_QUEUE;

typedef struct _ERROR_STORAGE
{
	ERROR_QUEUE		Queue;
	HANDLE			File;
	void*			Handler;
}ERROR_STORAGE;

typedef struct _LOG_STORAGE
{
	LOG_QUEUE		Queue;
	HANDLE			File;
	void*			Handler;
}LOG_STORAGE;

typedef struct _ERROR_PROP
{
	UINT				Flags;
	ERROR_STORAGE		Error;
	LOG_STORAGE			Log;
}ERROR_PROP;

typedef struct _MSG_QUEUE
{
	_DATA_QUEUE(MSG_DATA*)
}MSG_QUEUE;

typedef struct _THREAD_PROP
{
	HANDLE		Handle;
	UINT		State;
	MSG_QUEUE	Msg;
	ERROR_PROP*	Error;
}THREAD_PROP;

#define	THREAD_HANDLE				THREAD_PROP*




#ifdef __cplusplus
}
#endif

#endif


