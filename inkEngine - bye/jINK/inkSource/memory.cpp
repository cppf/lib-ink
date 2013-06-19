/****************************************************************************
 *                                                                          *
 * File    : memory.cpp                                                     *
 *                                                                          *
 * Purpose : Win32 memory handling Routines for C++                         *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           5/12/2011  Subhajit Sahu                                       *
 *                                                                          *
 ****************************************************************************/


#ifndef	_memory_cpp_

#define	_memory_cpp_		1




// ----Includes------------------------------------------------------
#include "win32.h"


// ----Constants-----------------------------------------------------
#define			_mSTATIC			GMEM_FIXED
#define			_mDYNAMIC			GMEM_MOVEABLE




// ----Macros--------------------------------------------------------
#define		inkmalloc(bytes)	\
GlobalAlloc(GMEM_FIXED, bytes)

#define		inkfree(ptr)	\
GlobalFree(ptr)



// ----Externals-----------------------------------------------------
//static UINT			inkLastError;






// ----Main Code-----------------------------------------------------

class	inkMemory
{
	// Static Data
	public:
	static MEMORYSTATUS		Info;


	// Local Data
	private:
	UINT					AllocType;
	HGLOBAL					Handle;
	UINT					BufferSize;
	public:
	void*					Ptr;
	UINT					Size;
	UINT					Buffering;


	// Functions
	public:
	static void	StartService (void);
	static void	StopService (void);
				inkMemory(UINT memtype, UINT buffering);
				~inkMemory(void);
	UINT		Alloc(UINT bytes);
	void*		PtrUse(void);
	void		PtrFree(void);
	void		Clean(void);
};
MEMORYSTATUS	inkMemory::Info;


void	inkMemory::StartService(void)
{
	GlobalMemoryStatus(&Info);
}

void	inkMemory::StopService(void)
{
	// Some statements
	return;
}

inkMemory::inkMemory(UINT memtype, UINT buffering)
{
	AllocType = memtype;
	Buffering = buffering;
	BufferSize = buffering + 1;
	Size = 0;
	Handle = GlobalAlloc(memtype, BufferSize);
	Ptr = (void*)Handle;
}

inkMemory::~inkMemory(void)
{
	GlobalFree(Handle);
}

UINT	inkMemory::Alloc(UINT bytes)
{
	Size = bytes;
	if(bytes <= BufferSize) return(0);
	BufferSize  = Size + Buffering;
	Handle = GlobalReAlloc(Handle, BufferSize, AllocType);
	Ptr = (void*)Handle;
}

void*	inkMemory::PtrUse(void)
{
	if(AllocType == _mSTATIC) return((void*)Handle);
	return((Ptr = GlobalLock(Handle)));
}

void	inkMemory::PtrFree(void)
{
	if(AllocType == _mDYNAMIC) GlobalUnlock(Handle);
}

void	inkMemory::Clean(void)
{
	if(AllocType == _mSTATIC) ZeroMemory(Ptr, Size);
	else
	{
		ZeroMemory(GlobalLock(Handle), Size);
		GlobalUnlock(Handle);
	}
}

#endif

