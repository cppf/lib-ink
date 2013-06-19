/****************************************************************************
 *                                                                          *
 * File    : file.h                                                         *
 *                                                                          *
 * Purpose : Win32 file handling Routines for C                             *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           9/7/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_file_h_

#define	_file_h_		1



#ifdef __cplusplus
    extern "C" {
#endif


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
#define	_fUSAGE_OVERLAPPED			FILE_FLAG_OVERLAPPED
#define	_fUSAGE_NO_BUFFERING		FILE_FLAG_NO_BUFFERING
#define	_fUSAGE_RANDOM_ACCESS		FILE_FLAG_RANDOM_ACCESS
#define	_fUSAGE_SEQUENTIAL_SCAN		FILE_FLAG_SEQUENTIAL_SCAN
#define	_fUSAGE_DELETE_ON_CLOSE		FILE_FLAG_DELETE_ON_CLOSE
#define	_fUSAGE_BACKUP_SEMANTICS	FILE_FLAG_BACKUP_SEMANTICS
#define	_FUSAGE_POSIX_SEMANTICS		FILE_FLAG_POSIX_SEMANTICS
	



// ----Prototypes----------------------------------------------------
PROC	inkStartFileService(void);
PROC	inkStopFileService(void);
HANDLE	inkFileOpen(LPSTR file, DWORD mode);
BOOL	inkFileClose(HANDLE file);
BOOL	inkFileRead(HANDLE file, LPVOID buffer, DWORD NumBytes, LPDWORD ReadBytes);
BOOL	inkFileWrite(HANDLE file, LPVOID buffer, DWORD NumBytes, LPDWORD WroteBytes);
PROC	inkFileAllowMode(DWORD mode);
PROC	inkFileAccessMode(DWORD mode);
PROC	inkFileUsageAttribMode(DWORD usage, DWORD attrib);


#ifdef __cplusplus
    }
#endif


#endif

