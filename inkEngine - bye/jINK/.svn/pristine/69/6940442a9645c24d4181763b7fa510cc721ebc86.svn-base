/****************************************************************************
 *                                                                          *
 * File    : string.cpp                                                     *
 *                                                                          *
 * Purpose : Handling of some string functions for C++                      *
 *                                                                          *
 * History : Date      		Part of INK Game Engine                         *
 *           14/07/2011		Subhajit Sahu                                   *
 *                                                                          *
 ****************************************************************************/

#ifndef		_string_cpp_

#define		_string_cpp_		1



// ----Includes------------------------------------------------------
#include "win32.h"




// ----Code----------------------------------------------------------




/*
Class:	inkString

A set of string management functions
*/
class inkString
{
	// String Management Functions Declarations
public:	
	static TCHAR*	strcpy_alloc(TCHAR* &dest, TCHAR* src);
	static TCHAR*	strconv_CharToTchar(TCHAR* dest, char* src);
	static TCHAR*	strconv_WcharToTchar(TCHAR* dest, WCHAR* src);
	static char*	strconv_TcharToChar(char* dest, TCHAR* src);
	static WCHAR*	strconv_TcharToWchar(WCHAR* dest, TCHAR* src);
};

// Allocate Memory at dest, then copy string from src to dest
TCHAR* inkString::strcpy_alloc(TCHAR* &dest, TCHAR* src)
{
	UINT len;
	len = lstrlen(src);
	dest = (TCHAR*)GlobalAlloc(GMEM_FIXED, len*sizeof(TCHAR));
	lstrcpy(dest, src);
	return(dest);
}

// Convert a string from char* to TCHAR*
TCHAR* inkString::strconv_CharToTchar(TCHAR* dest, char* src)
{
	#ifdef UNICODE
		while(*src)
		{
			*dest = (TCHAR)src;
			src++;
			dest++;
		}
	#else
	lstrcpyA(dest, src);
	#endif
	return(dest);
}

// Convert a string from WCHAR* to TCHAR*
TCHAR* inkString::strconv_WcharToTchar(TCHAR* dest, WCHAR* src)
{
	#ifndef UNICODE
		while(*src)
		{
			*dest = (TCHAR)src;
			src++;
			dest++;
		}
	#else
		lstrcpy(dest, src);
	#endif
	return(dest);
}

// Convert a string from TCHAR* to char*
char* inkString::strconv_TcharToChar(char* dest, TCHAR* src)
{
	#ifdef UNICODE
		while(*src)
		{
			*dest = (char)src;
			src++;
			dest++;
		}
	#else
		lstrcpy(dest, src);
	#endif
	return(dest);
}

// Convert a string from TCHAR* to WCHAR*
WCHAR* inkString::strconv_TcharToWchar(WCHAR* dest, TCHAR* src)
{
	#ifndef UNICODE
		while(*src)
		{
			*dest = (WCHAR)src;
			src++;
			dest++;
		}
	#else
		lstrcpy(dest, src);
	#endif
	return(dest);
}




#endif


