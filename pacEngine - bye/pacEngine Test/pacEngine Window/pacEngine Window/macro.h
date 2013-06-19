/****************************************************************************
 *                                                                          *
 * File    : macro.h                                                        *
 *                                                                          *
 * Purpose : Some macros for C++                                            *
 *                                                                          *
 * History : Date      Part of PAC Game Engine                              *
 *           9/7/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_macro_h_

#define	_macro_h_		1





// _TS(size)
// Give the size in bytes required for text of given size
#ifdef	UNICODE
	#define	_TS(size)	\
		((size)<<1)
#else
	#define	_TS(size)	\
		size
#endif
	

// Allocate Memory
#define pacMemAlloc(size)	\
	(void*)GlobalAlloc(GMEM_FIXED, size)

// Allocate Memory for String of given size
#define	pacStrAlloc(size)	\
	(TCHAR*)GlobalAlloc(GMEM_FIXED, _TS(size))

// Allocate Memory for a Structure(not function)
#define pacStructAlloc(strc)	\
	(void*)GlobalAlloc(GMEM_FIXED, sizeof(*(strc)))

// Allocate & Initialize Memory
#define	pacMemInitAlloc(size)	\
	(void*)GlobalAlloc(GPTR, size)

// Allcate & Initialize Memory for string
#define pacStrInitAlloc(size)	\
	(TCHAR*)GlobalAlloc(GPTR, _TS(size))

// Allocate & Initialize Memory for a Structure(not function)
#define	pacStructInitAlloc(strc)	\
	(void*)GlobalAlloc(GPTR, sizeof(*(strc)))

// Free Memory(not function)
#define	pacMemFree(ptr)	\
	GlobalFree((HGLOBAL)ptr)

// Free Memory allocated to a string(not function)
#define	pacStrFree(ptr)	\
	GlobalFree((HGLOBAL)ptr)

// Free Memory allocated to a structure(not function)
#define	pacStructFree(strc)	\
	GlobalFree((HGLOBAL)strc)

// Initialize Memory
#define	pacMemInit(ptr, size)	\
	ZeroMemory(ptr, size)

// Initialize String
#define	pacStrInit(str, size)	\
	ZeroMemory(str, _TS(size))

// Initialize Structure
#define	pacStructInit(strc)	\
	ZeroMemory(strc, sizeof(*(strc)))

#define	pacStrCpyAlloc(strd, strs)	\
	{\
		strd = pacStrAlloc(lstrlen(strs) + 1);\
		lstrcpy(strd, strs);\
	}\



#endif

