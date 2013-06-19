/****************************************************************************
 *                                                                          *
 * File    : network.cpp                                                    *
 *                                                                          *
 * Purpose : Network Usage Routines for C++                                 *
 *                                                                          *
 * History : Date       Part of INK Game Engine                             *
 *           5/12/2011  Subhajit Sahu                                       *
 *                                                                          *
 ****************************************************************************/



#ifndef	_network_cpp_

#define	_network_cpp_		1




// ----Includes------------------------------------------------------
#include "win32.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>



// ----Constants-----------------------------------------------------
#define		_nACTIVE				0								// Thread will start running as soon as it is created			
#define		_nSUSPENDED				CREATE_SUSPENDED				// Thread wont start running directly
#define		_nMESSAGEQUEUESIZE		4096							// Allocate 4KB space for each Thread's Message Queue (256 messages limit)
#define		_nMESSAGEQUEUEEND		1024							// Number of Max UINTs
#define		_nMESSAGEQUEUENUMMAX	256								// Number of Max Messages



// ----Externals-----------------------------------------------------





// ----Code----------------------------------------------------------


/*
Class:	inkNetwork

Objects of this class must be created to be able to use the threading functions.
However, first of all, inkNetwork::StartService() must be called before any other
function is used or object created.
At the end of the game, inkNetwork::StopService() must be called before exit.
*/
class inkNetwork
{
	// Global/Static Data
	private:
	static IPaddress[4];

	// Local/Instance Data
	private:
	public:


	// Thread Management & Error Handling Functions Declarations
	public:
	static void			StartService(void* err_handler);
	static void			StopService(void);
};
inkNetwork*		MainThread;						// The Main Thread of the Game
inkNetwork*		ErrorHelperThread;				// Error Helper's Thread Object
inkNetwork*		ErrorHelperData;				// Error Helper's Thread Data
BOOL			ErrorBox;						// Display the Error Box
UINT			ErrorBeep;						// Produce Beep Sound on Error
UINT			StackSize;						// Stack Size of Threads being created
UINT			NumThreads;						// Number of Threads created.



// Check for Windows Network Capabilities and prepare accordingly
void	inkNetwork::StartService(inkNetworkGame *net_game)
{
	MIB_IPADDRTABLE ipAddrTable;					// IP Address Table
	PULONG ipAddrTableSz;							// Size of IP Address Table
	WSADATA wsaData;								// Windows Sockets Startup return data
	SOCKET s;										// A socket
	BOOL val;										// Boolean value
	UINT ret;										// To get return values from functions
	UINT x;											// Some integer
	
	ret = WSAStartup(0x202, &wsaData);				// Try to start Windows Sockets v2.2
	if(ret != 0)									// Failed?
	{
		ret = WSAGetLastError();					// Get the error code
		if(ret == WSAVERNOTSUPPORTED)				// Windows Sockets v2.2 not supported
		{
			Supported = FALSE;						// not supported
			// inkThread* inkEngine->ErrorString("Winsock v2.2 not Supported");			// report error to inkEngine Thread
			return;
		}
		else
		{
			Supported = TURE;						// yes! supported
			ServiceAvail = FALSE;					// ooof! no service avail yet.
			// inkThread* inkEngine->ErrorString("Winsock is busy. Retry Later");		// ask inkEngine Thread to retry later
		}
	}
	// Winsock started successfully
	
	// Some initializations
	RawSocketSupport = FALSE;
	ICMPsupport = FALSE;
	BluetoothSupport = FALSE;
	BroadcastSupport = FALSE;
	
	// Check for RAW Socket support
	s = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if(s != INVALID_SOCKET)
	{
		RawSocketSupport = TRUE;
		closesocket(s)
	}
	
	// Check for ICMP support
	s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(s != INVALID_SOCKET)
	{
		ICMPsupport = TRUE;
		closesocket(s);
	}
	
	// Check for Bluetooth Support
	s = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if(s != INVALID_SOCKET)
	{
		BluetoothSupport = TRUE;
		closesocket(s);
	}
	
	// Check for Broadcast support
	s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	val = TRUE;
	ret = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
	if(ret == 0) BroadcastSupport = TRUE;
	
	// Get IP Address Details
	ret = GetIpAddrTable(&ipAddrTable, &ipAddrTableSz, FALSE);
	if(ret != NO_ERROR)
	{
		Supported = FALSE;						// not supported
		// inkThread* inkEngine->ErrorString("IP Helper not Supported");			// report error to inkEngine Thread
		return;
	}
	x = ipAddrTable.dwNumEntries;
	if(x > 4) x = 4;
	for(ret=0; ret<x; ret++)
	{
		IPaddress[i] = ipAddrTable.table.dwAddr;
		SubnetMask[i] = ipAddrTable.table.dwMask;
	}
	
	if(net_game != NULL)
	{
		if(BroadcastSupport)
		{
		}
		else
		{
		}
	}
	else closesocket(s);
}











// Conversion: during transmit
htons()		- short
htonl()		- long

// Conversion: during recieve
ntohs()		- short
ntohl()		- long

// Socket preparation
struct addrinfo
getaddrinfo()
freeaddrinfo()
gethostbyname()
getnameinfo()
gethostbyaddr()
gai_strerror()
memset() // no use
sin_addr.s_addr

// String address to network
inet_pton()
inet_addr()
inet_aton()

// Network to String address
inet_ntop()
inet_ntoa()

// Make the socket
socket()
bind()
connect()
listen()
accept()
close()
send()
recv()
sendto()
recvfrom()
shutdown()
getpeername()
gethostname()
AddIpAddress of dynamic ip addresses



Limited Broadcast: Dest IP = 255.255.255.255
Net-directed Broadcast: Dest IP = netid.255.255.255
Subnet-directed Broadcast: Dest IP = x.x.x.255 for subnet mask 255.255.255.0




// Functions to use
AcceptEX()
bind()
closesocket()
ConnectEx()
DisconnectEx()
freeaddrinfo()
getaddrinfo()
GetAddrInfoEx()
WNetGetNetworkInformation()








// Wow functions
SendARP():
The SendARP function sends an Address Resolution Protocol (ARP) request to obtain the physical address that corresponds
to the specified destination IPv4 address.









// Start Thread Service
void inkNetwork::StartService(void* err_handler)
{
	MainThread = NULL;
	ErrorHelperData = NULL;
	ErrorBox = FALSE;
	ErrorBeep = 0;
	NumThreads = 0;
	StackSize = 4096;
	ErrorHelperThread = new inkNetwork("Error Helper", ErrorHelper, NULL, _tSUSPENDED, NULL);
	return(0);
}


// Stop Thread Service
void inkNetwork::StopService(void)
{
	ErrorHelperExit = TRUE;
	WaitTillSuspended(ErrorHelperThread);
	ErrorHelperThread->Resume();
	WaitTillSuspended(ErrorHelperThread);
	delete ErrorHelperThread;
	return(0);
}


// Create the Main Thread
inkNetwork* inkNetwork::CreateMain(TCHAR* thread_descrip, void* func, void* param, DWORD start, void* err_handler)
{
	inkNetwork *TheMain = new inkNetwork(thread_descrip, func, param, start, err_handler);
	inkNetwork::MainThread = TheMain;
	return(TheMain);
}


// Create a new Thread
inkNetwork::inkNetwork(TCHAR* thread_descrip, void* func, void* param, DWORD start, void* err_handler)
{
	void (*MainErrorHandler)(void*);
	Running = FALSE;
	Active = FALSE;
	Handle = (HANDLE)0;
	ID = 0;
	Description = thread_descrip;														// Save the Thread's description
	ErrorHandler = err_handler;															// Save the Error Handler for the Thread
	if(err_handler == NULL) ErrorHandler = MainThread->ErrorHandler;					// Not given, set it to the Main Thread's Error handler
	MessageQueue = (UINT*) GlobalAlloc(GMEM_FIXED, _tMESSAGEQUEUESIZE);					// Allocate memory for Message Queue
	if(MessageQueue = NULL)																// Falied ? , inform this error to Main Thread
	{
		ErrorCode = 0;
		ErrorString = NULL;
		ErrorAct = 0;
		MainErrorHandler = MainThread->ErrorHandler;
		*MainErrorHandler((void*)this);
		return;
	}
	MessageQueueNum = 0;
	MessageQueueFront = 0;
	MessageQueueRear = 0;
	NumThreads++;
	Running = TRUE;
	if(start == _tACTIVE) Active = TRUE;
	Handle = CreateThread(NULL, StackSize, (LPTHREAD_START_ROUTINE)func, param, start, &ID);
}


// Delete an existing Thread (from another thread) (try not to use this unless really req.)
inkNetwork::~inkNetwork(UINT exit_code)
{
	if(Running == TRUE)	
	{
		NumThreads--;
		Running = FALSE;
		Active = FALSE;
		TerminateThread(Handle, exit_code);
	}
}


// Give a Message to a thread (susecptible to multithreading deadlocks)
void inkNetwork::GiveMessage(UINT msg_type, UINT msg_data1, UINT msg_data2, UINT msg_data3)
{
	if(MessageQueueNum < _tMESSAGEQUEUENUMMAX)
	{
		MessageQueueNum++;
		MessageQueue[MessageQueueRear] = msg_type;
		MessageQueueRear++;
		MessageQueue[MessageQueueRear] = msg_data1;
		MessageQueueRear++;
		MessageQueue[MessageQueueRear] = msg_data2;
		MessageQueueRear++;
		MessageQueue[MessageQueueRear] = msg_data3;
		MessageQueueRear++;
		if(MessageQueueRear >= _tMESSAGEQUEUEEND) MessageQueueRear = 0;
	}
}


// Give a Message to a thread (susecptible to multithreading deadlocks)
void inkNetwork::RecieveMessage(UINT *msg_type, UINT *msg_data1, UINT *msg_data2, UINT *msg_data3)
{
	if(MessageQueueNum > 0)
	{
		MessageQueueNum--;
		*msg_type = MessageQueue[MessageQueueFront];
		MessageQueueFront++;
		*msg_data1 = MessageQueue[MessageQueueFront];
		MessageQueueFront++;
		*msg_data2 = MessageQueue[MessageQueueFront];
		MessageQueueFront++;
		*msg_data3 = MessageQueue[MessageQueueFront];
		MessageQueueFront++;
		if(MessageQueueFront >= _tMESSAGEQUEUEEND) MessageQueueFront = 0;
	}
}


// Stop the current thread (from same thread) (best way to stop a thread)
void inkNetwork::Stop(UINT exit_code)
{
	NumThreads--;
	Running = FALSE;
	Active = FALSE;
	ExitThread(exit_code);
}


// Wait till the given thread becomes active
void inkNetwork::WaitTillActive(inkNetwork* thread)
{
	while(thread->Active == FALSE);
}


// Wait till the given thread becomes suspended
void inkNetwork::WaitTillSuspended(inkNetwork* thread)
{
	while(thread->Active == TRUE);
}


// Resume a Thread
void inkNetwork::Resume(void)
{
	if(Active == FALSE)
	{
		Active = TRUE;
		ResumeThread(Handle);
	}
}


// Suspend a Thread
void inkNetwork::Suspend(void)
{
	if(Active == TRUE)
	{
		Active = FALSE;
		SuspendThread(Handle);
	}
}


// Get the Exit Code of the Thread
UINT inkNetwork::GetExitCode(void)
{
	UINT exit_code = 0;
	if(Running = FALSE) GetExitCodeThread(ID, &exit_code);
	return(exit_code);
}


// Report Error Code to the Error Handler
void inkNetwork::Error(UINT err_code)
{
	ErrorCode = err_code;
	ErrorTransfer(this);
}


// Report Error String to the Error Handler
void inkNetwork::ErrorStr(char* err_str)
{
	ErrorString = err_str;
	ErrorTransfer(this);
}


// Report Error Code and Suggested Action to Error Handler
void inkNetwork::ErrorAct(UINT err_code, UINT act)
{
	ErrorCode = err_code;
	ErrorAction = act;
	ErrorTransfer(this);
}


// Report Error String and Suggested Action to Error Handler
void inkNetwork::ErrorStrAct(char* err_str, UINT act)
{
	ErrorString = err_str;
	ErrorAction = act;
	ErrorTransfer(this);
}


void inkNetwork::ErrorTransfer(inkNetwork *thread_obj)
{
	WaitTillSuspended(ErrorHelperThread);
	ErrorHelperData = thread_obj;
	ErrorHelperThread->Resume();
	WaitTillSuspended(ErrorHelperThread);
}


void inkNetwork::ErrorHelper(void *no_use)
{
	UNREFERENCED_PARAMETER(no_use);
	TCHAR *ErrorDisplay;
	void (*error_handler)(inkNetwork*);
	
	while(1)
	{
		if(ErrorHelperExit == TRUE) ErrorHelperThread->Stop(0);
		if(ErrorHelperData->ErrorCode == 0) ErrorHelperData->ErrorCode = GetLastError();
		if(ErrorHelperData->ErrorString == NULL) ErrorHelperData->ErrorString = _T("");
		//if(ErrorHelperData->ErrorStr == NULL) readstr from file
		if(ErrorBox == TRUE)
		{
			ErrorDisplay = (TCHAR*) GlobalAlloc(GMEM_FIXED, 512);
			wsprintf(ErrorDisplay, "Error Code: %d\nError String: %s\nError Action: %d\nThread ID: %d\nThread Description: %s\n", 
			ErrorHelperData->ErrorCode, ErrorHelperData->ErrorString, ErrorHelperData->ErrorAction,	ErrorHelperData->ID, ErrorHelperData->Description);
			MessageBox(NULL, ErrorDisplay, _T("Internal Error"), MB_OK|MB_ICONEXCLAMATION);
			GlobalFree(ErrorDisplay);
		}
		if(ErrorBeep != 0) MessageBeep(ErrorBeep);
		error_handler = ErrorHelperData->ErrorHandler;
		*error_handler(ErrorHelperData);
		ErrorHelperThread->Suspend();
	}
}



#endif

