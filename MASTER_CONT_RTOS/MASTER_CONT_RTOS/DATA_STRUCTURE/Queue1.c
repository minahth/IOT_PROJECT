/*
 * queue.c
 *
 * Created: 05/02/2020 01:20:52 ص
 *  Author: ideapad510
 */ 
#include "Queue1.h"
/*U8*/
void QUEUE_INTILIZE_U8(QUEUE_UINT8*MY_QUEUE)
{
	MY_QUEUE->COUNT=0;
	MY_QUEUE->FRONT=0;
	MY_QUEUE->REAR=max_Queue-1;
}

void QUEUE_ADD_U8(QUEUE_UINT8*MY_QUEUE,unsigned char DATA)
{
	if (MY_QUEUE->COUNT!=max_Queue)
	{
		(MY_QUEUE->COUNT)++;
		MY_QUEUE->REAR=(MY_QUEUE->REAR+1)%max_Queue;
		MY_QUEUE->THE_QUEUE[MY_QUEUE->REAR]=DATA;
	}
}

unsigned char QUEUE_GET_FRONT_U8(QUEUE_UINT8*MY_QUEUE)
{
	if (MY_QUEUE->COUNT!=0)
	{
		return MY_QUEUE->THE_QUEUE[MY_QUEUE->FRONT];
	}
	return 0;
}

unsigned char QUEUE_GET_REAR_U8(QUEUE_UINT8*MY_QUEUE)
{
	if (MY_QUEUE->COUNT!=0)
	{
		
		
		return MY_QUEUE->THE_QUEUE[MY_QUEUE->REAR];
	}
	return 0;
}

void QUEUE_DELETE_FRONT_U8(QUEUE_UINT8*MY_QUEUE)
{
	if (MY_QUEUE->COUNT!=0)
	{
		MY_QUEUE->FRONT=(MY_QUEUE->FRONT+1)%max_Queue;
		(MY_QUEUE->COUNT)--;
	}
}

void QUEUE_CLEAR_U8(QUEUE_UINT8*MY_QUEUE)
{
		MY_QUEUE->COUNT=0;
		MY_QUEUE->FRONT=0;
		MY_QUEUE->REAR=max_Queue-1;
}

void QUEUE_FORCED_ADD_U8(QUEUE_UINT8*MY_QUEUE,unsigned char DATA)
{if (MY_QUEUE->COUNT!=0)
	{
		MY_QUEUE->FRONT=(MY_QUEUE->FRONT+1)%max_Queue;
		(MY_QUEUE->COUNT)--;
	}
	else{
        QUEUE_DELETE_FRONT_U8(MY_QUEUE);
	QUEUE_ADD_U8(MY_QUEUE,DATA);
	}
}



















void QUEUE_INTILIZE_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE)
{
	MY_QUEUE->COUNT=0;
	MY_QUEUE->FRONT=0;
	MY_QUEUE->REAR=max_Queue-1;
}

void QUEUE_ADD_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE,unsigned char* DATA,unsigned char data_length)
{
	if (MY_QUEUE->COUNT!=max_Queue)
	{
		(MY_QUEUE->COUNT)++;
		MY_QUEUE->REAR=(MY_QUEUE->REAR+1)%max_Queue;
		unsigned char i;
		for(i=0;i<data_length;i++)
		{
			MY_QUEUE->THE_QUEUE[MY_QUEUE->REAR][i]=DATA[i];
		}
		
	
	}
}

unsigned char *QUEUE_GET_FRONT_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE)
{
	if (MY_QUEUE->COUNT!=0)
	{
		return MY_QUEUE->THE_QUEUE[MY_QUEUE->FRONT];
	}
	return 0;
}

unsigned char *QUEUE_GET_REAR_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE)
{
	if (MY_QUEUE->COUNT!=0)
	{
		
		
		return MY_QUEUE->THE_QUEUE[MY_QUEUE->REAR];
	}
	return 0;
}

void QUEUE_DELETE_FRONT_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE)
{
	if (MY_QUEUE->COUNT!=0)
	{
		MY_QUEUE->FRONT=(MY_QUEUE->FRONT+1)%max_Queue;
		(MY_QUEUE->COUNT)--;
	}
}

void QUEUE_CLEAR_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE)
{
	
	MY_QUEUE->COUNT=0;
	MY_QUEUE->FRONT=0;
	MY_QUEUE->REAR=max_Queue-1;
}
