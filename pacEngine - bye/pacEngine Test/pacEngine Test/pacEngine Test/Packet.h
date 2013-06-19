/****************************************************************************
 *                                                                          *
 * File    : Packet.h                                                       *
 *                                                                          *
 * Purpose : The Packet header of pacEngine                                 *
 *                                                                          *
 * History : Date        Part of pacEngine                                  *
 *           07/12/2011  Subhajit Sahu                                      *
 *                                                                          *
 ****************************************************************************/

/* Include Files */
#include <windows.h>




// Define a 2 DWORD structure
typedef struct _DWORD2
{
	LPVOID	High;
	LPVOID	Low;
}DWORD2;



// Define a 2 LPVOID structure
typedef struct _LPVOID2
{
	LPVOID	High;
	LPVOID	Low;
}LPVOID2;



// Define the Basic PACKET_HOLDER
#define		_PACKET_HOLDER	\
	DWORD		PacketSize;\
	DWORD		PacketType;\
	DWORD		DataType;\
	DWORD		DataSize;



// The Most Basic datatype of pacEngine - PACKET [Type  = 0x0000]
typedef struct _PACKET
{
	_PACKET_HOLDER
	TCHAR*	Dummy[16];
}PACKET;



// Packet whose data is in memory (1 pointer)  [Type  = 0x0010]
typedef struct _PACKET_MEM
{
	_PACKET_HOLDER
	void*	DataBlock;
}PACKET_MEM;



// Packet whose data is in memory (2 pointers) [Type  = 0x0011]
typedef struct _PACKET_MEM2
{
	_PACKET_HOLDER
	void*	DataBlock[2];
	UINT	DataBlockSize[2];
}PACKET_MEM2;



// Packet whose data is in memory (3 pointers) [Type  = 0x0012]
typedef struct _PACKET_MEM3
{
	_PACKET_HOLDER
	void*	DataBlock[3];
	UINT	DataBlockSize[3];
}PACKET_MEM3;



// Packet whose data is in memory (n pointers) [Type  = 0x0013]
typedef struct _PACKET_MEMN
{
	_PACKET_HOLDER
	UINT	N;
	void**	DataBlock;
	UINT*	DataBlockSize;
}PACKET_MEMN;



// Packet whose data is in file (handle, ptr, size) [Type  = 0x0020]
typedef struct _PACKET_FILE_HANDLE_DATA_PTR_SIZE
{
	_PACKET_HOLDER
	HANDLE		FileHandle;
	LPVOID		DataBlock;
}PACKET_FILE_HANDLE_DATA_PTR_SIZE;



// Packet whose data is in file (handle, packetnum) [Type  = 0x0021]
typedef struct _PACKET_FILE_HANDLE_PACK_NUM
{
	_PACKET_HOLDER
	HANDLE		FileHandle;
	UINT		PacketNum;
}PACKET_FILE_HANDLE_PACK_NUM;



// Packet whose data is in file (handle, packetname) [Type  = 0x0022]
typedef struct _PACKET_FILE_HANDLE_PACK_NAME
{
	_PACKET_HOLDER
	HANDLE		FileHandle;
	TCHAR*		PacketName;
}PACKET_FILE_HANDLE_PACK_NAME;



// Packet whose data is in file (handle) [Type  = 0x0023]
typedef struct _PACKET_FILE_HANDLE
{
	_PACKET_HOLDER
	HANDLE		FileHandle;
}PACKET_FILE_HANDLE;



// Packet whose data is in file (filename, ptr, size) [Type  = 0x0024]
typedef struct _PACKET_FILE_NAME_DATA_PTR_SIZE
{
	_PACKET_HOLDER
	TCHAR*		FileName;
	LPVOID		DataBlock;
}PACKET_FILE_NAME_DATA_PTR_SIZE;



// Packet whose data is in file (filename, packetnum) [Type  = 0x0025]
typedef struct _PACKET_FILE_NAME_PACK_NUM
{
	_PACKET_HOLDER
	TCHAR*		FileName;
	UINT		PacketNum;
}PACKET_FILE_NAME_PACK_NUM;



// Packet whose data is in file (handle, packetname) [Type  = 0x0026]
typedef struct _PACKET_FILE_NAME_PACK_NAME
{
	_PACKET_HOLDER
	TCHAR*		FileName;
	TCHAR*		PacketName;
}PACKET_FILE_NAME_PACK_NAME;



// Packet whose data is in file (filename) [Type  = 0x0027]
typedef struct _PACKET_FILE_NAME
{
	_PACKET_HOLDER
	TCHAR*		FileName;
}PACKET_FILE_NAME;



// Define a Packet Handle
#define	PACHANDLE		PACKET*


/* [PACKET TYPES] */
#define		PACKET_TYPE_UNKNOWN							0x0000
#define		PACKET_TYPE_NULL							0x0000
#define		PACKET_TYPE_MEMPTR							0x0010
#define		PACKET_TYPE_MEM2PTR							0x0011
#define		PACKET_TYPE_MEM3PTR							0x0012
#define		PACKET_TYPE_MEMNPTR							0x0013
#define		PACKET_TYPE_FILE_HANDLE_DATA_PTR_SIZE		0x0020
#define		PACKET_TYPE_FILE_HANDLE_PACK_NUM			0x0021
#define		PACKET_TYPE_FILE_HANDLE_PACK_NAME			0x0022
#define		PACKET_TYPE_FILE_HANDLE						0x0023
#define		PACKET_TYPE_FILE_NAME_DATA_PTR_SIZE			0x0024
#define		PACKET_TYPE_FILE_NAME_PACK_NUM				0x0025
#define		PACKET_TYPE_FILE_NAME_PACK_NAME				0x0026
#define		PACKET_TYPE_FILE_NAME						0x0027
#define		PACKET_TYPE_AUDIO_HANDLE					0x0050



/* [DATA TYPES] */
#define		DATA_TYPE_UNKNOWN					0x0000
#define		DATA_TYPE_AUDIO_MIN					0x0010
#define		DATA_TYPE_AUDIO_WAVE				0x0010
#define		DATA_TYPE_AUDIO_MAX					0x001F








/* Insert data to a queue whose size must be a power of 2 ( max = size-1 )		*
 * The macro may or may not be able to insert data to the queue					*/
#define		_pacInsertQueue(queue, num, rear, max, data)	\
if(num <= max)\
{\
	queue[rear] = data;\
	rear = (rear+1) & max;\
	num++;\
}


/* Delete data from a queue whose size must be a power of 2 ( max = size-1 )	*
 * The macro may or may not be able to delete data from queue					*
 * data may contain garbage value if queue is empty and data is not initialized	*/
#define		_pacDeleteQueue(queue, num, front, max, data)	\
if(num)\
{\
data = queue[front];\
front = (front+1) & max;\
num--;\
}\






/* ---------------------------Macros-------------------------- */
// Check if a Packet is a proper one
#ifdef	_DEBUG							// Checking will be done in debug mode only
	#define	_Check_Packet_Correctness(pac)	\
		if(!pac)\
		{\
			pacError = 0;\
			pacErrorString = _T("No Packet Given");\
			return;\
		}\
		if(!(pac->PacketSize))\
		{\
			pacError = 0;\
			pacErrorString = _T("Invalid Packet Size");\
			return;\
		}\

#else									// Reselase mode
	#define	_Check_Packet_Correctness(pac)	
#endif


// Check for a valid Packet Type
#ifdef	_DEBUG
	#define	_Check_Packet_Type(pac)	\
		if((pac->DataType) == DATA_TYPE_UNKNOWN)\
		{\
			if(((pac->PacketType) >= 0x0024) && ((pac->PacketType) <= 0x0027))\
			{\
				str = ((PACKET_FILE_NAME*)pac)->FileName;\
				len = lstrlen(str);\
				for(i=len-1; i>=0; i--)\
					if(str[i] == _T('.')) break;\
				if(i < 0)\
				{\
					pacError = 0;\
					pacErrorString = _T("No File Extension");\
					return;\
				}\
				str = str + i + 1;\
				if(lstrcmpi(str, _T("wav")) == 0) pac->DataType = DATA_TYPE_AUDIO_WAVE;\
				else\
				{\
					pacError = 0;\
					pacErrorString = _T("Unknown Audio Extension");\
					return;\
				}\
			}\
			else\
			{\
				pacError = 0;\
				pacErrorString = _T("Data Type could not be determined from file name");\
				return;\
			}\
		}
#else
	#define	_Check_Packet_Type(pac)	
#endif


// Memory Allocation
#ifdef	_DEBUG
	void*	pacAudioAlloc(SIZE_T sz)
	{
		void* ret;
		ret = GlobalAlloc(GMEM_FIXED, sz);
		if(ret == NULL)
		{
			pacError = 0;
			pacErrorString = _T("Memory Allocation failed");
		}
		return(ret);
	}
#else
	#define	pacAudioAlloc(sz)	\
		GlobalAlloc(GMEM_FIXED, sz)
#endif


// Memory Freeing
#ifdef	_DEBUG
	void*	pacAudioFree(void* mem)
	{
		void* ret;
		ret = GlobalFree((HGLOBAL)mem);
		if(ret != NULL)
		{
			pacError = 0;
			pacErrorString = _T("Memory Freeing failed");
		}
		return(ret);
	}
#else
	#define	pacAudioFree(mem)	\
		GlobalFree(mem)
#endif






