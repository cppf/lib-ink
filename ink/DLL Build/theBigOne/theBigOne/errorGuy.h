/****************************************************************************
 *                                                                          *
 * File    : errorGuy.h                                                     *
 *                                                                          *
 * Purpose : The functions for inkEngine related to error handling.         *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           31/1/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


 #ifndef	_errorGuy_h_
 #define	_errorGuy_h_		1

#include "stdafx.h"
#include "theBigOne.h"
#include "win32.h"
#include "macro.h"
#include "memoryGuy.h"
#include "stringGuy.h"
#include "threadErrorCommon.h"


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
#define	_ERROR_CONTINUEONERROR			1
#define	_ERROR_NOTIFYERRORHANDLER		2
#define	_ERROR_NOTIFYTHREAD				4


// seterrorflags()
// saveerror()




// Function Declarations
void __cdecl inkStartErrorService(TCHAR* errfile);
void __cdecl inkStopErrorService(void);
DLL_API BOOL __cdecl inkLogString(TCHAR* str);



#ifdef __cplusplus
}
#endif

#endif


