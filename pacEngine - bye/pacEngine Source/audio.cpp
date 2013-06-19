/****************************************************************************
 *                                                                          *
 * File    : audio.cpp                                                      *
 *                                                                          *
 * Purpose : The Audio module of pacEngine                                  *
 *                                                                          *
 * History : Date        Part of pacEngine                                  *
 *           07/12/2011  Subhajit Sahu                                      *
 *                                                                          *
 ****************************************************************************/

/* Include Files */
#include <windows.h>
#include <mmsystem.h>


/* Import Library required */
#pragma comment(lib, "winmm.lib")







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



// Packet with all Wave data details
typedef struct _PACKET_WAVEOUT_DATA
{
	_PACKET_HOLDER
	PACKET*				SourcePacket;
	HANDLE				Handle;
	UINT				DataAddress;
	UINT				DataLeft;
	void*				Buffer;
	UINT				BufferSize;
	WAVEHDR				WaveHeader;
	HWAVEOUT			hWaveOut;
	float				Volume;
	float				PlaybackRate;
	float				Pitch;
	BOOL				PlayState;
}PACKET_WAVEOUT_DATA;




// WAVE file header		-	thanks to https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
typedef struct _WAVE_FILE_HEADER
{
	char	ChunkID[4];
	DWORD	ChunkSize;
	char	Format[4];
	char	Subchunk1ID[4];
	DWORD	Subchunk1Size;
	WORD	AudioFormat;
	WORD	NumChannels;
	DWORD	SampleRate;
	DWORD	ByteRate;
	WORD	BlockAlign;
	WORD	BitsPerSample;
	char	Subchunk2ID[4];
	DWORD	Subchunk2Size;
}WAVE_FILE_HEADER;

#define	PACHANDLE		PACKET*







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









/* Global data */
static UINT				pacError;
static TCHAR*			pacErrorString;


/* Audio Module Data */
static UINT				pacAudioDataLoadLimit;			//if audio data < this then fully load it to memory
static UINT				pacAudioDeviceID;
static WAVEOUTCAPS		pacAudioDevice;
static UINT				pacAudioNumDevices;
static HANDLE			pacAudioJobThread;
static void**			pacAudioJobQueue;
static UINT				pacAudioJobQueueRear;
static UINT				pacAudioJobQueueFront;
static UINT				pacAudioJobQueueNum;
static UINT				pacAudioJobQueueMax;
static UINT				pacAudioJobQueueBusy;
static BOOL				pacAudioJobActive;
static BOOL				pacAudioJobExit;






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








// Audio Module Functions
PACHANDLE	pacAudioOpen(PACKET*);
BOOL		pacAudioSetVolume(PACHANDLE, float);
float		pacAudioGetVolume(PACHANDLE);
BOOL		pacAudioSetPlaybackRate(PACHANDLE, float);
float		pacAudioGetPlaybackRate(PACHANDLE);
BOOL		pacAudioSetPitch(PACHANDLE);
float		pacAudioGetPitch(PACHANDLE);
BOOL		pacAudioPlay(PACHANDLE);
BOOL		pacAudioPause(PACHANDLE);
BOOL		pacAudioResume(PACHANDLE);
BOOL		pacAudioStop(PACHANDLE);
BOOL		pacAudioClose(PACHANDLE);
BOOL		pacAudioIsBeingPlayed(PACHANDLE);
//PACHANDLE	pacAudioCreateChannel(UINT);
//BOOL		pacAudioRemoveChannel(PACHANDLE);
//BOOL		pacAudioPlayChannel(PACHANDLE);
//BOOL		pacAudioPauseChannel(PACHANDLE);
//BOOL		pacAudioResumeChannel(PACHANDLE);
//BOOL		pacAudioSetChannelVolume(PACHANDLE, float);
//BOOL		pacAudioGetChannelVolume(PACHANDLE)
//BOOL		pacAudioSetChannelPlaybackRate(PACHANDLE, float);
//float		pacAudioGetChannelPlaybackRate(PACHANDLE);
//BOOL		pacAudioAddToChannel(PACHANDLE, PACHANDLE);
//BOOL		pacAudioRemoveFromChannel(PACHANDLE, PACHANDLE);
//BOOL		pacAudioIsInChannel(PACHANDLE, PACHANDLE);
//BOOL		pacAudioIsChannelBeingPlayed(PACHANDLE);
BOOL		pacAudioSetBufferSize(UINT);
UINT		pacAudioGetBufferSize(void);


// Internal Functions
BOOL		pacAudioStartService(UINT, UINT, UINT);
BOOL		pacAudioStopService(void);
void		pacAudioJobManager(void* no_use);
void		pacAudioOpen_Job(PACHANDLE, PACKET*);
void		pacAudioOpenWav_Job(PACKET_WAVEOUT_DATA*, PACKET*);




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







// Start the Audio service by selecting the best audio device
BOOL		pacAudioStartService(UINT buf_sz, UINT q_sz, UINT stack_sz, UINT hnd_sz)
{
	int				BestDev;
	UINT			i;
	UINT			CapsCount;
	UINT			MaxCapsCount;
	
	pacAudioSetBufferSize(buf_sz);
	BestDev = -1;
	MaxCapsCount = 0;
	pacAudioNumDevices = waveOutGetNumDevs();
	if(!pacAudioNumDevices)
	{
		pacError = 0;		// Failed! No audio devices present
		pacErrorString = _T("No Audio devices present");
		return(FALSE);
	}
	for(i=0; i<pacAudioNumDevices; i++)
	{
		if(waveOutGetDevCaps((UINT_PTR)i, &pacAudioDevice, sizeof(WAVEOUTCAPS)) == MMSYSERR_NOERROR)
		{
			CapsCount = 0;
			while(pacAudioDevice.dwFormats)
			{
				if(pacAudioDevice.dwFormats & 1) CapsCount++;
				pacAudioDevice.dwFormats >>= 1;
			}
			if(MaxCapsCount < CapsCount)
			{
				BestDev = i;
				MaxCapsCount = CapsCount;
			}
		}
	}
	if(BestDev == -1)
	{
		pacError = 0;
		pacErrorString = _T("Proper Audio drivers not present");
	}
	pacAudioDeviceID = BestDev;
	waveOutGetDevCaps((UINT_PTR)BestDev, &pacAudioDevice, sizeof(WAVEOUTCAPS));
	#ifdef	_DEBUG
	if(q_sz < 5) q_sz = 5;
	#endif

	// Create Job Queue
	q_sz = (1<<q_sz);
	pacAudioJobQueue = (void**) pacAudioAlloc(q_sz);
	#ifdef	_DEBUG
	if(!pacAudioJobQueue) return(FALSE);
	#endif
	pacAudioJobQueueNum = 0;
	pacAudioJobQueueMax = q_sz - 1;
	pacAudioJobQueueRear = 0;
	pacAudioJobQueueFront = 0;
	// Audio Device(s) found. Selected best device.
	// Now Creating a Separate Thread for Audio Processing
	#ifdef	_DEBUG
	if(stack_sz < 1024) stack_sz = 1024;
	#endif
	pacAudioJobActive = FALSE;
	pacAudioJobQueueBusy = FALSE;
	pacAudioJobThread = CreateThread(NULL, stack_sz, (LPTHREAD_START_ROUTINE)pacAudioJobManager, (void*)pacAudioJobQueue, CREATE_SUSPENDED, NULL);
	return(TRUE);
}


// Stop the audio service
BOOL		pacAudioStopService(void)
{
	// Stop the Audio Module Thread
	pacAudioJobExit = TRUE;
	if(!pacAudioJobActive)
	{
		pacAudioJobActive = TRUE;
		ResumeThread(pacAudioJobThread);
	}
	while(pacAudioJobExit);

	// Stop and remove audios

	return(TRUE);
}


// The Main Job Manager of pacAudio Module
void		pacAudioJobManager(void* queue)
{
	
	void*					test;
	PACKET_WAVEOUT_DATA*	ret;
	void					(*func)(PACHANDLE, PACKET*);

	//pacAudioJobQueue = (void**)queue;
	while(1)
	{
		printf("pacAudioJob Thread running.\n");
		pacAudioJobActive = TRUE;
		while(pacAudioJobQueueNum)
		{
			printf("pacAudioJob start getting from Queue: %d.\n", pacAudioJobQueueNum);
			printf("%d %d %d %d\n", (UINT)pacAudioJobQueue, pacAudioJobQueueNum, pacAudioJobQueueFront, pacAudioJobQueueMax);
			_pacDeleteQueue(pacAudioJobQueue, pacAudioJobQueueNum, pacAudioJobQueueFront, pacAudioJobQueueMax, test)
			ret = (PACKET_WAVEOUT_DATA*)test;
			printf("pacAudioJobManager::ret = %d\n", ret);
			printf("pacAudioJob ret got.\n");
			_pacDeleteQueue(pacAudioJobQueue, pacAudioJobQueueNum, pacAudioJobQueueFront, pacAudioJobQueueMax, test)
			func = (void (*)(PACHANDLE, PACKET*))test;
			printf("pacAudioJob func got.\n");
			(*func)((PACHANDLE)ret, ret->SourcePacket);
			printf("pacAudioJob Function call over.\n");
		}
		pacAudioJobActive = FALSE;
		printf("pacAudioJob Suspending Thread.\n");
		if(pacAudioJobExit)
		{
			pacAudioJobExit = FALSE;
			ExitThread(0);
		}
		SuspendThread(pacAudioJobThread);
	}
}


// Stop the audio service
BOOL		pacAudioSetBufferSize(UINT buf_sz)
{
	#ifdef	_DEBUG
		if(buf_sz > 4096)
		{
			pacAudioDataLoadLimit = buf_sz;
			return(TRUE);
		}
		pacError = 0;
		pacErrorString = _T("Buffer Size Too Small");
		return(FALSE);
	#else
		pacAudioDataLoadLimit = buf_sz;
		return(TRUE);
	#endif
}


// Stop the audio service
UINT		pacAudioGetBufferSize(void)
{
	return(pacAudioDataLoadLimit);
}


// Open a Audio Packet
PACHANDLE	pacAudioOpen(PACKET* pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	ret = (PACKET_WAVEOUT_DATA*)pacAudioAlloc(sizeof(PACKET_WAVEOUT_DATA));
	#ifdef	_DEBUG
	if(!ret) return(NULL);
	#endif
	ret->SourcePacket = (PACKET*)pacAudioAlloc(pac->PacketSize);
	#ifdef	_DEBUG
	if(!(ret->SourcePacket)) return(NULL);
	#endif
	CopyMemory(ret->SourcePacket, pac, pac->PacketSize);
	ret->PacketType = 0;			// Dont use the PACHANDLE until PacketType != 0
	ret->PacketSize = sizeof(PACKET_WAVEOUT_DATA);
	while(pacAudioJobQueueBusy);
	pacAudioJobQueueBusy = TRUE;
	_pacInsertQueue(pacAudioJobQueue, pacAudioJobQueueNum, pacAudioJobQueueRear, pacAudioJobQueueMax, (void*)ret)
	_pacInsertQueue(pacAudioJobQueue, pacAudioJobQueueNum, pacAudioJobQueueRear, pacAudioJobQueueMax, (void*)pacAudioOpen_Job)
	pacAudioJobQueueBusy = FALSE;
	printf("pacAudioOpen::ret = %d\n", ret);
	if(!pacAudioJobActive) ResumeThread(pacAudioJobThread);
	return((PACHANDLE)ret);
}


// Set the Volume (improve it for >1x volume as well)
BOOL		pacAudioSetVolume(PACHANDLE pac, float vol)
{
	DWORD					vols;
	PACKET_WAVEOUT_DATA*	ret;

	#ifdef	_DEBUG
	if(vol < 0.0f) vol = 0.0f;
	#endif
	if(pac->PacketType)
	{
		vols = (DWORD)(vol * 0xFFFF);
		ret = (PACKET_WAVEOUT_DATA*)pac;
		waveOutSetVolume(ret->hWaveOut, vols);
		ret->Volume = vol;
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Get the Volume
float		pacAudioGetVolume(PACHANDLE pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	ret = (PACKET_WAVEOUT_DATA*)pac;
	return(ret->Volume);
}


// Set the PlaybackRate (improve it for >1x volume as well)
BOOL		pacAudioSetPlaybackRate(PACHANDLE pac, float rate)
{
	DWORD					rates;
	PACKET_WAVEOUT_DATA*	ret;

	#ifdef	_DEBUG
	if(rate < 0.0f) rate = 0.0f;
	#endif
	if(pac->PacketType)
	{
		rates = (DWORD)(rate * 0x00010000);
		ret = (PACKET_WAVEOUT_DATA*)pac;
		waveOutSetPlaybackRate(ret->hWaveOut, rates);
		ret->PlaybackRate = rate;
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Get the PlaybackRate
float		pacAudioGetPlaybackRate(PACHANDLE pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	ret = (PACKET_WAVEOUT_DATA*)pac;
	return(ret->PlaybackRate);
}


// Set the Pitch (improve it for >1x volume as well)
BOOL		pacAudioSetPitch(PACHANDLE pac, float pitch)
{
	DWORD					pitchs;
	PACKET_WAVEOUT_DATA*	ret;

	#ifdef	_DEBUG
	if(pitch < 0.0f) pitch = 0.0f;
	#endif
	if(pac->PacketType)
	{
		pitchs = (DWORD)(pitch * 0x00010000);
		ret = (PACKET_WAVEOUT_DATA*)pac;
		waveOutSetVolume(ret->hWaveOut, pitchs);
		ret->Pitch = pitch;
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Get the Pitch
float		pacAudioGetPitch(PACHANDLE pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	ret = (PACKET_WAVEOUT_DATA*)pac;
	return(ret->Pitch);
}


// Pause the audio
BOOL		pacAudioPause(PACHANDLE pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	if(pac->PacketType)
	{
		ret = (PACKET_WAVEOUT_DATA*)pac;
		if(ret->PlayState)
		{
			waveOutPause(ret->hWaveOut);
			ret->PlayState = FALSE;
		}
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Resume the audio
BOOL		pacAudioResume(PACHANDLE pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	if(pac->PacketType)
	{
		ret = (PACKET_WAVEOUT_DATA*)pac;
		if(!(ret->PlayState))
		{
			waveOutPause(ret->hWaveOut);
			ret->PlayState = TRUE;
		}
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Stop playing the Audio. Goto Start.
BOOL		pacAudioStop(PACHANDLE pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	if(pac->PacketType)
	{
		ret = (PACKET_WAVEOUT_DATA*)pac;
		if(ret->PlayState)
		{
			waveOutReset(ret->hWaveOut);
			ret->PlayState = FALSE;
		}
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Start playing an audio
BOOL		pacAudioPlay(PACHANDLE pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	if(pac->PacketType)
	{
		ret = (PACKET_WAVEOUT_DATA*)pac;
		printf("pacAudioPlay::ret = %d\n", ret);
		if(!(ret->PlayState))
		{
			waveOutWrite(ret->hWaveOut, &(ret->WaveHeader), sizeof(WAVEHDR));
			ret->PlayState = TRUE;
		}
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Close the Audio
BOOL		pacAudioClose(PACHANDLE pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	if(pac->PacketType)
	{
		ret = (PACKET_WAVEOUT_DATA*)pac;
		if(ret->PlayState)
			pacAudioStop(pac);
		waveOutClose(ret->hWaveOut);
		pacAudioFree(ret->Buffer);
		pacAudioFree(pac);
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Tells if Audio is being played
BOOL		pacIsAudioBeingPlayed(PACHANDLE pac)
{
	PACKET_WAVEOUT_DATA*	ret;

	ret = (PACKET_WAVEOUT_DATA*)pac;
	return(ret->PlayState);
}


// Open an Audio (Job)
void		pacAudioOpen_Job(PACHANDLE ret, PACKET* pac)
{
	#ifdef	_DEBUG
	UINT i, len;
	TCHAR* str;

	/* -----------------------Checking Part----------------------- */
	// Check if Data type is unknown

	_Check_Packet_Correctness(pac)
	_Check_Packet_Type(pac)
	#endif
	/* -----------------------Opening Part------------------------ */
	// Switch as per Data Type
	switch(pac->DataType)
	{

	// A WAVE file
	case DATA_TYPE_AUDIO_WAVE:
		pacAudioOpenWav_Job((PACKET_WAVEOUT_DATA*)ret, pac);
		break;

	#ifdef	_DEBUG
	default:
		pacError = 0;
		pacErrorString = _T("Unknown File Format");
		return;
	#endif
	}
	pacAudioFree(((PACKET_WAVEOUT_DATA*)ret)->SourcePacket);
}


// Open a Wave Audio (Job)
void		pacAudioOpenWav_Job(PACKET_WAVEOUT_DATA* ret, PACKET* pac)
{
	DWORD					dw;
	LPVOID					DataPtr;
	PACKET_FILE_NAME*		pacFileName;
	WAVE_FILE_HEADER		WavFileHdr;
	WAVEFORMATEX			WavFmt;

	DataPtr = NULL;
	ret->DataType = DATA_TYPE_AUDIO_WAVE;
	#ifdef	_DEBUG
		if(!ret) return;
	#endif
	if(((pac->PacketType) >= 0x0024) && ((pac->PacketType) <= 0x0027))		// Open the file and give the handle
	{
		pacFileName = (PACKET_FILE_NAME*)pac;
		ret->Handle = CreateFile(pacFileName->FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		#ifdef	_DEBUG
		if((ret->Handle) == INVALID_HANDLE_VALUE) return;
		#endif
		// Manage with packet numbers, names later(derive from file module)
		if((pac->PacketType) == 0x0024)
		{
			DataPtr = ((PACKET_FILE_NAME_DATA_PTR_SIZE*)pac)->DataBlock;
			SetFilePointer(ret->Handle, (LONG)DataPtr, NULL, FILE_BEGIN);
		}
	}
	else if(((pac->PacketType) >= 0x0020) && ((pac->PacketType) <= 0x0023))		// File handle given
	{
		ret->Handle = ((PACKET_FILE_HANDLE*)pac)->FileHandle;
		// Manage with packet numbers, names later(derive from file module)
		if((pac->PacketType) == 0x0024)
		{
			DataPtr = ((PACKET_FILE_HANDLE_DATA_PTR_SIZE*)pac)->DataBlock;
			SetFilePointer(ret->Handle, (LONG)DataPtr, NULL, FILE_BEGIN);
		}
	}
	#ifdef	_DEBUG
	if(ReadFile(ret->Handle, &WavFileHdr, sizeof(WAVE_FILE_HEADER), &dw, NULL) == FALSE)
	{
		pacError = 0;
		pacErrorString = _T("Reading data from file failed");
		return;
	}
	#else
	ReadFile(ret->Handle, &WavFileHdr, sizeof(WAVE_FILE_HEADER), &dw, NULL);
	#endif
	WavFmt.wFormatTag = WavFileHdr.AudioFormat;
	WavFmt.nChannels = WavFileHdr.NumChannels;
	WavFmt.nSamplesPerSec = WavFileHdr.SampleRate;
	WavFmt.nAvgBytesPerSec = WavFileHdr.ByteRate;
	WavFmt.nBlockAlign = WavFileHdr.BlockAlign;
	WavFmt.wBitsPerSample = WavFileHdr.BitsPerSample;
	WavFmt.cbSize = sizeof(WAVEFORMATEX);
	ret->DataSize = WavFileHdr.Subchunk2Size;
	ret->DataAddress = (UINT)(((char*)DataPtr) + sizeof(WAVE_FILE_HEADER));
	#ifdef	_DEBUG
	if(waveOutOpen(&(ret->hWaveOut), pacAudioDeviceID, &WavFmt, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR)
	{
		pacError = 0;
		pacErrorString = _T("Failed to open Audio device");
		return;
	}
	#else
	waveOutOpen(&(ret->hWaveOut), pacAudioDeviceID, &WavFmt, 0, 0, CALLBACK_NULL);
	#endif

	ret->PlayState = FALSE;
	ret->Volume = 1.0f;
	ret->PlaybackRate = 1.0f;
	ret->Pitch = 1.0f;
	ret->WaveHeader.dwFlags = 0;
	ret->WaveHeader.dwLoops = 0;
	ret->WaveHeader.dwUser = 0;
	ret->WaveHeader.lpNext = NULL;
	ret->WaveHeader.reserved = 0;
	if((ret->DataSize) <= pacAudioDataLoadLimit)
	{
		ret->Buffer = (void*) pacAudioAlloc(ret->DataSize);
		#ifdef	_DEBUG
		if(!(ret->Buffer))return;
		#endif
		ret->BufferSize = ret->DataSize;
		ret->DataLeft = 0;
		ret->WaveHeader.lpData = (LPSTR)(ret->Buffer);
		ret->WaveHeader.dwBufferLength = ret->BufferSize;
		#ifdef	_DEBUG
		if(ReadFile(ret->Handle, ret->Buffer, ret->DataSize, &dw, NULL) == FALSE)
		{
			pacError = 0;
			pacErrorString = _T("Reading data from file failed");
			return;
		}
		#else
		ReadFile(ret->Handle, ret->Buffer, ret->DataSize, &dw, NULL);
		#endif
		waveOutPrepareHeader(ret->hWaveOut, &(ret->WaveHeader), sizeof(WAVEHDR));
		ret->PacketType = 1;
		printf("pacAudioOpenWav_Job::ret = %d\n", ret);
		return;
	}
	else
	{
		ret->Buffer = (void*) pacAudioAlloc(pacAudioDataLoadLimit);
		ret->BufferSize = pacAudioDataLoadLimit;
		ret->DataLeft = (ret->DataSize) - pacAudioDataLoadLimit;
		ret->WaveHeader.lpData = (LPSTR)(ret->Buffer);
		ret->WaveHeader.dwBufferLength = pacAudioDataLoadLimit;
		ReadFile(ret->Handle, ret->Buffer, ret->DataSize, &dw, NULL);
		waveOutPrepareHeader(ret->hWaveOut, &(ret->WaveHeader), sizeof(WAVEHDR));
		ret->PacketType = 1;		// Can Play now
		printf("pacAudioOpenWav_Job::ret = %d\n", ret);
		return;
	}
	waveOutPrepareHeader(ret->hWaveOut, &(ret->WaveHeader), sizeof(WAVEHDR));
}






