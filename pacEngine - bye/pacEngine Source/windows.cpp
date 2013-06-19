/****************************************************************************
 *                                                                          *
 * File    : windows.cpp                                                    *
 *                                                                          *
 * Purpose : The Window module of pacEngine                                 *
 *                                                                          *
 * History : Date        Part of pacEngine                                  *
 *           07/12/2011  Subhajit Sahu                                      *
 *                                                                          *
 ****************************************************************************/

/* Include Files */
#include <windows.h>



#define		_ITEM_HOLDER	\
	UINT	Type;\
	UINT	Data;
	
typedef struct _ITEM
{
	_ITEM_HOLDER
}ITEM;


// Location (of a pixel)
typedef struct	_LOCATION
{
	UINT	x;
	UINT	y;
}LOCATION;

// Location in DWORD
typedef struct	_LOCATION_DW
{
	DWORD	x;
	DWORD	y;
}LOCATION_DW;



typedef struct	_PACKET_DISPLAY_SETTINGS
{
	LOCATION_DW		Resolution;
	DWORD			BitsPerPixel;
	DWORD			RefreshRate;
}PACKET_DISPLAY_SETTINGS;










static HDC			pacWindowsHDC;
static HWND			pacWindowsHWND;
static HINSTANCE	pacWindowsHInstance;







BOOL	pacWindowGetDisplaySettings(PACKET_DISPLAY_SETTINGS *pac)
{
	DEVMODE		DevMode;
	
	DevMode.dmSize = sizeof(DEVMODE);
	DevMode.dmDriverExtra = 0;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);
	pac->Resolution.x = ;
	pac->Resolution.y = ;
	pac->BitsPerPixel = ;
	pac->RefreshRate = ;
}
























LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc




GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return TRUE;										// Initialization Went OK
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix
	return TRUE;										// Everything Went OK
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/







static BOOL		pacWindowResize;			// Shall the window be allowed to resize?
static BOOL		pacWindowCloseDisabled;		// Close Button disables?
 
 
 
 
 
 
 
 PACKET*	pacWindowCreateSettingsPacket(UINT win_type)
 {
	PACKET_WINDOW_CREATE*	pac;
	
	pac = pacMemAlloc(sizeof(PACKET_WINDOW_CREATE));
	pac->PacketSize = sizeof(PACKET_WINDOW_CREATE);
	pac->DataType = ;
	pac->DataSize = sizeof(PACKET_WINDOW_CREATE) - sizeof(PACKET);
	
	switch(win_type)
	{
	case	WINDOW_TYPE_GAME_FULL_SCREEN:
		pac->WindowResize = FALSE;
		pac->WindowCloseDisabled = FALSE;
		pac->WindowProcedure = pacWindowProcedure;
		pac->WindowIcon.Type = 0;
		pac->WindowIcon.Data = 0;
		pac->WindowCursor.Type = 0;
		pac->WindowCursor.Data = 0;
		pac->WindowBackground.Type = 0;
		pac->WindowBackground.Data = 0;
		pac->WindowMenu.Type = 0;
		pac->WindowMenu.Data = 0;
		pac->WindowClassName = _T("pacWindow");
		pac->WindowExStyle 
		break;
	}
	
	pac->PacketType = ;
 }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
BOOL		pacWindowCreate(PACKET_WINDOW_REGISTER* pac)
{
	WNDCLASSEX				wc;
	
	// First Check if the Window of the Given Class Name already exists
	if(!GetClassInfoEx(pacWindowHInstance, pac->WindowClassName, &wc))
	{
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_PARENTDC | CS_DBLCLKS | ((ret->WindowResize)?(CS_HREDRAW|CS_VREDRAW):0) | ((ret->WindowCloseDisabled)?CS_NOCLOSE:0);
		wc.lpfnWndProc = pac->WindowProcedure;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = pacWindowHInstance;
		
		// Deal with Window Icon
		switch(ret->WindowIcon.Type)
		{
		case	ITEM_TYPE_FILE_NAME:
			wc.hIcon = LoadImage(NULL, (LPCTSTR)(ret->WindowIcon.Data), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_RESOURCE_NAME:
			wc.hIcon = LoadImage(pacWindowHInstance, (LPCTSTR)(ret->WindowIcon.Data), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_RESOURCE_NUMBER:
			wc.hIcon = LoadImage(pacWindowHInstance, MAKEINTRESOURCE(ret->WindowIcon.Data), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_OEM_IMAGE:
			wc.hIcon = LoadImage(NULL, (LPCTSTR)(ret->WindowIcon.Data), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		default:
			wc.hIcon = NULL;
		}
		
		// Deal with Window Cursor
		switch(ret->WindowCursor.Type)
		{
		case	ITEM_TYPE_FILE_NAME:
			wc.hCursor = LoadImage(NULL, (LPCTSTR)(ret->WindowCursor.Data), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_RESOURCE_NAME:
			wc.hCursor = LoadImage(pacWindowHInstance, (LPCTSTR)(ret->WindowCursor.Data), IMAGE_CURSOR, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_RESOURCE_NUMBER:
			wc.hCursor = LoadImage(pacWindowHInstance, MAKEINTRESOURCE(ret->WindowCursor.Data), IMAGE_CURSOR, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		case	ITEM_TYPE_OEM_IMAGE:
			wc.hCursor = LoadImage(NULL, (LPCTSTR)(ret->WindowCursor.Data), IMAGE_CURSOR, 0, 0, LR_LOADTRANSPARENT);
			break;
		
		default:
			wc.hCursor = NULL;
		}
		
		// Deal with Window Background
		switch(ret->WindowBackground.Type)
		{
		case	ITEM_TYPE_FILE_NAME:
			wc.hbrBackGround = CreatePatternBrush((HBITMAP)LoadImage(NULL, (LPCTSTR)(ret->WindowBackground.Data), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
			break;
		
		case	ITEM_TYPE_RESOURCE_NAME:
			wc.hbrBackGround = CreatePatternBrush((HBITMAP)LoadImage(pacWindowHInstance, (LPCTSTR)(ret->WindowBackground.Data), IMAGE_BITMAP, 0, 0, 0));
			break;
		
		case	ITEM_TYPE_RESOURCE_NUMBER:
			wc.hbrBackGround = CreatePatternBrush((HBITMAP)LoadImage(pacWindowHInstance, MAKEINTRESOURCE(ret->WindowBackground.Data), IMAGE_BITMAP, 0, 0, 0));
			break;
		
		case	ITEM_TYPE_OEM_NUMBER:
			wc.hbrBackGround = CreatePatternBrush((HBITMAP)LoadImage(NULL, (LPCTSTR)(ret->WindowBackground.Data), IMAGE_BITMAP, 0, 0, 0));
			break;	
		
		case	ITEM_RGBCOLOR:
			wc.hbrBackground = CreateSolidBrush((COLORREF)(ret->WindowsBackground.Data));
			break;
		
		case	ITEM_TYPE_OEM_COLOR:
			wc.hbrBackground = (ret->WindowsBackground.Data);
			break;
			
		default:
			wc.hbrBackground = NULL;
		}
	
		// Deal with Window Menu
		switch(ret->WindowMenu.Type)
		{
		case	ITEM_TYPE_RESOURCE_NAME:
			wc.lpszMenuName = (LPCTSTR)(ret->WindowMenu.Data);
			break;
		
		case	ITEM_TYPE_RESOURCE_NUMBER:
			wc.lpszMenuName = MAKEINTRESOURCE(ret->WindowMenu.Data);
			break;
		
		default:
			wc.lpszMenuName = NULL;
		}
		wc.lpszClassName = pac->WindowClassName;
		wc.hIconSm = NULL;
		if(!RegisterClassEx(&wc))
		{
			pacError = 0;
			pacErrorString = _T("Registration of Window failed");
			return(FALSE);
		}
	}
	
	// Create the Window Now
	switch(ret->WindowMenu.Type)
	{
	case	ITEM_TYPE_RESOURCE_NAME:
		wc.lpszMenuName = (LPCTSTR)(ret->WindowMenu.Data);
		break;
	
	case	ITEM_TYPE_RESOURCE_NUMBER:
		wc.lpszMenuName = MAKEINTRESOURCE(ret->WindowMenu.Data);
		break;
	
	default:
		wc.lpszMenuName = NULL;
	}
	CreateWindowEx(pac->WindowExStyle, pac->WindowClassName, ret->WindowName, pac->WindowStyle, pac->WindowPositionX, pac->WindowPositionY, 
	pac->WindowWidth, pac->WindowHeight, NULL, LoadMenu(pacWindowHInstance, wc.lpszMenuName), pacWindowHInstance, NULL);
}


























BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}







_WPROC		pacWindowService(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_ACTIVATE:
	if(HIWORD(wParam)) win selected
	else minimized
	
	
	case 
	}
}
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("NeHe's OpenGL Framework",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's OpenGL Framework",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
