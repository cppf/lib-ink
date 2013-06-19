/****************************************************************************
 *                                                                          *
 * File    : stringGuy.h                                                    *
 *                                                                          *
 * Purpose : String functions for inkEngine.                                *
 *                                                                          *
 * History : Date      Part of inkEngine                                    *
 *           03/2/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


 #ifndef	_stringGuy_h_
 #define	_stringGuy_h_		1
 
#include "stdafx.h"
#include "theBigOne.h"
#include "win32.h"
#include "memoryGuy.h"

// Export with no name mangling
#ifdef __cplusplus
extern "C" {
#endif



// Some Internal Definitions
#define	_ASCII_CONVERT				CP_UTF8		// CP_OEMCP, CP_ACP






// Macros
#define	inkStrLenA(str)	\
	lstrlenA(str)

#define	inkStrLenW(str)	\
	lstrlenW(str)

#define	inkStrToLowercaseA(str)	\
	CharLowerA(str)

#define	inkStrToLowercaseW(str)	\
	CharLowerW(str)

#define	inkStrBuffToLowercaseA(str, size)	\
	CharLowerBuffA(str, size)

#define	inkStrBuffToLowercaseW(str, size)	\
	CharLowerBuffW(str, size)

#define	inkStrToUppercaseA(str)	\
	CharUpperA(str)

#define	inkStrToUppercaseW(str)	\
	CharUpperW(str)

#define	inkStrBuffToUppercaseA(str, size)	\
	CharUpperBuffA(str, size)

#define	inkStrBuffToUppercaseW(str, size)	\
	CharUpperBuffW(str, size)

#define	inkStrCmpA(strd, strs)	\
	(CompareStringA(LOCALE_USER_DEFAULT, 0, strs, -1, strs, -1) - 2)

#define	inkStrCmpW(strd, strs)	\
	(CompareStringW(LOCALE_USER_DEFAULT, 0, strs, -1, strs, -1) - 2)

#define	inkStrCmpIA(strd, strs)	\
	(CompareStringA(LOCALE_USER_DEFAULT, NORM_IGNORECASE, strs, -1, strs, -1) - 2)

#define	inkStrCmpIW(strd, strs)	\
	(CompareStringW(LOCALE_USER_DEFAULT, NORM_IGNORECASE, strs, -1, strs, -1) - 2)

#define	inkStrCmpHA(strd, strs)	\
	(CompareStringA(LOCALE_USER_DEFAULT, NORM_IGNORECASE | NORM_IGNORESYMBOLS | NORM_IGNOREWIDTH, strs, -1, strs, -1) - 2)

#define	inkStrCmpHW(strd, strs)	\
	(CompareStringW(LOCALE_USER_DEFAULT, NORM_IGNORECASE | NORM_IGNORESYMBOLS | NORM_IGNOREWIDTH, strs, -1, strs, -1) - 2)

#define	inkStrBuffCmpA(strd, szd, strs, szs)	\
	(CompareStringA(LOCALE_USER_DEFAULT, 0, strs, szd, strs, szs) - 2)

#define	inkStrBuffCmpW(strd, szd, strs, szs)	\
	(CompareStringW(LOCALE_USER_DEFAULT, 0, strs, szd, strs, szs) - 2)

#define	inkStrBuffCmpIA(strd, szd, strs, szs)	\
	(CompareStringA(LOCALE_USER_DEFAULT, NORM_IGNORECASE, strs, szd, strs, szs) - 2)

#define	inkStrBuffCmpIW(strd, szd, strs, szs)	\
	(CompareStringW(LOCALE_USER_DEFAULT, NORM_IGNORECASE, strs, szd, strs, szs) - 2)

#define	inkStrBuffCmpHA(strd, szd, strs, szs)	\
	(CompareStringA(LOCALE_USER_DEFAULT, NORM_IGNORECASE | NORM_IGNORESYMBOLS | NORM_IGNOREWIDTH, strs, szd, strs, szs) - 2)

#define	inkStrBuffCmpHW(strd, szd, strs, szs)	\
	(CompareStringW(LOCALE_USER_DEFAULT, NORM_IGNORECASE | NORM_IGNORESYMBOLS | NORM_IGNOREWIDTH, strs, szd, strs, szs) - 2)

#define	inkStrBuffLoadA(buff, bsz, strid)	\
	LoadStringA(inkHInstance, strid, buff, bsz)

#define	inkStrBuffLoadW(buff, bsz, strid)	\
	LoadStringW(inkHInstance, strid, buff, bsz)

#define	inkStrCatA(strd, strs)	\
	lstrcatA(strd, strs)

#define	inkStrCatW(strd, strs)	\
	lstrcatW(strd, strs)

#define	inkStrBuffCatA(strd, bszd, strs)	\
	StringCchCatA(strd, bszd, strs)

#define	inkStrBuffCatW(strd, bszd, strs)	\
	StringCchCatW(strd, bszd, strs)

#define	inkStrBuffCatNA(strd, bszd, strs, n)	\
	StringCchCatNA(strd, bszd, strs, n)

#define	inkStrBuffCatNW(strd, bszd, strs, n)	\
	StringCchCatNW(strd, bszd, strs, n)

#define	inkStrCpyA(strd, strs)	\
	lstrcpyA(strd, strs)

#define	inkStrCpyW(strd, strs)	\
	lstrcpyW(strd, strs)

#define	inkStrBuffCpyA(strd, bszd, strs)	\
	StringCchCopyA(strd, bszd, strs)

#define	inkStrBuffCpyW(strd, bszd, strs)	\
	StringCchCopyW(strd, bszd, strs)

#define	inkStrBuffCpyNA(strd, bszd, strs, n)	\
	StringCchCopyNA(strd, bszd, strs, n)

#define	inkStrBuffCpyNW(strd, bszd, strs, n)	\
	StringCchCopyNW(strd, bszd, strs, n)

#define	inkStrBuffGetA(strd, bszd)	\
	StringCchGetsA(strd, bszd)

#define	inkStrBuffGetW(strd, bszd)	\
	StringCchGetsW(strd, bszd)

#define	inkStrBuffPrintFW	\
	StringCChPrintfW
	
#define	inkStrBuffPrintFA	\
	StringCChPrintfA

#define	inkStrBuffConvWA(strd, bszd, strs, bszs)	\
	MultiByteToWideChar(_ASCII_CONVERT, 0, strs, bszs, strd, bszd)

#define	inkStrBuffConvAW(strd, bszd, strs, bszs)	\
	WideCharToMultiByte(_ASCII_CONVERT, 0, strs, bszs, strd, bszd, NULL, NULL)

#define	inkStrSizeA(str)	\
	(inkStrLenA(str)+1)
	
#define	inkStrSizeW(str)	\
	((inkStrLenW(str)+1)<<1)
	
	
// Seletion of appropriate macros	
#ifdef	UNICODE
#define	inkStrLen							inkStrLenW
#define	inkStrToLowercase					inkStrToLowercaseW
#define	inkStrBuffToLowercase				inkStrBuffToLowercaseW
#define	inkStrToUppercase					inkStrToUppercaseW
#define	inkStrBuffToUppercase				inkStrBuffToUppercaseW
#define	inkStrCmp							inkStrCmpW
#define	inkStrCmpI							inkStrCmpIW
#define	inkStrCmpH							inkStrCmpHW
#define	inkStrBuffCmp						inkStrBuffCmpW
#define	inkStrBuffCmpI						inkStrBuffCmpIW
#define	inkStrBuffCmpH						inkStrBuffCmpHW
#define	inkStrBuffLoad						inkStrBuffLoadW
#define	inkStrCat							inkStrCatW
#define	inkStrBuffCat						inkStrBuffCatW
#define	inkStrBuffCatN						inkStrBuffCatNW
#define	inkStrCpy							inkStrCpyW
#define	inkStrBuffCpy						inkStrBuffCpyW
#define	inkStrBuffCpyN						inkStrBuffCpyNW
#define	inkStrPrintF						inkStrPrintFW
#define	inkStrBuffConvTA					inkStrBuffConvWA
#define	inkStrBuffConvAT					inkStrBuffConvAW
#define	inkStrBuffConvTW(strd, bszd, strs, bszs)
#define	inkStrBuffConvWT(strd, bszd, strs, bszs)
#define	inkStrSize							inkStrSizeW
#else
#define	inkStrLen							inkStrLenA
#define	inkStrToLowercase					inkStrToLowercaseA
#define	inkStrBuffToLowercase				inkStrBuffToLowercaseA
#define	inkStrToUppercase					inkStrToUppercaseA
#define	inkStrBuffToUppercase				inkStrBuffToUppercaseA
#define	inkStrCmp							inkStrCmpA
#define	inkStrCmpI							inkStrCmpIA
#define	inkStrCmpH							inkStrCmpHA
#define	inkStrBuffCmp						inkStrBuffCmpA
#define	inkStrBuffCmpI						inkStrBuffCmpIA
#define	inkStrBuffCmpH						inkStrBuffCmpHA
#define	inkStrBuffLoad						inkStrBuffLoadA
#define	inkStrCat							inkStrCatA
#define	inkStrBuffCat						inkStrBuffCatA
#define	inkStrBuffCatN						inkStrBuffCatNA
#define	inkStrCpy							inkStrCpyA
#define	inkStrBuffCpy						inkStrBuffCpyA
#define	inkStrBuffCpyN						inkStrBuffCpyNA
#define	inkStrPrintF						inkStrPrintFA
#define	inkStrBuffConvTW					inkStrBuffConvAW
#define	inkStrBuffConvWT					inkStrBuffConvWA
#define	inkStrBuffConvTA(strd, bszd, strs, bszs)
#define	inkStrBuffConvAT(strd, bszd, strs, bszs)
#define	inkStrSize							inkStrSizeA
#endif
	
	
// Function Declarations
DLL_API WCHAR* inkStrCpyAllocW(WCHAR* src);
DLL_API char* inkStrCpyAllocA(char* src);


// Seletion of appropriate functions
#ifdef	UNICODE
#define	inkStrCpyAlloc						inkStrCpyAllocW
#else
#define	inkStrCpyAlloc						inkStrCpyAllocA
#endif	
	

#ifdef __cplusplus
}
#endif

#endif


