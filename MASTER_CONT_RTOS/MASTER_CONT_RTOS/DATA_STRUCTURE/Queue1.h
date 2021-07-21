/*
 * queue.h
 *
 * Created: 05/02/2020 01:21:06 ص
 *  Author: ideapad510
 */ 


#ifndef QUEUE_H_
#define QUEUE_H_
#define max_Queue 8

/*QUEUES*/
/*U8*/
typedef struct{  unsigned char FRONT;
	             unsigned char REAR;
	             unsigned char COUNT;
	             unsigned char THE_QUEUE[max_Queue];
            	}QUEUE_UINT8;
void QUEUE_INTILIZE_U8(QUEUE_UINT8*MY_QUEUE);
void QUEUE_ADD_U8(QUEUE_UINT8*MY_QUEUE,unsigned char DATA);
unsigned char  QUEUE_GET_FRONT_U8(QUEUE_UINT8*MY_QUEUE);
unsigned char  QUEUE_GET_REAR_U8(QUEUE_UINT8*MY_QUEUE);
void  QUEUE_DELETE_FRONT_U8(QUEUE_UINT8*MY_QUEUE);
void  QUEUE_CLEAR_U8(QUEUE_UINT8*MY_QUEUE);
void  QUEUE_FORCED_ADD_U8(QUEUE_UINT8*MY_QUEUE,unsigned char DATA);

/*ARRAY U8*/
#define max_ARRAY_SIZE 40
typedef struct{  unsigned char FRONT;
	unsigned char REAR;
	unsigned char COUNT;
	unsigned char THE_QUEUE[max_Queue][max_ARRAY_SIZE];
}QUEUE_ARRAY_UINT8;
void QUEUE_INTILIZE_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE);
void QUEUE_ADD_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE,unsigned char* DATA,unsigned char data_length);
unsigned char *QUEUE_GET_FRONT_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE);
unsigned char *QUEUE_GET_REAR_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE);
void  QUEUE_DELETE_FRONT_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE);
void  QUEUE_CLEAR_ARRAY_U8(QUEUE_ARRAY_UINT8*MY_QUEUE);



#endif /* QUEUE_H_ */