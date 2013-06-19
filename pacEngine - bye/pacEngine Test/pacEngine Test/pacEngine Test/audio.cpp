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
#include "Packet.h"
#include <mmsystem.h>


/* Import Library required */
#pragma comment(lib, "winmm.lib")

// The Audio Handle Structure
typedef struct _PACKET_AUDIO_HANDLE
{
	_PACKET_HOLDER
	UINT				DataAddress;
	UINT				DataPlayed;
	BYTE*				Buffer;
	UINT				BufferSize;
	BYTE*				BufferInUse;
	HANDLE				Handle;
	BOOL				PlayState;
	BOOL				PlayLoop;
	PACKET*				SourcePacket;
	WAVEHDR				WaveHeader;
	HWAVEOUT			hWaveOut;
}PACKET_AUDIO_HANDLE;

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
static UINT				pacAudioHandlesNum;





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
void		pacAudioClose(PACHANDLE);
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
void		pacAudioSetBufferSize(UINT);
UINT		pacAudioGetBufferSize(void);


// Internal Functions
BOOL		pacAudioStartService(UINT, UINT, UINT);
BOOL		pacAudioStopService(void);
void		pacAudioJobManager(void* no_use);
void		pacAudioOpen_Job(PACHANDLE, PACKET*);
void		pacAudioOpenWav_Job(PACKET_AUDIO_HANDLE*, PACKET*);




// Play the Audio in the Buffer In Use
// Then load data into the other buffer
void		pacAudioPlayNext_Job(PACHANDLE pac)
{
	PACKET_AUDIO_HANDLE*	ret;
	DWORD	x;

	waveOutWrite(ret->hWaveOut, &(ret->WaveHeader), sizeof(WAVEHDR));
	ret->DataPlayed += (ret->WaveHeader.dwBufferLength);
	x = (ret->DataSize) - (ret->DataPlayed);
	if((ret->BufferInUse) == (ret->Buffer))
		ret->BufferInUse = (ret->Buffer) + ret->WaveHeader.dwBufferLength;
	else
		ret->BufferInUse = ret->Buffer;
	ret->WaveHeader.lpData = (LPSTR)(ret->BufferInUse);
	ret->WaveHeader.dwFlags = 0;
	ret->WaveHeader.dwLoops = 0;
	ret->WaveHeader.dwUser = 0;
	ret->WaveHeader.lpNext = NULL;
	ret->WaveHeader.reserved = 0;
	if(x > (ret->BufferSize))
	{
		ret->WaveHeader.dwBufferLength = ret->BufferSize;
		ReadFile(ret->Handle, ret->WaveHeader.lpData, ret->WaveHeader.dwBufferLength, &x, NULL);
		waveOutPrepareHeader(ret->hWaveOut, &(ret->WaveHeader), sizeof(WAVEHDR));
		return;
	}
	ret->WaveHeader.dwBufferLength = x;
	ReadFile(ret->Handle, ret->WaveHeader.lpData, ret->WaveHeader.dwBufferLength, &x, NULL);
	waveOutPrepareHeader(ret->hWaveOut, &(ret->WaveHeader), sizeof(WAVEHDR));
}


// Select an Audio device
// Furthur pacAudioOpen's will make use of that device
// Returns FALSE if the Device cant be selected
BOOL		pacAudioSelectDevice(UINT dev)
{
	if(dev < pacAudioNumDevices)
	{
			pacAudioDeviceID = dev;
			return(TRUE);
	}
	return(FALSE);
}


// Get the Device ID of the Audio device being used
UINT		pacAudioGetSelectedDevice(void)
{
	return(pacAudioDeviceID);
}


// Get the number of Audio devices present on this computer
UINT		pacAudioGetNumDevices(void)
{
	return(pacAudioNumDevices);
}


// Start the Audio service
// Select the best audio device available on this computer
// Returns FALSE if no Audio devices present
BOOL		pacAudioStartService(UINT buf_sz, UINT q_sz, UINT stack_sz)
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
// Returns FALSE if some Audio Handles are still open
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
	if(pacAudioHandlesNum > 0) return(FALSE);

	return(TRUE);
}


// The Thread of this Module
// Performs Open Handle asynchronously
void		pacAudioJobManager(void* queue)
{
	
	void*					test;
	PACKET_AUDIO_HANDLE*	ret;
	void					(*func)(PACHANDLE, PACKET*);

	//pacAudioJobQueue = (void**)queue;
	while(1)
	{
		pacAudioJobActive = TRUE;
		while(pacAudioJobQueueNum)
		{
			_pacDeleteQueue(pacAudioJobQueue, pacAudioJobQueueNum, pacAudioJobQueueFront, pacAudioJobQueueMax, test)
			ret = (PACKET_AUDIO_HANDLE*)test;
			_pacDeleteQueue(pacAudioJobQueue, pacAudioJobQueueNum, pacAudioJobQueueFront, pacAudioJobQueueMax, test)
			func = (void (*)(PACHANDLE, PACKET*))test;
			(*func)((PACHANDLE)ret, ret->SourcePacket);
		}
		pacAudioJobActive = FALSE;
		if(pacAudioJobExit)
		{
			pacAudioJobExit = FALSE;
			ExitThread(0);
		}
		SuspendThread(pacAudioJobThread);
	}
}


// Stop the audio service
void		pacAudioSetBufferSize(UINT buf_sz)
{
	if(buf_sz < 4096) buf_sz = 4096;
	pacAudioDataLoadLimit = buf_sz;
}


// Stop the audio service
UINT		pacAudioGetBufferSize(void)
{
	return(pacAudioDataLoadLimit);
}


// Open a Audio Handle
// Returns NULL if Audio Handle could not be opened
// More than 1 thread can call this function simultaneously
PACHANDLE	pacAudioOpen(PACKET* pac)
{
	PACKET_AUDIO_HANDLE*	ret;

	ret = (PACKET_AUDIO_HANDLE*)pacAudioAlloc(sizeof(PACKET_AUDIO_HANDLE));
	#ifdef	_DEBUG
	if(!ret) return(NULL);
	#endif
	ret->PacketType = PACKET_TYPE_NULL;
	ret->PacketSize = sizeof(PACKET_AUDIO_HANDLE);
	ret->SourcePacket = (PACKET*)pacAudioAlloc(pac->PacketSize);
	#ifdef	_DEBUG
	if(!(ret->SourcePacket)) return(NULL);
	#endif
	CopyMemory(ret->SourcePacket, pac, pac->PacketSize);
	while(pacAudioJobQueueBusy);			// Wait for another instance of this function to complete
	pacAudioJobQueueBusy = TRUE;			// Ask the other instances to wait
	_pacInsertQueue(pacAudioJobQueue, pacAudioJobQueueNum, pacAudioJobQueueRear, pacAudioJobQueueMax, (void*)ret)
	_pacInsertQueue(pacAudioJobQueue, pacAudioJobQueueNum, pacAudioJobQueueRear, pacAudioJobQueueMax, (void*)pacAudioOpen_Job)
	pacAudioJobQueueBusy = FALSE;
	if(!pacAudioJobActive) ResumeThread(pacAudioJobThread);	// Start
	pacAudioHandlesNum++;					// Count the number of handles opened
	return((PACHANDLE)ret);
}


// Set the Volume (0.0f - 1.0f)
// Returns FALSE if Audio Handle is not yet ready
BOOL		pacAudioSetVolume(PACHANDLE pac, float vol)
{
	DWORD					vols;
	PACKET_AUDIO_HANDLE*	ret;

	#ifdef	_DEBUG
	if(vol < 0.0f) vol = 0.0f;
	#endif
	if(pac->PacketType)
	{
		vols = (DWORD)(vol * 65535);
		ret = (PACKET_AUDIO_HANDLE*)pac;
		waveOutSetVolume(ret->hWaveOut, vols);
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Get the Volume (0.0f - 1.0f)
// Returns -1.0f if Audio Handle is not yet ready
float		pacAudioGetVolume(PACHANDLE pac)
{
	PACKET_AUDIO_HANDLE*	ret;
	DWORD					vols;

	if(pac->PacketType)
	{
		ret = (PACKET_AUDIO_HANDLE*)pac;
		waveOutGetVolume(ret->hWaveOut, &vols);
		return(((float)vols) / 65535.0f);
	}
	return(-1.0f);
}


// Set the PlaybackRate (0.0f - 15.0f)
// Returns FALSE if Audio Handle is not yet ready
BOOL		pacAudioSetPlaybackRate(PACHANDLE pac, float rate)
{
	DWORD					rates;
	PACKET_AUDIO_HANDLE*	ret;

	#ifdef	_DEBUG
	if(rate < 0.0f) rate = 0.0f;
	#endif
	if(pac->PacketType)
	{
		rates = (DWORD)(rate * 65536);
		ret = (PACKET_AUDIO_HANDLE*)pac;
		waveOutSetPlaybackRate(ret->hWaveOut, rates);
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Get the PlaybackRate (0.0f - 15.0f)
// Returns -1.0f if Audio Handle is not yet ready
float		pacAudioGetPlaybackRate(PACHANDLE pac)
{
	PACKET_AUDIO_HANDLE*	ret;
	DWORD	rates;

	if(pac->PacketType)
	{
		ret = (PACKET_AUDIO_HANDLE*)pac;
		waveOutGetPlaybackRate(ret->hWaveOut, &rates);
		return(((float)rates) / 65536.0f);
	}
	return(-1.0f);
}


// Set the Pitch (0.0f - 5.0f)
// Returns FALSE if Audio Handle is not yet ready
BOOL		pacAudioSetPitch(PACHANDLE pac, float pitch)
{
	DWORD					pitchs;
	PACKET_AUDIO_HANDLE*	ret;

	#ifdef	_DEBUG
	if(pitch < 0.0f) pitch = 0.0f;
	#endif
	if(pac->PacketType)
	{
		pitchs = (DWORD)(pitch * 65536);
		ret = (PACKET_AUDIO_HANDLE*)pac;
		waveOutSetPitch(ret->hWaveOut, pitchs);
		return(TRUE);
	}
	return(FALSE);		// Audio is not yet Open
}


// Get the Pitch (0.0f - 5.0f)
// Returns -1.0f if Audio Handle is not yet ready
float		pacAudioGetPitch(PACHANDLE pac)
{
	PACKET_AUDIO_HANDLE*	ret;
	DWORD	pitchs;

	if(pac->PacketType)
	{
		ret = (PACKET_AUDIO_HANDLE*)pac;
		waveOutGetPitch(ret->hWaveOut, &pitchs);
		return(((float)pitchs) / 65536.0f);
	}
	return(FALSE);
}


// Pause the audio
// Returns FALSE if Audio Handle is not yet ready
BOOL		pacAudioPause(PACHANDLE pac)
{
	PACKET_AUDIO_HANDLE*	ret;

	if(pac->PacketType)
	{
		ret = (PACKET_AUDIO_HANDLE*)pac;
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
// Returns FALSE if Audio Handle is not yet ready
BOOL		pacAudioResume(PACHANDLE pac)
{
	PACKET_AUDIO_HANDLE*	ret;

	if(pac->PacketType)
	{
		ret = (PACKET_AUDIO_HANDLE*)pac;
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
// Returns FALSE if Audio Handle is not yet ready
BOOL		pacAudioStop(PACHANDLE pac)
{
	PACKET_AUDIO_HANDLE*	ret;

	if(pac->PacketType)
	{
		ret = (PACKET_AUDIO_HANDLE*)pac;
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
// Returns FALSE if Audio Handle is not yet ready
BOOL		pacAudioPlay(PACHANDLE pac)
{
	PACKET_AUDIO_HANDLE*	ret;

	if(pac->PacketType)
	{
		ret = (PACKET_AUDIO_HANDLE*)pac;
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
// Waits if Audio Hanle is not yet ready
void		pacAudioClose(PACHANDLE pac)
{
	PACKET_AUDIO_HANDLE*	ret;

	while(!(pac->PacketType));
	ret = (PACKET_AUDIO_HANDLE*)pac;
	if(ret->PlayState)
		pacAudioStop(pac);
	waveOutClose(ret->hWaveOut);
	pacAudioFree(ret->Buffer);
	pacAudioFree(pac);
	pacAudioHandlesNum--;
	return(TRUE);
}


// Tells if Audio is being played
BOOL		pacIsAudioBeingPlayed(PACHANDLE pac)
{
	return(((PACKET_AUDIO_HANDLE*)pac)->PlayState);
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
		pacAudioOpenWav_Job((PACKET_AUDIO_HANDLE*)ret, pac);
		break;

	#ifdef	_DEBUG
	default:
		pacError = 0;
		pacErrorString = _T("Unknown File Format");
		return;
	#endif
	}
	pacAudioFree(((PACKET_AUDIO_HANDLE*)ret)->SourcePacket);
}


// Open a Wave Audio (Job)
void		pacAudioOpenWav_Job(PACKET_AUDIO_HANDLE* ret, PACKET* pac)
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
		ret->DataPlayed = ret->DataSize;
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
		return;
	}
	ret->Buffer = (void*) pacAudioAlloc(pacAudioDataLoadLimit);
	ret->BufferSize = pacAudioDataLoadLimit;
	ret->DataLeft = (ret->DataSize) - pacAudioDataLoadLimit;
	ret->WaveHeader.lpData = (LPSTR)(ret->Buffer);
	ret->WaveHeader.dwBufferLength = pacAudioDataLoadLimit;
	ReadFile(ret->Handle, ret->Buffer, ret->DataSize, &dw, NULL);
	waveOutPrepareHeader(ret->hWaveOut, &(ret->WaveHeader), sizeof(WAVEHDR));
	ret->PacketType = 1;		// Can Play now
	waveOutPrepareHeader(ret->hWaveOut, &(ret->WaveHeader), sizeof(WAVEHDR));
}






