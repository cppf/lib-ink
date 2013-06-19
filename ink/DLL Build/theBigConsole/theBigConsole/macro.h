/****************************************************************************
 *                                                                          *
 * File    : macro.h                                                        *
 *                                                                          *
 * Purpose : Some Win32 macros for C                                        *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           9/7/2011  Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_macro_h_
#define	_macro_h_		1

// ----Includes------------------------------------------------------
#include "win32.h"


// ----Type Declarations---------------------------------------------

// 32-bit RGBA pixel
typedef struct _RGBA_PIXEL_DATA
	{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
	}RGBA_PIXEL_DATA;

// 32-bit RGB pixel for editing
typedef union _RGB_PIXEL
	{
	RGBA_PIXEL_DATA		clr;
	DWORD				value;
	}RGB_PIXEL;

// Message Data (for storage)
typedef struct _MSG_DATA
{
	UINT	Type;
	UINT	Param[3];
}MSG_DATA;



// ----Macros--------------------------------------------------------

// Set a 32-bit RGB pixel
#define		_inkSetRGB_PIXEL(_RGBpixel, r, g, b)	\
_RGBpixel.clr.red=r;\
_RGBpixel.clr.green=g;\
_RGBpixel.clr.blue=b;\
_RGBpixel.clr.unused=0;


// Set a RECT
#define		_inkSetRECT(Rectangle, x1, y1, x2, y2)	\
Rectangle.top=y1;\
Rectangle.bottom=y2;\
Rectangle.left=x1;\
Rectangle.right=x2;



// Draw on a Device Context from a BITMAP object opaquely
#define		_inkBitBltOpaque(hdc, xdest, ydest, xres, yres, image, bitmap, xsrc, ysrc)		\
SelectObject(image, bitmap);\
BitBlt(hdc, xdest, ydest, xres, yres, image, xsrc, ysrc, SRCCOPY);


// Draw on a Device Context from a BITMAP object transparently
#define		_inkBitBltTrans(hdc, xdest, ydest, xres, yres, image, bitmap, mask, xsrc, ysrc)	\
SelectObject(image, mask);\
BitBlt(hdc, xdest, ydest, xres, yres, img, xsrc, ysrc, SRCAND);\
SelectObject(img, bitmap);\
BitBlt(hdc, xdest, ydest, xres, yres, image, xsrc, ysrc, SRCPAINT);




// ----Format Simplifiers--------------------------------------------
#define		_MSG_PARAMS	\
HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam

#define		_MSG_PROTO_PARAMS	\
HWND, UINT, WPARAM, LPARAM

#define		_MSG_CALL_PARAMS	\
hWnd, Msg, wParam, lParam

#define		_MSG_ARRAY_TYPE		\
(*)(_MSG_PROTO_PARAMS)

#define	_VOID_CALL			void (*)()

#define	_LPMSG_CALL			void (*)(LPMSG)



#endif

