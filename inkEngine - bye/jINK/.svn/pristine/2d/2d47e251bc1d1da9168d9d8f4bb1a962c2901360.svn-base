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
#include "strings.h"


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
char*	FileUnicodeStart = "\\\\?\\";




// ----Externals-----------------------------------------------------
DWORD	inkLastError;





class	inkFile
{
// Static Data
public:
static UINT				AccessMode;
static UINT				AllowMode;
static UINT				AttribMode;
static UINT				UsageMode;


// Local Data
private:
HANDLE					Handle;
UINT					Buffer;
UINT					IsAsync;
UINT					BytesRead;
OVERLAPPED				Overlapped;


public:
UINT					ReadComplete;



// CODE
public:

// Start the File Handling Service
static int		StartService(void)
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
inkFile(LPSTR file, DWORD create)
	{
	UINT	len, newlen;
	short*	str;
	
	if(UsageMode == _fUSAGE_ASYNC) IsAsync = 1;
	len = strlen(file);
	if(len < MAX_PATH)
		{
		Handle = CreateFile(file, AccessMode, AllowMode, NULL, create, AttribMode | UsageMode, NULL);
		if(Handle == INVALID_HANDLE_VALUE)
			inkLastError = GetLastError();
		}
	else
		{
		newlen = inkStrlenA(FileUnicodeStart);
		str = GlobalAlloc(GMEM_FIXED, (len+newlen+1)<<1);
			if(str)
				{
				inkStrcpyWA(str, FileUnicodeStart);
				inkStrcpyWA(str+newlen, file);
				Handle = CreateFileW(str, AccessMode, AllowMode, NULL, create, AttribMode | UsageMode, NULL);
				GlobalFree(str);
				if(Handle == INVALID_HANDLE_VALUE)
					inkLastError = GetLastError();
				}
			else
				inkLastError = GetLastError();
		}
	}



// Close a file (destructor)
~inkFile(void)
	{
	if(CloseHandle(Handle) == 0)
		inkLastError = GetLastError();
	}


 
// Seek to a file location
int		Seek(DWORD AddrLow, DWORD AddrHigh)
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



// Read data from a file
int		Read(HANDLE file, LPVOID buffer, DWORD NumBytes)
	{
	return(ReadFile(file, buffer, NumBytes, &BytesRead, NULL));
	}



// Write data to a file
BOOL	Write(HANDLE file, LPVOID buffer, DWORD NumBytes, LPDWORD WroteBytes)
	{
	return(WriteFile(file, buffer, NumBytes, WroteBytes, NULL));
	}



};
static UINT				inkFile::AccessMode = 0;
static UINT				inkFile::AllowMode = 0;
static UINT				inkFile::AttribMode = 0;
static UINT				inkFile::UsageMode = 0;


// CreateFile
// CloseHandle
// GetShortPathName
// GetFullPathName
// CreateDirectory(Ex)
// FindFirstFile(Ex)
// GetFileAttributes(Ex)
// SetFileAttributes(Ex)
// DeleteFile
// CopyFile(Ex)
// MoveFile(Ex)
// SetFilePointer
// ReadFile
// WriteFile
// SetEndOfFile
// FlushFileBuffers
// LockFile(Ex)
// UnlockFile(Ex)
// CancelIO
// GetQueuedCompletionStatus
// PostQueuedCompletionStatus
// FindNextFile
// FindClose
// SearchPath
// GetTempFileName
// GetTempPath
// GetInformationByHandle
// GetVolumeInformation
// SetVolumeLabel
// GetSystemDirectory
// GetWindowsDirectory
// GetDiskFreeSpace
// GetDriveType
// GetLogicalDrives
// GetLogicalDriveStrings
// RemoveDirectory
// GetCurrentDirectory
// SetCurrentDirectory
// BackupRead
// BackupSeek
// BackupWrite
// GetFileInformationByHandle
// GetFileSize
// GetFileTime
// GetFileType
// ReadDirectoryChangesW
// SetFileTime
// FindFirstChangeNotification
// FindNextChangeNotification
// FindCloseChangeNotification
// DeviceIoControl
// GetCompressedFileSize




#endif

