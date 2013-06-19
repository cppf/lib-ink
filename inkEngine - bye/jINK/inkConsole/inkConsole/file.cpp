#include "stdafx.h"
/****************************************************************************
 *                                                                          *
 * File    : file.cpp                                                       *
 *                                                                          *
 * Purpose : Win32 file handling Routines for C++                           *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           5/12/2011  Subhajit Sahu                                       *
 *                                                                          *
 ****************************************************************************/


#ifndef	_file_cpp_

#define	_file_cpp_		1




// ----Includes------------------------------------------------------
#include "win32.h"


// ----Constants-----------------------------------------------------
#define	_fREAD						GENERIC_READ
#define	_fREAD_WRITE				GENERIC_READ | GENERIC_WRITE
#define	_fALLOW_NONE				0
#define	_fALLOW_READ				FILE_SHARE_READ
#define	_fOPEN_CREATE_NEW			CREATE_NEW
#define	_fOPEN_CREATE_ALWAYS		CREATE_ALWAYS
#define	_fOPEN_OPEN_EXISTING		OPEN_EXISTING
#define	_fOPEN_OPEN_ALWAYS			OPEN_ALWAYS
#define	_fOPEN_TRUNCATE_EXISTING	TRUNCATE_EXISTING
#define	_fALLOW_READ_WRITE			FILE_SHARE_WRITE
#define	_fATTRIB_ARCHIVE			FILE_ATTRIBUTE_ARCHIVE
#define	_fATTRIB_COMPRESSED			FILE_ATTRIBUTE_COMPRESSED
#define	_fATTRIB_HIDDEN				FILE_ATTRIBUTE_HIDDEN
#define	_fATTRIB_NORMAL				FILE_ATTRIBUTE_NORMAL
#define	_fATTRIB_OFFLINE			FILE_ATTRIBUTE_OFFLINE
#define	_fATTRIB_READONLY			FILE_ATTRIBUTE_READONLY
#define	_fATTRIB_SYSTEM				FILE_ATTRIBUTE_SYSTEM
#define	_fATTRIB_TEMPORARY			FILE_ATTRIBUTE_TEMPORARY
#define	_fUSAGE_WRITE_THROUGH		FILE_FLAG_WRITE_THROUGH
#define	_fUSAGE_ASYNC				FILE_FLAG_OVERLAPPED
#define	_fUSAGE_NO_BUFFERING		FILE_FLAG_NO_BUFFERING
#define	_fUSAGE_RANDOM_ACCESS		FILE_FLAG_RANDOM_ACCESS
#define	_fUSAGE_SEQUENTIAL_SCAN		FILE_FLAG_SEQUENTIAL_SCAN
#define	_fUSAGE_DELETE_ON_CLOSE		FILE_FLAG_DELETE_ON_CLOSE
#define	_fUSAGE_BACKUP_SEMANTICS	FILE_FLAG_BACKUP_SEMANTICS
#define	_fUSAGE_POSIX_SEMANTICS		FILE_FLAG_POSIX_SEMANTICS



// ----Globals-------------------------------------------------------
static wchar_t*		FileUnicodeStart = L"\\\\?\\";


// ----Externals-----------------------------------------------------
static UINT			inkLastError;






// ----Main Code-----------------------------------------------------

class	inkFile
{
	// Static Data
	public:
	static UINT				NumObjects;
	static UINT				AccessMode;
	static UINT				AllowMode;
	static UINT				AttribMode;
	static UINT				UsageMode;


	// Local Data
	private:
	HANDLE					Handle;
	UINT					Buffer;
	UINT					IsAsync;
	DWORD					BytesRead;
	OVERLAPPED				Overlapped;
	public:
	UINT					ReadComplete;


	// Functions
	public:
	static int	StartService (void);
	static int	StopService (void);
				inkFile (TCHAR* file, UINT create);
				~inkFile (void);
//	int			Seek (UINT AddrLow, UINT AddrHigh);
	int			Read (HANDLE file, void* buffer, UINT NumBytes);
	int			Write (HANDLE file, void* buffer, UINT NumBytes, DWORD* WroteBytes);
};
UINT	inkFile::AccessMode = 0;
UINT	inkFile::AllowMode = 0;
UINT	inkFile::AttribMode = 0;
UINT	inkFile::UsageMode = 0;




// Start the File Handling Service
int		inkFile::StartService(void)
	{
	AccessMode = _fREAD_WRITE;
	AllowMode = _fALLOW_READ_WRITE;
	AttribMode = _fATTRIB_NORMAL;
	UsageMode = _fUSAGE_RANDOM_ACCESS;
	return(0);
	}



// Stop the File Handling Service
static int		StopService(void)
	{
	return(0);
	}


// Open a file (constructor)
inkFile::inkFile(TCHAR* file, UINT create)
	{
	UINT	len, newlen;
	WCHAR*	str;
	
	if(NumObjects == 0) StartService();
	NumObjects++;
	if(UsageMode == _fUSAGE_ASYNC) IsAsync = 1;
	len = lstrlen(file);
	newlen = lstrlen(FileUnicodeStart);
	str = (WCHAR*)GlobalAlloc(GMEM_FIXED, (len+newlen+1)<<1);
	if(str)
		{
		lstrcpy(str, FileUnicodeStart);
		lstrcpy(str+newlen, file);
		Handle = CreateFile(str, AccessMode, AllowMode, NULL, create, AttribMode | UsageMode, NULL);
		GlobalFree(str);
		if(Handle == INVALID_HANDLE_VALUE)
			inkLastError = GetLastError();
		}
	else
		inkLastError = GetLastError();
	}



// Close a file (destructor)
inkFile::~inkFile(void)
	{
	if(CloseHandle(Handle) == 0)
		inkLastError = GetLastError();
	}

/*
 
// Seek to a file location
int		inkFile::Seek(UINT AddrLow, UINT AddrHigh)
	{
	UINT res;
	Overlapped.Offset = AddrLow;
	Overlapped.OffsetHigh = AddrHigh;
	Overlapped.hEvent = NULL;
	if(IsAsync)
		{
		ReadComplete = 0;
		res = ReadFileEx(Handle, &Buffer, 0, &BytesRead, &Overlapped, ReadCompletion);
		if(!res)
			{
			inkLastError = GetLastError();
			return(-1);
			}
		else
			return(0);
		}
	res = ReadFile(Handle, &Buffer, 0, &BytesRead, &Overlapped);
	if(!res)
		{
		inkLastError = GetLastError();
		return(-1);
		}
	return(0);
	}

*/

// Read data from a file
int		inkFile::Read(HANDLE file, void* buffer, UINT NumBytes)
	{
	return(ReadFile(file, buffer, NumBytes, &BytesRead, NULL));
	}



// Write data to a file
int	inkFile::Write(HANDLE file, void* buffer, UINT NumBytes, DWORD* WroteBytes)
	{
	return(WriteFile(file, buffer, NumBytes, WroteBytes, NULL));
	}





#endif




int _tmain(int argc, _TCHAR* argv[])
{
	inkFile::StartService();
//	inkFile *f = new inkFile(_T("abc.png"), _fOPEN_CREATE_NEW);
//	delete f;
	inkFile::StopService();
	return 0;
}


