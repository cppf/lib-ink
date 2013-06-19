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
#define		_INT			int
#define		INT				static _INT
#define		_LPINT			int*
#define		LPINT			static _LPINT
#define		_LPUINT			unsigned int*
#define		LPUINT			static _LPUINT
#define		_UBYTE			unsigned char
#define		UBYTE			static _UBYTE
#define		_LPUBYTE		unsigned char*
#define		LPUBYTE			static _LPUBYTE
#define		_UWORD			unsigned short
#define		UWORD			static _UWORD
#define		_LPUWORD		unsigned short*
#define		LPUWORD			static _LPUWORD
#define		_UDWORD			unsigned int
#define		UDWORD			static _UDWORD
#define		_LPUDWORD		unsigned int*
#define		LPUDWORD		static _LPUDWORD
#define		_CHAR			char
#define		CHAR			static _CHAR



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

