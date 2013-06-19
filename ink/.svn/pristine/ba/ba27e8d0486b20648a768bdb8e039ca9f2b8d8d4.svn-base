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

// 32-bit RGB pixel
typedef struct
	{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char unused;
	}RGB;


// 32-bit RGB pixel for editing
typedef union
	{
	RGB		clr;
	unsigned long int value;
	}RGBcolor;


// Double DWORD
typedef	struct
	{
	DWORD lodword;
	DWORD hidword;
	}DWORD2;


// Double int
typedef	struct
	{
	int loint;
	int hiint;
	}int2;



// ----Type Abbreviations--------------------------------------------
#define		INT				int
#define		LPINT			int*
#define		LPUINT			unsigned int*
#define		UBYTE			unsigned char
#define		LPUBYTE			unsigned char*
#define		UWORD			unsigned short
#define		LPUWORD			unsigned short*
#define		UDWORD			unsigned int
#define		LPUDWORD		unsigned int*
#define		CHAR			char



// ----Macros--------------------------------------------------------

// Set a 32-bit RGB pixel
#define		inkSetRGBcolor(_RGBpixel, r, g, b)	\
_RGBpixel.clr.red=r;\
_RGBpixel.clr.green=g;\
_RGBpixel.clr.blue=b;\
_RGBpixel.clr.unused=0;


// Set a RECT
#define		inkSetRECT(Rectangle, x1, y1, x2, y2)	\
Rectangle.top=y1;\
Rectangle.bottom=y2;\
Rectangle.left=x1;\
Rectangle.right=x2;



// Draw on a Device Context from a BITMAP object opaquely
#define		inkBitBltOpaque(hdc, xdest, ydest, xres, yres, image, bitmap, xsrc, ysrc)		\
SelectObject(image, bitmap);\
BitBlt(hdc, xdest, ydest, xres, yres, image, xsrc, ysrc, SRCCOPY);


// Draw on a Device Context from a BITMAP object transparently
#define		inkBitBltTrans(hdc, xdest, ydest, xres, yres, image, bitmap, mask, xsrc, ysrc)	\
SelectObject(image, mask);\
BitBlt(hdc, xdest, ydest, xres, yres, img, xsrc, ysrc, SRCAND);\
SelectObject(img, bitmap);\
BitBlt(hdc, xdest, ydest, xres, yres, image, xsrc, ysrc, SRCPAINT);


// Insert data to a queue whose size must be a power of 2 ( max = size-1 )
// The macro may or may not be able to insert data to the queue
#define		inkInsertQueue(queue, num, rear, max, data)	\
if(num <= max)\
{\
	queue[rear] = data;\
	rear = (rear+1) & max;\
	num++;\
}


// Delete data from a queue whose size must be a power of 2 ( max = size-1 )
// The macro may or may not be able to delete data from queue
// data may contain garbage value if queue is empty and data is not initialized
#define		inkDeleteQueue(queue, num, front, max, data)	\
if(num)\
{\
data = queue[front];\
front = (front+1) & max;\
num--;\
}\




// ----Format Simplifiers--------------------------------------------
#define		_MSG_PARAMS	\
HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam


#define		_MSG_PROTO_PARAMS	\
HWND, UINT, WPARAM, LPARAM


#define		_MSG_CALL_PARAMS	\
hWnd, Msg, wParam, lParam

#define		_MSG_ARRAY_TYPE		\
(*)(_MSG_PROTO_PARAMS)

#endif

