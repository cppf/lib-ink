/****************************************************************************
 *                                                                          *
 * File    : engine.cpp                                                     *
 *                                                                          *
 * Purpose : The main engine code of inkEngine                              *
 *                                                                          *
 * History : Date      		Part of INK Game Engine                         *
 *           14/07/2011		Subhajit Sahu                                   *
 *                                                                          *
 ****************************************************************************/

#ifndef		_engine_cpp_

#define		_engine_cpp_		1



// ----Includes------------------------------------------------------
#include "win32.h"
#include "macro.h"
#include <winbase.h>


// ----Global Prototypes---------------------------------------------





// ----Externals-----------------------------------------------------
extern void*		WM_Handler[];





// ----Constants-----------------------------------------------------
#define			_ink_PROCESSOR_unknown					0
#define			_ink_PROCESSOR_x86						1
#define			_ink_PROCESSOR_IA64						2
#define			_ink_PROCESSOR_x64						3

#define			_ink_PROCESSOR_SUPPORTS_3DNOW			PF_3DNOW_INSTRUCTIONS_AVAILABLE
#define			_ink_PROCESSOR_SUPPORTS_CHANNELS		PF_CHANNELS_ENABLED
#define			_ink_PROCESSOR_SUPPORTS_CMPXCHG			PF_COMPARE_EXCHANGE_DOUBLE
#define			_ink_PROCESSOR_SUPPORTS_CMPXCHG16B		PF_COMPARE_EXCHANGE128
#define			_ink_PROCESSOR_SUPPORTS_CMP8XCHG16		PF_COMPARE64_EXCHANGE128
#define			_ink_PROCESSOR_SUPPORTS_FPU				PF_FLOATING_POINT_EMULATED
#define			_ink_PROCESSOR_SUPPORTS_FPUPRECISE		PF_FLOATING_POINT_PRECISION_ERRATA
#define			_ink_PROCESSOR_SUPPORTS_MMX				PF_MMX_INSTRUCTIONS_AVAILABLE
#define			_ink_PROCESSOR_SUPPORTS_DEP				PF_NX_ENABLED
#define			_ink_PROCESSOR_SUPPORTS_PAE				PF_PAE_ENABLED
#define			_ink_PROCESSOR_SUPPORTS_RDTSC			PF_RDTSC_INSTRUCTION_AVAILABLE
#define			_ink_PROCESSOR_SUPPORTS_SSE3			PF_SSE3_INSTRUCTIONS_AVAILABLE
#define			_ink_PROCESSOR_SUPPORTS_SSE				PF_XMMI_INSTRUCTIONS_AVAILABLE
#define			_ink_PROCESSOR_SUPPORTS_SSE2			PF_XMMI64_INSTRUCTIONS_AVAILABLE
#define			_ink_PROCESSOR_SUPPORTS_XSAVEXRSTOR		PF_XSAVE_ENABLED

#define			_ink_OS_WIN32_NT						VER_PLATFORM_WIN32_NT

#define			_ink_OS_NT_SERVER_DOMAIN_CONTROLLER		VER_NT_DOMAIN_CONTROLLE
#define			_ink_OS_NT_SERVER						VER_NT_SERVER
#define			_ink_OS_NT_WORKSTATION					VER_NT_WORKSTATION

#define			_ink_PROCESSOR_MANUFACTURER_UNKNOWN		0
#define			_ink_PROCESSOR_MANUFACTURER_INTEL		1
#define			_ink_PROCESSOR_MANUFACTURER_AMD			2



// ----Macros--------------------------------------------------------






// ----Structures----------------------------------------------------
typedef struct _COMPUTER_NAME
{
	TCHAR*	Local;
	TCHAR*	Network;
}COMPUTER_NAME;

typedef struct _PROCESSOR_SUPPORT
{
	BOOL	CpuID;
	BOOL	BrandString;
	BOOL	BrandID;
}PROCESSOR_SUPPORT;

typedef struct _PROCESSOR_INFO
{
	WORD				Manufacturer;
	PROCESSOR_SUPPORT	Support;
	DWORD				Architecture;
	DWORD				ActiveMask;
	DWORD				Cores;
}PROCESSOR_INFO;

typedef struct _SERVICE_PACK_INFO
{
	TCHAR*	Name;
	DWORD	Version;
}SERVICE_PACK_INFO;

typedef struct _OS_INFO
{
	TCHAR*				Name;
	DWORD				Version;
	DWORD				BuildNumber;
	DWORD				PlatformID;
	SERVICE_PACK_INFO	ServicePack;
	WORD				SuiteMask;
	BYTE				ProductType;
	TCHAR*				CoreDirectory;
	TCHAR*				SystemDirectory;
}OS_INFO;




// ----Code----------------------------------------------------------






/*
Class:	inkInput

First of all inkInput::StartService() must be called before any other
function is used.
At the end of the game, inkInput::StopService() must be called before exit.
*/
class inkThread
{
	// Global/Static Data
	public:
	static COMPUTER_NAME			ComputerName;
	static TCHAR*					UserName;
	static TCHAR*					SystemDirectory;
	static OS_INFO					OS;
	static PROCESSOR_INFO			Processor;
	private:
	static TCHAR*					ProcessorNames[];


	// Input Handling Functions Declarations
	public:
	int		StartService(char mode, void* control);
	int		StopService(void);
};
TCHAR*		ProcessorNames[] = {_T("Intel Celeron processor"),_T( "Intel Pentium III processor"), _T("Intel Pentium III Xeon processor"), 
								_T("Intel Pentium III processor"), _T("Unknown processor"), _T("Mobile Intel Pentium III processor-M"), 
								_T("Mobile Intel Celeron processor"), _T("Intel Pentium 4 processor"), _T("Intel Pentium 4 processor"),
								_T("Intel Celeron processor"), _T("Intel Xeon processor"), _T("Intel Xeon processor MP"), _T("Unknown processor"), 
								_T("Mobile Intel Pentium 4 processor-M"), _T("Mobile Intel Celeron processor"), _T("Unknown processor"), 
								_T("Mobile Genuine Intel processor"), _T("Intel Celeron M processor"), _T("Mobile Intel Celeron processor"), 
								_T("Intel Celeron processor"), _T("Mobile Genuine Intel processor"), _T("Intel Pentium M processor"), 
								_T("Mobile Intel Celeron processor")};





// Start the Input Service with Text Mode(1) or Gaming Mode(2)
int		inkEngine::StartService(char mode, void* control)
{
	UINT					i;
	TCHAR					*buffer;
	SYSTEM_INFO				*sys_info;
	OSVERSIONINFOEX			*os_ver;
	COMPUTER_NAME_FORMAT	comp_name;

	buffer = inkString::alloc(512);	
	// Determine Compter Name(local name & net name), User Name, OS Core directory and System directory
	ComputerName.Local = NULL;
	ComputerName.Network = NULL;
	UserName = NULL;
	SystemDirectory = NULL;
	comp_name = ComputerNamePhysicalNetBIOS;
	if(GetComputerNameEx(comp_name, buffer, 512)) inkString::strcpy_alloc(ComputerName.Local, buffer);
	comp_name = ComputerNamePhysicalDnsFullyQualified;
	if(GetComputerNameEx(comp_name, buffer, 512)) inkString::strcpy_alloc(ComputerName.Network, buffer);
	if(GetUserName(buffer, 512))inkString::strcpy_alloc(UserName, buffer);
	if(GetSystemDirectory(buffer, 512)) inkString::strcpy_alloc(SystemDirectory, buffer);
	if(GetWindowsDirectory(buffer, 512)) inkString::
	
	// Get Processor Information and Capabilities from Windows (+ memory details)
	sys_info = (SYSTEM_INFO*)buffer;
	GetSystemInfo(sys_info);
	switch(sys_info->wProcessorArchitecture)
	{
		case 0:
		ProcessorArchitecture = _ink_PROCESSOR_x86;
		break;
		
		case 6:
		ProcessorArchitecture = _ink_PROCESSOR_IA64;
		break;
		
		case 9:
		ProcessorArchitecture = _ink_PROCESSOR_x64;
		break;
		
		default:
		ProcessorArchitecture = _ink_PROCESSOR_unknown;
	}
	MemoryPageSize = sys_info->dwPageSize;
	AvailableRAM = (UINT)sys_info->lpMaximumApplicationAddress - (UINT)sys_info->lpMinimumApplicationAddress;
	ActiveProcessorMask = sys_info->dwActiveProcessorMask;
	NumberOfProcessors = sys_info->dwNumberOfProcessors;
	MemoryAllocationGranularity = sys_info->dwAllocationGranularity;
	ProcessorFeatures = 0;
	for(ret=0; ret<=17; ret++)
		if(IfProcessorFeaturePresent(ret)) ProcessorFeatures |= (1<<ret);
	ProcessorFeatures ^= (1<<_ink_PROCESSOR_SUPPORTS_FPU);
	ProcessorFeatures ^= (1<<_ink_PROCESSOR_SUPPORTS_FPUPRECISE);

	// Get Windows Version
	os_ver = (OSVERSIONINFOEX*)buffer;
	os_ver->dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(GetVersionEx(os_ver))
	{
		OS.Version = ((os_ver->dwMajorVersion)<<16) | (os_ver->dwMinorVersion);
		OS.BuildNumber = os_ver->dwBuildNumber;
		OS.PlatformID = os_ver->dwPlatformId;
		inkString::strcpy_alloc(OS.ServicePack.Name, os_ver->szCSDVersion);
		OS.ServicePack.Version = ((os_ver->wServicePackMajor)<<16) | (os_ver->wServicePackMinor);
		if(OS.Version == 0x00050000)
			OS.Name = _T("Windows 2000");
		else if(OS.Version == 0x00050001)
			OS.Name = _T("Windows XP");
		else if((OS.Version == 0x00050002) && (OS.ProductType == _ink_OS_NT_WORKSTATION) && (Processor.Architecture == _ink_PROCESSOR_x64))
			OS.Name = _T("Windows XP Professional x64 Edition");
		else if((OS.Version == 0x00050002) && (GetSystemMetrics(SM_SERVERR2) == 0))
			OS.Name = _T("Windows Server 2003");
		else if((OS.Version == 0x00050002) && (OS.SuiteMask & VER_SUITE_WH_SERVER))
			OS.Name = _T("Windows Home Server");
		else if((OS.Version == 0x00050002) && (GetSystemMetrics(SM_SERVERR2) != 0))
			OS.Name = _T("Windows Server 2003 R2");
		else if((OS.Version == 0x00060000) && (OS.ProductType == _ink_OS_NT_WORKSTATION))
			OS.Name = _T("Windows Vista");
		else if((OS.Version == 0x00060000) && (OS.ProductType != _ink_OS_NT_WORKSTATION))
			OS.Name = _T("Windows Server 2008");
		else if((OS.Version == 0x00060001) && (OS.ProductType != _ink_OS_NT_WORKSTATION))
			OS.Name = _T("Windows Server 2008 R2");
		else if((OS.Version == 0x00060001) && (OS.ProductType == _ink_OS_NT_WORKSTATION))
			OS.Name = _T("Windows 7");
		else
			OS.Name = _T("Windows Operating System");
	}


	// Re-Determine Processor Information and capabilities (using assembly code, if allowed)
	asm
	{
		// Check if cpuid instruction is allowed
		.386
		pushfd
		pop		ecx
		mov		eax, ecx
		xor		eax, 200000h
		push	eax
		popfd
		pushf
		pop		eax
		cmp		eax, ecx
		je		old_processor_present				// CPUID not supported. inkEngine will exit
		mov		Processor.Support.CpuID, TRUE
		
		// cpuid is supported
		.586
		mov		eax, 0
		cpuid
		cmp		eax, 'Genu'
		jne		non_intel_processor
		cmp		edx, 'ineI'
		jne		non_intel_processor
		cmp		ecx, 'ntel'
		jne		non_intel_processor
		mov		Processor.Manufacturer, _ink_PROCESSOR_MANUFACTURER_INTEL
		
		// It is a Genuine Intel processor
		mov		eax, 80000000h
		cpuid
		cmp		eax, 80000004h
		jae		brand_string_supported
		mov		Processor.Support.BrandString, FALSE
		
		// Brand String not supported
		mov		eax, 1
		cpuid
		mov		Processor.Signature, eax
		and		ebx, 0FFh
		jz		brand_id_not_supported
		mov		Processor.BrandID, bl
		
		brand_string_supported:
		mov		eax, 80000002h
		mov		edi, OFFSET Processor.Name
		cpuid
		call	store_brand_name
		mov		eax, 80000003h
		cpuid
		call	store_brand_name
		mov		eax, 80000004h
		cpuid
		call	store_brand_name
		jmp		asm_processor_inf_end
		
		store_brand_name:
		mov DWORD PTR[edi], eax
		mov DWORD PTR [edi+4], ebx
		mov DWORD PTR [edi+8], ecx
		mov DWORD PTR [edi+12], edx
		ret
		
		old_processor_present:
		mov		Processor.Support.CpuID, FALSE
		jmp		asm_processor_inf_end
		
		non_intel_processor:
		// Check if AMD then?
		mov		Processor.Manufacturer, _ink_PROCESSOR_MANUFACTURER_UNKNOWN
		jmp		asm_processor_inf_end
		
		asm_processor_inf_end:
	}
	Processor.Name = ProcessorNames[Processor.BrandID - 1];
	switch(Processor.BrandID)
	{
		case 0x03:
		if(Processor.Signature == 0x6B1)
			Processor.Name = _T("Intel Celeron processor");
		break;
		
		case 0x0B:
		if(Processor.Signature == 0xF13)
			Processor.Name = _T("Intel Xeon processor MP");
		break;
		
		case 0x0E:
		if(Processor.Signature == 0xF13)
			Processor.Name = _T("Intel Xeon processor");
		break;
	}
}








// NetWkstaGetInfo 
// EnumDisplayMonitors 





int		inkInput::StopService(void)
	{
	if(!KeyMode)return(0);
	KeyMode = 0;
	GlobalFree(BaseMem);
	return(0);
	}


int		inkInput::GetKey(void)
	{
	int	k;
	if(KeyMode == 2)
		{
		inkDeleteQueue_Game(k);
		return(k);
		}
	inkDeleteQueue_Text(k);
	return(k);
	}


int2	inkInput::GetMouse(void)
	{
	int2 k;
	if(KeyMode == 1)
		inkDeleteQueue_Mouse(k.loint, k.hiint);
	return(k);
	}



// Empties all the input buffers
void	inkInput::Flush(void)
	{
	TextItems = 0;
	TextFront = 0;
	TextRear = 0;
	GameItems = 0;
	GameFront = 0;
	GameRear = 0;
	MouseItems = 0;
	MouseFront = 0;
	MouseRear = 0;
	}




int		inkInput::IsKeyPressed(int key)
	{
	if(KeyMode == 2)
		return(inkKeyPressed_Game(key) > 0);
	return(-1);
	}




void	inkInput::SetKeyControl(int key, int control)
	{
	if(KeyMode == 2)
		Control[key & 0xFF] = (unsigned char)(control & 0xFF);
	}


int		inkInput::GetKeyControl(int key)
	{
	if(KeyMode == 2)
		return(Control[key & 0xFF]);
	return(-1);
	}



// Load custom controls or the default one (NULL)
void	inkInput::LoadKeyControls(void* ctrl)
	{
	int i;
	
	if(KeyMode == 2)
		{
		// Load default controls ?
		if(ctrl == NULL)
			{
			for(i=0; i<_In_ControlSizeB; i++)
				Control[i] = i;
			return;
			}
		CopyMemory(Control, ctrl, _In_ControlSizeB);
		}
	}



// Save the current controls to a memory location
void	inkInput::SaveKeyControls(void* ctrl)
	{
	if(KeyMode == 2)
		CopyMemory(ctrl, Control, _In_ControlSizeB);
	}




int		inkInput::GetPressedKeys(void)
	{
	LPUINT status;
	int i, j, c;

	if(KeyMode == 2)
		{
		c = 0;
		status = GameStatus;
		for(i=0; i<_In_GameStatusSizeB; i+=4)
			{
			if(status[i] == 0)continue;
			j = 0;
			c = 1;
			while(status[i] != 0)
				{
				if(status[i] & 1){ inkInsertQueue_Game(Control[(i<<5)|j]|0x400); }
				status[i] = status[i] >> 1;
				j++;
				}
			}
		}
	return(-1);
	}



// Get the number of butons on the mouse (0 = no mouse)
// Get the keyboard initial delay time
int		inkInput::GetKeyboardDelay(void)
	{
	UINT x;
	LPUINT y;
	y = &x;
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, y, 0);
	return(x);
	}



// Set the keyboard initial delay time
void	inkInput::SetKeyboardDelay(UINT x)
	{
	SystemParametersInfo(SPI_SETKEYBOARDDELAY, x, NULL, 0);
	}



// Get the keyboard speed / repeat rate
int		inkInput::GetKeyboardSpeed(void)
	{
	UINT x;
	LPUINT y;
	y = &x;
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, y, 0);
	return(x);
	}



// Set the keyboard speed / repeat rate
void	inkInput::SetKeyboardSpeed(UINT x)
	{
	SystemParametersInfo(SPI_SETKEYBOARDSPEED, x, NULL, 0);
	}



// Get mouse x and y threshold values and mouse speed in 3 integer array
void	inkInput::GetMouseInformation(_LPUINT x)
	{
	SystemParametersInfo(SPI_GETMOUSE, 0, x, 0);
	}



// Set mouse x and y threshold values and mouse speed in 3 integer array
void	inkInput::SetMouseInformation(_LPUINT x)
	{
	SystemParametersInfo(SPI_SETMOUSE, 0, x, 0);
	}



// WM_CHAR Message Handler (Text)
PROC	Msg_WM_CHAR(_MSG_PARAMS)
	{
	inkInsertQueue_Text(wParam);
	return(0);
	}



// WM_SYSCHAR Message Handler (Text)
PROC	Msg_WM_SYSCHAR(_MSG_PARAMS)
	{
	inkInsertQueue_Text(wParam|0x80);
	return(0);
	}



// WM_KEYDOWN Message Handler (Gaming)
PROC	Msg_WM_KEYDOWN(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[wParam];
	k2 = (lParam & 0xE0000000) | (((lParam & 0x0000FFFF) << 8) | k1);
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_SYSKEYDOWN Message Handler (Gaming)
PROC	Msg_WM_SYSKEYDOWN(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[wParam];
	k2 = (lParam & 0xE0000000) | ((((lParam & 0x0000FFFF) << 8) | k1) | 0x10000000);
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_KEYUP Message Handler (Gaming)
PROC	Msg_WM_KEYUP(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[wParam];
	k2 = (lParam & 0xE0000000) | (((lParam & 0x0000FFFF) << 8) | k1);
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_SYSKEYUP Message Handler (Gaming)
PROC	Msg_WM_SYSKEYUP(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[wParam];
	k2 = (lParam & 0xE0000000) | ((((lParam & 0x0000FFFF) << 8) | k1) | 0x10000000);
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_LBUTTONDOWN Message Handler (Text)
PROC	Msg_WM_LBUTTONDOWN_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_LBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_LBUTTONDOWN_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[_In_Key_MouseLeft];
	k2 = (0x40000000 & ((inkKeyPressed_Game(k1) == 0) - 1)) | k1;
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_RBUTTONDOWN Message Handler (Text)
PROC	Msg_WM_RBUTTONDOWN_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_RBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_RBUTTONDOWN_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[_In_Key_MouseRight];
	k2 = (0x40000000 & ((inkKeyPressed_Game(k1) == 0) - 1)) | k1;
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_MBUTTONDOWN Message Handler (Text)
PROC	Msg_WM_MBUTTONDOWN_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_MBUTTONDOWN Message Handler (Gaming)
PROC	Msg_WM_MBUTTONDOWN_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[_In_Key_MouseMiddle];
	k2 = (0x40000000 & ((inkKeyPressed_Game(k1) == 0) - 1)) | k1;
	inkInsertQueue_Game(k2);
	inkKeySet_Game(k1);
	return(0);
	}



// WM_LBUTTONUP Message Handler (Text)
PROC	Msg_WM_LBUTTONUP_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_LBUTTONUP Message Handler (Gaming)
PROC	Msg_WM_LBUTTONUP_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[_In_Key_MouseLeft];
	k2 = 0xC0000000 | k1;
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_RBUTTONUP Message Handler (Text)
PROC	Msg_WM_RBUTTONUP_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_RBUTTONUP Message Handler (Gaming)
PROC	Msg_WM_RBUTTONUP_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[_In_Key_MouseRight];
	k2 = 0xC0000000 | k1;
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_MBUTTONUP Message Handler (Text)
PROC	Msg_WM_MBUTTONUP_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);
	}



// WM_MBUTTONUP Message Handler (Gaming)
PROC	Msg_WM_MBUTTONUP_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = Control[_In_Key_MouseMiddle];
	k2 = 0xC0000000 | k1;
	inkInsertQueue_Game(k2);
	inkKeyReset_Game(k1);
	return(0);
	}



// WM_LBUTTONDBLCLK Message Handler (Text)
PROC	Msg_WM_LBUTTONDBLCLK_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_LBUTTONDBLCLK Message Handler (Gaming)
PROC	Msg_WM_LBUTTONDBLCLK_Gaming(_MSG_PARAMS)
	{
	UINT k;
	k = Control[_In_Key_MouseLeftDblClk];
	inkInsertQueue_Game(k);
	return(0);
	}



// WM_RBUTTONDBLCLK Message Handler (Text)
PROC	Msg_WM_RBUTTONDBLCLK_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_RBUTTONDBLCLK Message Handler (Gaming)
PROC	Msg_WM_RBUTTONDBLCLK_Gaming(_MSG_PARAMS)
	{
	UINT k;
	k = Control[_In_Key_MouseRightDblClk];
	inkInsertQueue_Game(k);
	return(0);
	}



// WM_MBUTTONDBLCLK Message Handler (Text)
PROC	Msg_WM_MBUTTONDBLCLK_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_MBUTTONDBLCLK Message Handler (Gaming)
PROC	Msg_WM_MBUTTONDBLCLK_Gaming(_MSG_PARAMS)
	{
	UINT k;
	k = Control[_In_Key_MouseMiddleDblClk];
	inkInsertQueue_Game(k);
	return(0);
	}



// WM_MOUSEWHEEL Message Handler (Text)
PROC	Msg_WM_MOUSEWHEEL_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_MOUSEWHEEL Message Handler (Gaming)
PROC	Msg_WM_MOUSEWHEEL_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2;
	k1 = (wParam>0)?Control[_In_Key_MouseWheelPlus]:Control[_In_Key_MouseWheelMinus];
	k2 = ((wParam & 0xFFFF0000) >> 8) | k1;
	inkInsertQueue_Game(k2);
	return(0);
	}



// WM_MOUSEMOVE Message Handler (Text)
PROC	Msg_WM_MOUSEMOVE_Text(_MSG_PARAMS)
	{
	inkInsertQueue_Mouse(wParam, lParam);
	return(0);	
	}



// WM_MOUSEMOVE Message Handler (Gaming)
PROC	Msg_WM_MOUSEMOVE_Gaming(_MSG_PARAMS)
	{
	UINT k1, k2, k3, k4;
	int delX, delY;
	delX = (lParam & 0xFFFF) - _inkMouseHoldPositionX;
	delY = (lParam >> 16) - _inkMouseHoldPositionY;
	k1 = ((delX == 0) - 1) & Control[_In_Key_MouseXmove];
	k2 = ((delY == 0) - 1) & Control[_In_Key_MouseYmove];
	k3 = ((delX & 0xFFFF) << 8) | k1;
	k4 = ((delY & 0xFFFF) << 8) | k2;
	inkInsertQueue_Game(k3);
	inkInsertQueue_Game(k4);
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, _inkMouseHoldPositionX, _inkMouseHoldPositionY, 0, 0);
	return(0);
	}







#endif


