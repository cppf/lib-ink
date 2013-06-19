/****************************************************************************
 *                                                                          *
 * File    : graphics.c                                                     *
 *                                                                          *
 * Purpose : OpenGL Graphics Routines for C                                 *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           1/9/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_graphics_c_

#define	_graphics_c_		loaded




// ----Includes------------------------------------------------------
#include "win32.h"
#include "macro.h"



// ----Prototypes----------------------------------------------------
PROC	inkStartGraphicsService(void);
PROC	inkStopGraphicsService(void);
PROC	Msg_WM_PAINT(_MSG_PARAMS);
PROC	Msg_WM_SIZE(_MSG_PARAMS);
PROC	Msg_WM_CREATE(_MSG_PARAMS);
PROC	Msg_WM_DESTROY(_MSG_PARAMS);





// ----Externals-----------------------------------------------------
extern HDC					ink_MainHDC;
extern HWND					ink_MainHWND;
extern UINT					inkWindowWidth;
extern UINT					inkWindowHeight;
extern void*				WM_Handler[];
extern int					inkGetKey(void);





// ----Global Data---------------------------------------------------
static HGLRC				ink_hGLrc;
static GLfloat				ink_AngleX = 0.1;






// ----Code----------------------------------------------------------


// Start the graphics service (need not be called)
PROC	inkStartGraphicsService(void)
	{
	PIXELFORMATDESCRIPTOR	pfd;
	int						PixFormat;

	WM_Handler[WM_PAINT] = Msg_WM_PAINT;
	WM_Handler[WM_SIZE] = Msg_WM_SIZE;
	WM_Handler[WM_CREATE] = Msg_WM_CREATE;
	WM_Handler[WM_DESTROY] = Msg_WM_DESTROY;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = GL_VERSION_1_1;
	pfd.bReserved = 0;
	pfd.cAccumBits = 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cAccumBlueBits = 0;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumRedBits = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cBlueBits = 0;
	pfd.cBlueShift = 0;
	pfd.cGreenBits = 0;
	pfd.cGreenShift = 0;
	pfd.cRedBits = 0;
	pfd.cRedShift = 0;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;
	pfd.cStencilBits = 0;
	pfd.cAuxBuffers = 0;
	pfd.dwDamageMask = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER|PFD_DRAW_TO_WINDOW;
	PixFormat = ChoosePixelFormat(ink_MainHDC, &pfd);
	SetPixelFormat(ink_MainHDC, PixFormat, &pfd);
	ink_hGLrc = wglCreateContext(ink_MainHDC);
	wglMakeCurrent(ink_MainHDC, ink_hGLrc);
	glViewport(0, 0, inkWindowWidth, inkWindowHeight);
	glEnable(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	return(0);
	}



// Stop graphics service (need not be called)
PROC	inkStopGraphicsService(void)
	{
 	wglMakeCurrent(0, 0);
	wglDeleteContext(ink_hGLrc);
	return(0);
	}



// Perform graphics operations
PROC	Msg_WM_PAINT(_MSG_PARAMS)
	{
	DWORD	i, j;
	int key;
	key = inkGetKey();
	if(key == VK_LEFT)
	glRotatef(ink_AngleX, 0.0, 1.0, 0.0);
	else if(key == VK_RIGHT)
	glRotatef(-ink_AngleX, 0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, 0.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(0.0, 1.0, 1.0);

	for(i=0; i<10;i++)
		{
		for(j=0; j<4; j++)
			{
			glColor3f(0.024f*i+0.2f*j+0.1f, 0.024f*i+0.1f*j+0.2f, 0.024f*i+0.15f*j+0.15f);
			glVertex4f(-0.0012f*i*i+0.06f*j, -0.024f*i+0.06f*j*(4-j), -0.0010f*i*(50-i)+0.06f*j,0.9f);
			}
		}

	glEnd();
	SwapBuffers(ink_MainHDC);
	return(0);
	}



// Resize the viewport
PROC	Msg_WM_SIZE(_MSG_PARAMS)
	{
	glViewport(0, 0, inkWindowWidth, inkWindowHeight);
	return(0);
	}



// Resize the viewport
PROC	Msg_WM_CREATE(_MSG_PARAMS)
	{
	ink_MainHDC = GetDC(ink_MainHWND);
	inkStartGraphicsService();
	return(0);
	}



// Resize the viewport
PROC	Msg_WM_DESTROY(_MSG_PARAMS)
	{
	inkStopGraphicsService();
	ReleaseDC(NULL, ink_MainHDC);
	return(0);
	}








#endif

