/****************************************************************************
 *                                                                          *
 * File    : string.cpp                                                     *
 *                                                                          *
 * Purpose : Handling of some string functions for C++                      *
 *                                                                          *
 * History : Date      		Part of INK Game Engine                         *
 *           14/07/2011		Subhajit Sahu                                   *
 *                                                                          *
 ****************************************************************************/

#ifndef		_string_cpp_

#define		_string_cpp_		1



// ----Includes------------------------------------------------------
#include "win32.h"




// ----Code----------------------------------------------------------




/*
Class:	inkString

A set of string management functions
*/


namespace ink
{
	#ifdef	UNICODE
	#define _TS(x)		(x<<1)
	#else
	#define _TS(x)		(x)
	#endif


	class String
	{

	private:
		// String descriptors
		TCHAR*			Buffer;
		UINT			Len;
		UINT			Size;

		// Amount of Buffering
		static UINT		Buffering;

	public:
		String(TCHAR* s);
		String(void);
		String(String* str);
		~String(void);
		static void SetBuffering(UINT buffering);
		UINT Length(void);
		void operator+= (String* str2);
		String* operator+ (String* str2);
		void operator= (String* str2);
		TCHAR* GetStr(void);
		char* GetCharStr(void);
		WCHAR* GetWcharStr(void);
		void SetCharStr(char* src);
		void SetWcharStr(WCHAR* src);
	};
	UINT	String::Buffering = 16;


	String::String(TCHAR* str)
	{
		if(str != NULL)
		{
			Len = lstrlen(str);
			Size = Len + 1 + Buffering;
			Buffer = (TCHAR*)GlobalAlloc(GMEM_FIXED, _TS(Size));
			lstrcpy(Buffer, str);
		}
		else
		{
			Len = 0;
			Size = 1 + Buffering;
			Buffer = (TCHAR*)GlobalAlloc(GMEM_FIXED, _TS(Size));
			*Buffer = _T('\0');
		}
	}


	String::String(void)
	{
		Len = 0;
		Size = 1 + Buffering;
		Buffer = (TCHAR*)GlobalAlloc(GMEM_FIXED, _TS(Size));
		*Buffer = _T('\0');
	}


	String::String(String* str)
	{
		Len = str->Len;
		Size = Len + 1 + Buffering;
		Buffer = (TCHAR*)GlobalAlloc(GMEM_FIXED, _TS(Size));
		lstrcpy(Buffer, str->Buffer);
	}


	String::~String(void)
	{
		GlobalFree(Buffer);
	}


	void String::SetBuffering(UINT buffering)
	{
		Buffering = buffering;
	}


	UINT String::Length(void)
	{
		return(Len);
	}


	void String::operator+= (String* str2)
	{
		UINT lent;
		lent = Len + str2->Len;
		if(lent < Size)
		{
			lstrcpy(Buffer+Len, str2->Buffer);
			Len = lent;
			return;
		}
		lent = Len + str2->Len;
		Size = lent + 1 + Buffering;
		Buffer = (TCHAR*)GlobalReAlloc(Buffer, _TS(Size), 0);
		lstrcpy(Buffer+Len, str2->Buffer);
		Len = lent;
	}


	String* String::operator+ (String* str2)
	{
		UINT lent;
		String *res = new String();
		lent = Len + str2->Len;
		res->Size = lent + 1 + Buffering;
		res->Buffer = (TCHAR*)GlobalReAlloc(res->Buffer, _TS(Size), 0);
		lstrcpy(res->Buffer, Buffer);
		lstrcpy(res->Buffer + Len, str2->Buffer);
		res->Len = lent;
		return(res);
	}


	void String::operator= (String* str2)
	{
		if(str2->Len < Size)
		{
			lstrcpy(Buffer, str2->Buffer);
			Len = str2->Len;
			return;
		}
		Len = str2->Len;
		Size = Len + 1 + Buffering;
		Buffer = (TCHAR*)GlobalReAlloc(Buffer, _TS(Size), 0);
		lstrcpy(Buffer, str2->Buffer);
	}


	TCHAR* String::GetStr(void)
	{
		return(Buffer);
	}


	char* String::GetCharStr(void)
	{
		TCHAR* src;
		char *dest, *dest2;
		src = Buffer;
		dest = (char*)GlobalAlloc(GMEM_FIXED, Len+1);
		dest2 = dest;
		#ifdef UNICODE
			while(1)
			{
				*dest = (char)*src;
				if(!(*src))break;
				src++;
				dest++;
			}
		#else
			lstrcpy(dest, src);
		#endif
		return(dest2);
	}


	WCHAR* String::GetWcharStr(void)
	{
		TCHAR* src;
		WCHAR *dest, *dest2;
		src = Buffer;
		dest = (WCHAR*)GlobalAlloc(GMEM_FIXED, (Len+1)<<1);
		dest2 = dest;
		#ifdef UNICODE
			while(1)
			{
				*dest = (WCHAR)*src;
				if(!(*src))break;
				src++;
				dest++;
			}
		#else
			lstrcpy(dest, src);
		#endif
		return(dest2);
	}


	void String::SetCharStr(char* src)
	{
		TCHAR* dest;
		UINT lent;
		lent = lstrlenA(src);
		if(lent < Size)
		{
			dest = Buffer;
			#ifdef UNICODE
			while(1)
			{
				*dest = (TCHAR)*src;
				if(!(*src))break;
				src++;
				dest++;
			}
			#else
				lstrcpy(dest, src);
			#endif
			Len = lent;
			return;
		}
		Len = lent;
		Size = Len + 1 + Buffering;
		Buffer = (TCHAR*)GlobalReAlloc(Buffer, _TS(Size), 0);
		dest = Buffer;
		#ifdef	UNICODE
		while(1)
		{
			*dest = (TCHAR)*src;
			if(!(*src))break;
			src++;
			dest++;
		}
		#else
			lstrcpy(dest, src);
		#endif
	}


	void String::SetWcharStr(WCHAR* src)
	{
		TCHAR* dest;
		UINT lent;
		lent = lstrlenW(src);
		if(lent < Size)
		{
			dest = Buffer;
			#ifdef UNICODE
			while(1)
			{
				*dest = (TCHAR)*src;
				if(!(*src))break;
				src++;
				dest++;
			}
			#else
				lstrcpy(dest, src);
			#endif
			Len = lent;
			return;
		}
		Len = lent;
		Size = Len + 1 + Buffering;
		Buffer = (TCHAR*)GlobalReAlloc(Buffer, _TS(Size), 0);
		dest = Buffer;
		#ifdef	UNICODE
		while(1)
		{
			*dest = (TCHAR)*src;
			if(!(*src))break;
			src++;
			dest++;
		}
		#else
			lstrcpy(dest, src);
		#endif
	}
	// use for lower case to uppercase
	// use for parameter filtering (%d,%d) for (23,56) gives 23 56
}






#endif


