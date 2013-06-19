/****************************************************************************
 *                                                                          *
 * File    : error.cpp                                                      *
 *                                                                          *
 * Purpose : Error handling Routines for C++                                *
 *                                                                          *
 * History : Date       Part of INK Game Engine                              *
 *           5/12/2011  Subhajit Sahu                                       *
 *                                                                          *
 ****************************************************************************/


#ifndef	_error_cpp_

#define	_error_cpp_		1




// ----Includes------------------------------------------------------
#include "win32.h"


// ----Constants-----------------------------------------------------
#define		_eRELEASE_MODE			0
#define		_eNORMAL_MODE			0
#define		_eDEBUG_MODE			1
#define		_eADD_SIZE				32

// ----Globals-------------------------------------------------------
UINT				inkLastError;




// ----Externals-----------------------------------------------------





// ----Code----------------------------------------------------------

class inkError
{
// Data
public:
// Universal data
static void**		AddressList;
static char**		FunctionList;
static UINT			ListsNum;
static UINT			ListsSize;
static UINT*		ErrorTable;
static char**		ErrorString;
static UINT			ErrorsNum;
static UINT			ErrorsSize;
// store all this in a dump file
// normally use the dump file to
// retrive error details.


// Per thread data
UINT		Code;
char*		String;
void*		Address;
UINT		ThreadID;
char*		ThreadDescription;
void*		HandlingRoutine;

// Code
public:

// Starts the Error handling service
// Pass the pointer to your Function adding routine here
// It will be called during starup process, and then
// it should add all functions to the error service
// Note, that all this happens only if _eDEBUG_MODE is used
static void StartService(int mode, void* add_routine)
	{
	void (*adder)(void);
	SetErrorMode(SEM_NOOPENFILEERRORBOX);
	if(mode == _eDEBUG_MODE)
		{
		ListsNum = 0;
		ListsSize = _eADD_SIZE;
		AddressList = (void**)GlobalAlloc(GMEM_FIXED, ListsSize);
		FunctionList = (char**)GlobalAlloc(GMEM_FIXED, ListsSize);
		AddInkFunctions();
		adder = add_routine;
		*adder();
		}
	}



// All INK functions will be added here
static void AddInkFunctions(void)
	{
	return;
	}



// Add a function of the program. This information will be
// used to find out where error might have ocurred
// Listing partial number of functions can lead to incorrect
// prediction of where the error ocurred and while tracing furthur.
// Its parameters can also be looked at, if primitive.
// Ex- int add_num(int a, int b)
//     inkError::AddFunction(add_num, "add_numbers|ii");
// int - i | UINT - u | DWORD - u | char - c | short - s
// void* - w | int* -  j | char* - d | float - f | double - d
// float* - g | double* - e
static void AddFunction(void* address, char* function)
	{
	if(ListsNum == ListsSize)
		{
		ListsSize += _eADD_SIZE;
		AddressList = (void**)GlobalReAlloc(GMEM_FIXED, ListsSize);
		FunctionList = (char**)GlobalReAlloc(GMEM_FIXED, ListsSize);
		}
	AddressList[ListsNum] = address;
	FunctionList[ListsNum] = (char*)GlobalAlloc(GMEM_FIXED, lstrlen(function)+1);
	lstrcpy(FunctionList[ListsNum], function);
	ListsNum++;
	}



static void AddFunctionList(void** addresslist, char** functionlist, UINT listsize)
	{
	void** addr;
	ListsSize += listsize;
	AddressList = (void**)GlobalReAlloc(GMEM_FIXED, ListsSize);
	FunctionList = (char**)GlobalReAlloc(GMEM_FIXED, ListsSize);
	addr = addresslist + listsize;
	
	}
// Constructor of error object for each thread
inkError(
// :to add functions address to find out where the error ocurred
// :constructor for each thread to create its own error object
// :destructor
// :set , get , stringadd , modify , find out function , 
// :get windows error automatically function
// :pass control to handling routine

// lookup level - no. of bytes to lookup in stack

standard format for error handling

inkError:AddFunction();



inkError(
};


inkError::inkError(



#endif

