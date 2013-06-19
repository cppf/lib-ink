// theBigConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "memoryGuy.h"


typedef struct _INTEGER_DATA_QUEUE
{
	INK_DATA_QUEUE(int)
}INTEGER_DATA_QUEUE;




INTEGER_DATA_QUEUE	queue;



void PrintQueue(INTEGER_DATA_QUEUE* queue);






int _tmain(int argc, _TCHAR* argv[])
{
	char c;int x;
	queue.Size = 8;
	queue.Data = (int*) inkMemAlloc(queue.Size);
	INK_INSERT_QUEUE(queue, 1)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 2)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 3)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 4)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 5)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 6)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 7)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 8)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 9)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 10)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 11)
	PrintQueue(&queue);
	INK_INSERT_QUEUE(queue, 12)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	INK_DELETE_QUEUE(queue, x)
	PrintQueue(&queue);
	scanf("%c", &c);
	return(0);
}


void PrintQueue(INTEGER_DATA_QUEUE* queue)
{
	int i, j;
	printf("Queue Number = %d\n", queue->Number);
	for(i=queue->Front,j=queue->Number; j>0; i=(i+1)&(queue->Size - 1), j--)
	{
		printf("Queue[%d] = %d\n", i, queue->Data[i]);
	}
}









