/****************************************************************************
 *                                                                          *
 * File    : memoryGuy.h                                                    *
 *                                                                          *
 * Purpose : The functions for inkEngine related to memory handling.        *
 *                                                                          *
 * History : Date      Part of INK Game Engine                              *
 *           03/3/2012 Subhajit Sahu                                        *
 *                                                                          *
 ****************************************************************************/


#ifndef	_memoryGuy_h_
#define	_memoryGuy_h_		1



// Double DWORD
typedef	struct _DWORD2
	{
	DWORD	low;
	DWORD	high;
	}DWORD2;

// Double int
typedef	struct _INT2
	{
	int		low;
	int		high;
	}INT2;




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




// Data Queue [Circular and expandable] (for any datatype)
#define	INK_DATA_QUEUE(Datatype)	\
	UINT		Size;\
	UINT		Number;\
	UINT		Front;\
	UINT		Rear;\
	UINT		BufferingLength;\
	Datatype	Data;

// Data List [expandable] (for any datatype)
#define	INK_DATA_LIST(Datatype)	\
	UINT		Size;\
	UINT		Number;\
	UINT		BufferingLength;\
	Datatype	Data;

// Data Stack [expandable] (for any data type)
#define	INK_DATA_STACK(Datatype)	\
	UINT		Buffer;\
	UINT		Size;\
	UINT		Top;\
	UINT		BufferingLength;\
	Datatype	Data;





// ----Memory Related Macros-----------------------------------------


// _TS(size)
// Give the size in bytes required for text of given size
#define	_AS(size)		size
#define	_WS(size)		((size)<<1)
#ifdef	UNICODE
	#define	_TS				_WS
#else
	#define	_TS				_AS
#endif
#define	TEXT_SIZE			_TS


// Allocate Memory
#define inkMemAlloc(size)	\
	GlobalAlloc(GMEM_FIXED, size)

// Allocate Memory for String of given size
#define	inkStrAlloc(size)	\
	GlobalAlloc(GMEM_FIXED, _TS(size))

// Allocate Memory for a Structure(not function)
#define inkStructAlloc(strc)	\
	GlobalAlloc(GMEM_FIXED, sizeof(*(strc)))

// Allocate & Initialize Memory
#define	inkMemInitAlloc(size)	\
	GlobalAlloc(GPTR, size)

// Allocate & Initialize Memory for string
#define inkStrInitAlloc(size)	\
	GlobalAlloc(GPTR, _TS(size))

// Allocate & Initialize Memory for a Structure(not function)
#define	inkStructInitAlloc(strc)	\
	GlobalAlloc(GPTR, sizeof(*(strc)))

// ReAllocate Memory
#define inkMemReAlloc(mem, size)	\
	GlobalReAlloc(mem, size, 0)

// ReAllocate Memory for String of given size
#define	inkStrReAlloc(str, size)	\
	GlobalReAlloc(str, _TS(size), 0)

// ReAllocate & Initialize Memory
#define	inkMemInitReAlloc(mem, size)	\
	GlobalReAlloc(mem, size, GMEM_ZEROINIT)

// ReAllocate & Initialize Memory for string
#define inkStrInitReAlloc(str, size)	\
	GlobalReAlloc(str, _TS(size), GMEM_ZEROINIT)

// Free Memory(not function)
#define	inkMemFree(ptr)	\
	GlobalFree((HGLOBAL)ptr)

// Free Memory allocated to a string(not function)
#define	inkStrFree(ptr)	\
	GlobalFree((HGLOBAL)ptr)

// Free Memory allocated to a structure(not function)
#define	inkStructFree(strc)	\
	GlobalFree((HGLOBAL)strc)

// Initialize Memory
#define	inkMemInit(ptr, size)	\
	ZeroMemory(ptr, size)

// Initialize String
#define	inkStrInit(str, size)	\
	ZeroMemory(str, _TS(size))

// Initialize Structure
#define	inkStructInit(strc)	\
	ZeroMemory(strc, sizeof(*(strc)))






// Insert data to a queue whose size must be a power of 2 ( max = size-1 )
// The macro may or may not be able to insert data to the queue
#define		INK_INSERT_QUEUE(queue, data)	\
	if(queue##.##Number < queue##.##Size)\
	{\
		queue##.##Data[queue##.##Rear] = data;\
		queue##.##Rear = (queue##.##Rear+1) & (queue##.##Size-1);\
		queue##.##Number++;\
	}


// Delete data from a queue whose size must be a power of 2 ( max = size-1 )
// The macro may or may not be able to delete data from queue
// data may contain garbage value if queue is empty and data is not initialized
#define		INK_DELETE_QUEUE(queue, data)	\
	if(queue##.##Number)\
	{\
		data = queue##.##Data[queue##.##Front];\
		queue##.##Front = (queue##.##Front+1) & (queue##.##Size-1);\
		queue##.##Number--;\
	}\



// Insert data to a queue whose size must be a power of 2 ( max = size-1 )
// The macro may or may not be able to insert data to the queue
#define		INK_INSERT_QUEUE_BUFFERED(queue, data, Datatype, uint_scratch_var1 , uint_scratch_var2)	\
	{\
		if((queue##.##Number >= queue##.##Size) && (uint_scratch_var1 = (UINT)inkMemReAlloc(queue##.##Data, queue##.##Size<<1))\
		{\
			queue.Data = (Datatype*)uint_scratch_var1;\
			queue##.##Size <<= 1;\
			if(queue##.##Rear != ((queue##.##Size>>1) - 1))\
			{\
				for(uint_scratch_var1=(queue##.##Size>>1)-1,uint_scratch_var2=queue##.##Size-1; uint_scratch_var1>=queue##.##Front; uint_scratch_var1--,uint_scratch_var2--)\
					queue##.##Data[uint_scratch_var2] = queue##.##Data[uint_scratch_var1];\
				queue##.##Front = uint_scratch_var2+1;\
			}\
		}\
		INK_INSERT_QUEUE(queue, data)\
	}
// ^-- else: falied to allocate memory for queue





// Delete data from a queue whose size must be a power of 2 ( max = size-1 )
// The macro may or may not be able to delete data from queue
// data may contain garbage value if queue is empty and data is not initialized
#define		INK_DELETE_QUEUE_BUFFERED(queue, data)	\
	if(queue.Number <= (queue.Size>>1))\
	{\
		queue.Size >>= 1;\
		if(queue.Front != (queue.Size-1))\
		{\
			\
		}\
	}\
	if(queue##.##Number)\
	{\
		data = queue##.##Data[queue##.##Front];\
		queue##.##Front = (queue##.##Front+1) & queue##.##Size;\
		queue##.##Number--;\
	}\
	if(queue




#define		INK_INSERT_LIST(list, data)	\
	if(list##.##Number < list##.##Size)\
	{\
	list##.##Data[list##.##Number] = data;\
	list##.##Number++;\
	}

#define		INK_DELETE_LIST(list, index, uint_scratch_var)	\
	if(index < list##.##Number)\
	{\
		for(uint_scratch_var=index+1; uint_scratch_var<list##.##Number; uint_scratch_var++)\
			list##.##Data[uint_scratch_var-1] = list##.##Data[uint_scratch_var];\
		list##.##Number--;\
	}



#endif


