/****************************************************************************
 *                                                                          *
 * File    : stringGuy.cpp                                                  *
 *                                                                          *
 * Purpose : String functions for inkEngine.                                *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           03/2/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_stringGuy_cpp_
#define	_stringGuy_cpp_		1
 
#include "stringGuy.h"


// Export with no name mangling
#ifdef __cplusplus
extern "C" {
#endif

// Copy a String after allocating memory for it (W, Release)
DLL_API WCHAR* inkStrCpyAllocW(WCHAR* src)
{
	WCHAR*		ret;
	ret = (WCHAR*)inkMemAlloc(inkStrSizeW(src));
	inkStrCpyW(ret, src);
	return(ret);
}

// Copy a String after allocating memory for it (A, Release)
DLL_API char* inkStrCpyAllocA(char* src)
{
	char*		ret;
	ret = (char*)inkMemAlloc(inkStrSizeA(src));
	inkStrCpyA(ret, src);
	return(ret);
}

#ifdef __cplusplus
}
#endif

#endif


