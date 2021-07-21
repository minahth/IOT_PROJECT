/*
 * protocol.c
 *
 * Created: 02-Apr-21 2:28:54 PM
 *  Author: Mina Medhat
 */ 
#include "protocol.h"
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "semphr.h"
#include "EXINTEREPUT128A.h"

unsigned char MY_ADDRESS[5]={MY_PROTOCOL_ADDRESS_ARRAY_0,MY_PROTOCOL_ADDRESS_ARRAY_1,MY_PROTOCOL_ADDRESS_ARRAY_2,MY_PROTOCOL_ADDRESS_ARRAY_3,MY_PROTOCOL_ADDRESS_ARRAY_4};

SemaphoreHandle_t NRF_IRQ_SEMAPHORE;


void INT0_NRF_ISR()
{
	xSemaphoreGiveFromISR(NRF_IRQ_SEMAPHORE,NULL);
}

void PROTOCOL_SETUP()
{    NRF_IRQ_SEMAPHORE=xSemaphoreCreateBinary();
	
	 external_int_enable(INT0_ENABLE,INT_FALLING);
	 NRF_SETUP_USED_CONFIG();
}

void PROTOCOL_IO_SETUP()
{
	 NRF_IO_SETUP();
}

unsigned char PROTOCOL_MASTER_CONNECT_TO_SENSOR(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*PASSWORD_KEY)
{
	unsigned char PACKET_TEMP[12];
	unsigned char i,RX_LENGTH;
	unsigned char HASH_VALUE[2];
	volatile unsigned long COUNT_TIMER=0;
	
	
	
	/*********SEND MASTER NEEDS TO CONNECT**********/
	PACKET_TEMP[0]=10|0B10000000;
	PACKET_TEMP[1]=SENSOR_VERSION;
	for (i=0;i<5;i++)
	{
		PACKET_TEMP[2+i]=MY_ADDRESS[i];
	}
	PACKET_TEMP[7]=MASTER_REQUEST_CONNECT_CODE;
	HASH_PASWORD_KEY(PACKET_TEMP,HASH_VALUE,8,PASSWORD_KEY);
	PACKET_TEMP[8]=HASH_VALUE[0];
	PACKET_TEMP[9]=HASH_VALUE[1];
	if (!NRF_SEND_DATA_WITH_ACK(SENSOR_ADDRESS,SENSOR_ADDRESS_LENGTH,PACKET_TEMP,10))
	{
		TEST_UART_SEND_str("FIRST FAIL\n");
		return 0;
	}
	/*****************************************************************************************/
	
	
	/******************************RECIVE SENSOR CHALLANGE*******************************************/
	NRF_START_RX_MODE();
	TEST_UART_SEND_str("FIRST PASS\n");
	
	if(pdPASS!=xSemaphoreTake(NRF_IRQ_SEMAPHORE,pdMS_TO_TICKS(3000)))
	{
		TEST_UART_SEND_str("FAIL 2\n");
		NRF_STOP_RX_MODE();
		return 0;
	}
	
	NRF_CHECK_RX_PL(PACKET_TEMP,&RX_LENGTH);
	
	
	NRF_STOP_RX_MODE();
	
	if (!PROTOCOL_CHECK_RECIVED_PACKET(PACKET_TEMP, SENSOR_ADDRESS,RX_LENGTH,PASSWORD_KEY))
	{
		return 0;
	}
	if(PACKET_TEMP[7]!=SENSOR_CHALLENGE_CODE)
	{
		return 0;
	}
	
	/**************************************************************************************************/
	
	/****************************************SEND THE ENCRYPTED RANDOM NUMBER***********************************************************/
	unsigned char R_N[2],ENC_R_N[2];
	R_N[0]=PACKET_TEMP[8];
	R_N[1]=PACKET_TEMP[9];
	ENCRYPT_ALGORISM_2_V1(R_N,ENC_R_N,2,PASSWORD_KEY);
	
	PACKET_TEMP[0]=12|0B10000000;
	PACKET_TEMP[1]=SENSOR_VERSION;
	for (i=0;i<SENSOR_ADDRESS_LENGTH;i++)
	{
		PACKET_TEMP[2+i]=MY_ADDRESS[i];
	}
	PACKET_TEMP[7]=CHALLENGE_ANSWER_CODE;
	PACKET_TEMP[8]=ENC_R_N[0];
	PACKET_TEMP[9]=ENC_R_N[1];
	HASH_PASWORD_KEY(PACKET_TEMP,HASH_VALUE,10,PASSWORD_KEY);
	PACKET_TEMP[10]=HASH_VALUE[0];
	PACKET_TEMP[11]=HASH_VALUE[1];
	if (!NRF_SEND_DATA_WITH_ACK(SENSOR_ADDRESS,SENSOR_ADDRESS_LENGTH,PACKET_TEMP,12))
	{
		return 0;
	}
	/******************************************************************************************************************************/
	
	
	/****************************************CHECK THE RESULT OF CHALLANGE***********************************************************/
	COUNT_TIMER=0;
	NRF_START_RX_MODE();
	
	if(pdPASS!=xSemaphoreTake(NRF_IRQ_SEMAPHORE,pdMS_TO_TICKS(3000)))
	{
		TEST_UART_SEND_str("FAIL 3\n");
		NRF_STOP_RX_MODE();
		return 0;
	}
	
	NRF_CHECK_RX_PL(PACKET_TEMP,&RX_LENGTH);
	
	TEST_UART_SEND_str("counter=");
	TEST_UART_SEND_VALUE(COUNT_TIMER);
	TEST_UART_SEND_str("\n");
	NRF_STOP_RX_MODE();
	if (!PROTOCOL_CHECK_RECIVED_PACKET(PACKET_TEMP, SENSOR_ADDRESS,RX_LENGTH,PASSWORD_KEY))
	{
		return 0;
	}	
	if(PACKET_TEMP[7]!=CHALLENGE_RESULT_CODE)
	{
		return 0;
	}
	unsigned char TEMP=0;
	DECRYPT_ALGORISM_2_V1(&TEMP,&PACKET_TEMP[8],1,PASSWORD_KEY);
	
	if(TEMP!=CHALLANGE_RESULT_SUCCESS)
	{
		return 0;
	}
	TEST_UART_SEND_str("SENSOR CONNECTED\n");
	return 1;
}
















unsigned char PROTOCOL_CHECK_RECIVED_PACKET(unsigned char *PACKET, unsigned char *SENSOR_ADDRESS,unsigned char LENGTH,unsigned char*KEY)
{
	if (LENGTH!=(0b01111111 & PACKET[0]))
	{
		return 0;
	}
	if (PACKET[1]>PROTOCOL_CURRENT_VERSION)
	{
		return 0;
	}
	unsigned char i;
	for(i=0;i<5;i++)
	{
		if (PACKET[2+i]!=SENSOR_ADDRESS[i])
		{
			return 0;
		}
	}
	
	
	
	unsigned char HASH_VALUE[2];
	if (0b10000000&PACKET[0])
	{
		HASH_PASWORD_KEY(PACKET,HASH_VALUE,LENGTH-2,KEY);
	}
	else
	{
		HASH_SESSION_KEY(PACKET,HASH_VALUE,LENGTH-2,KEY);
	}
	if ((HASH_VALUE[0]==PACKET[LENGTH-2])&&(HASH_VALUE[1]==PACKET[LENGTH-1]))
	{
		return 1;
	}
	return 0;
	
}

unsigned char PROTOCOL_SEND_BEACON(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY)
{
	unsigned char PACKET_TEMP[10];
	unsigned char i,RX_LENGTH;
	unsigned char HASH_VALUE[2];
	volatile unsigned long COUNT_TIMER=0;
	
	
	
	/*********SEND BEACON**********/
	PACKET_TEMP[0]=10|0B00000000;
	PACKET_TEMP[1]=SENSOR_VERSION;
	for (i=0;i<SENSOR_ADDRESS_LENGTH;i++)
	{
		PACKET_TEMP[2+i]=MY_ADDRESS[i];
	}
	PACKET_TEMP[7]=BEACON_CODE;
	HASH_SESSION_KEY(PACKET_TEMP,HASH_VALUE,8,SESSION_KEY);
	PACKET_TEMP[8]=HASH_VALUE[0];
	PACKET_TEMP[9]=HASH_VALUE[1];
	if (!NRF_SEND_DATA_WITH_ACK(SENSOR_ADDRESS,SENSOR_ADDRESS_LENGTH,PACKET_TEMP,10))
	{
		return 0;
	}
	/*****************************************************************************************/
	
	
	/******************************RECIVE BEACON *******************************************/
	NRF_START_RX_MODE();
	
		if(pdPASS!=xSemaphoreTake(NRF_IRQ_SEMAPHORE,pdMS_TO_TICKS(3000)))
		{
			TEST_UART_SEND_str("BEACON FAIL\n");
			NRF_STOP_RX_MODE();
			return 0;
		}
		
		NRF_CHECK_RX_PL(PACKET_TEMP,&RX_LENGTH);
	
	
	
	
	
	TEST_UART_SEND_str("counter=");
	TEST_UART_SEND_VALUE(COUNT_TIMER);
	TEST_UART_SEND_str("\n");
	NRF_STOP_RX_MODE();
	if (!PROTOCOL_CHECK_RECIVED_PACKET(PACKET_TEMP, SENSOR_ADDRESS,RX_LENGTH,SESSION_KEY))
	{
		return 0;
	}
	if(PACKET_TEMP[7]!=BEACON_RECEIVED_CODE)
	{
		return 0;
	}
	
	return 1;
}

unsigned char PROTOCOL_SEND_SESSION_KEY(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY,unsigned char*PASSWORD_KEY)
{
	unsigned char PACKET_TEMP[12];
	unsigned char i,RX_LENGTH;
	unsigned char HASH_VALUE[2];
	volatile unsigned long COUNT_TIMER=0;
	
	
	
	/*********NEW SESSION**********/
	PACKET_TEMP[0]=12|0B10000000;
	PACKET_TEMP[1]=SENSOR_VERSION;
	for (i=0;i<SENSOR_ADDRESS_LENGTH;i++)
	{
		PACKET_TEMP[2+i]=MY_ADDRESS[i];
	}
	PACKET_TEMP[7]=NEW_SESSION_KEY_CODE;
	unsigned char ENC_SESSION[2];
	ENCRYPT_ALGORISM_2_V1(SESSION_KEY,ENC_SESSION,2,PASSWORD_KEY);
	PACKET_TEMP[8]=ENC_SESSION[0];
	PACKET_TEMP[9]=ENC_SESSION[1];
	HASH_PASWORD_KEY(PACKET_TEMP,HASH_VALUE,10,PASSWORD_KEY);
	PACKET_TEMP[10]=HASH_VALUE[0];
	PACKET_TEMP[11]=HASH_VALUE[1];
	if (!NRF_SEND_DATA_WITH_ACK(SENSOR_ADDRESS,SENSOR_ADDRESS_LENGTH,PACKET_TEMP,12))
	{
		return 0;
	}
	/*****************************************************************************************/
	
	
	/******************************SESSION KEY APPLIED*******************************************/
	NRF_START_RX_MODE();
	
	
	
	if(pdPASS!=xSemaphoreTake(NRF_IRQ_SEMAPHORE,pdMS_TO_TICKS(3000)))
	{
		TEST_UART_SEND_str("SESSION FAIL\n");
		NRF_STOP_RX_MODE();
		return 0;
	}
	
	NRF_CHECK_RX_PL(PACKET_TEMP,&RX_LENGTH);
	
	
	
	TEST_UART_SEND_str("counter=");
	TEST_UART_SEND_VALUE(COUNT_TIMER);
	TEST_UART_SEND_str("\n");
	NRF_STOP_RX_MODE();
	if (!PROTOCOL_CHECK_RECIVED_PACKET(PACKET_TEMP, SENSOR_ADDRESS,RX_LENGTH,PASSWORD_KEY))
	{
		return 0;
	}
	if(PACKET_TEMP[7]!=NEW_SESSION_KEY_APPLIED_CODE)
	{
		return 0;
	}
	unsigned char TEMP=0;
	DECRYPT_ALGORISM_2_V1(&TEMP,&PACKET_TEMP[8],1,PASSWORD_KEY);
	
	if(TEMP!=CHALLANGE_RESULT_SUCCESS)
	{
		return 0;
	}
	return 1;
	
	
}

unsigned char PROTOCOL_CHANGE_CHANNEL(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY,unsigned char CHANNEL)
{
	volatile unsigned char PACKET_TEMP[12];
	unsigned char i,RX_LENGTH;
	unsigned char HASH_VALUE[2];
	volatile unsigned long COUNT_TIMER=0;
	
	
	
	/*********NEW CHANNEL**********/
	PACKET_TEMP[0]=11|0B00000000;
	PACKET_TEMP[1]=SENSOR_VERSION;
	for (i=0;i<SENSOR_ADDRESS_LENGTH;i++)
	{
		PACKET_TEMP[2+i]=MY_ADDRESS[i];
	}
	PACKET_TEMP[7]=MASTER_CHANGE_CHANNEL_CODE;
	unsigned char ENC_CHANNEL;
	ENCRYPT_ALGORISM_1_V1(&CHANNEL,&ENC_CHANNEL,1,SESSION_KEY);
	PACKET_TEMP[8]=ENC_CHANNEL;
	HASH_SESSION_KEY(PACKET_TEMP,HASH_VALUE,9,SESSION_KEY);
	PACKET_TEMP[9]=HASH_VALUE[0];
	PACKET_TEMP[10]=HASH_VALUE[1];
	if (!NRF_SEND_DATA_WITH_ACK(SENSOR_ADDRESS,SENSOR_ADDRESS_LENGTH,PACKET_TEMP,11))
	{
		return 0;
	}
	/*****************************************************************************************/
	
	
	/******************************CHANNEL APPLIED*******************************************/
	NRF_START_RX_MODE();
	
	
	if(pdPASS!=xSemaphoreTake(NRF_IRQ_SEMAPHORE,pdMS_TO_TICKS(3000)))
	{
		TEST_UART_SEND_str("CHANNEL FAIL\n");
		NRF_STOP_RX_MODE();
		return 0;
	}
	
	NRF_CHECK_RX_PL(PACKET_TEMP,&RX_LENGTH);
	
	
	TEST_UART_SEND_str("counter=");
	TEST_UART_SEND_VALUE(COUNT_TIMER);
	TEST_UART_SEND_str("\n");
	NRF_STOP_RX_MODE();
	if (!PROTOCOL_CHECK_RECIVED_PACKET(PACKET_TEMP, SENSOR_ADDRESS,RX_LENGTH,SESSION_KEY))
	{
		return 0;
	}
	if(PACKET_TEMP[7]!=CHANNEL_CHANGED_CODE)
	{
		return 0;
	}
	unsigned char TEMP=0;
	DECRYPT_ALGORISM_1_V1(&TEMP,&PACKET_TEMP[8],1,SESSION_KEY);
	
	if(TEMP!=CHALLANGE_RESULT_SUCCESS)
	{
		return 0;
	}
	return 1;
}

unsigned char PROTOCOL_DATA_SEND(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY,unsigned char *REGISTER_ADDRESS,unsigned char*DATA_SEND_BUFFER)
{
	unsigned char PACKET_TEMP[14];
	unsigned char i,RX_LENGTH;
	unsigned char HASH_VALUE[2];
	volatile unsigned long COUNT_TIMER=0;
	
	
	
	/*********SEND DATA**********/
	PACKET_TEMP[0]=14|0B00000000;
	PACKET_TEMP[1]=SENSOR_VERSION;
	for (i=0;i<SENSOR_ADDRESS_LENGTH;i++)
	{
		PACKET_TEMP[2+i]=MY_ADDRESS[i];
	}
	PACKET_TEMP[7]=MASTER_CHANGE_DATA_CODE;
	unsigned char PLAIN_TEXT[4],CIPHER_TEXT[4];
	PLAIN_TEXT[0]=REGISTER_ADDRESS[0];
	PLAIN_TEXT[1]=REGISTER_ADDRESS[1];
	PLAIN_TEXT[2]=DATA_SEND_BUFFER[0];
	PLAIN_TEXT[3]=DATA_SEND_BUFFER[1];
	ENCRYPT_ALGORISM_1_V1(PLAIN_TEXT,CIPHER_TEXT,4,SESSION_KEY);
	PACKET_TEMP[8]=CIPHER_TEXT[0];
	PACKET_TEMP[9]=CIPHER_TEXT[1];
	PACKET_TEMP[10]=CIPHER_TEXT[2];
	PACKET_TEMP[11]=CIPHER_TEXT[3];
	HASH_SESSION_KEY(PACKET_TEMP,HASH_VALUE,12,SESSION_KEY);
	PACKET_TEMP[12]=HASH_VALUE[0];
	PACKET_TEMP[13]=HASH_VALUE[1];
	if (!NRF_SEND_DATA_WITH_ACK(SENSOR_ADDRESS,SENSOR_ADDRESS_LENGTH,PACKET_TEMP,14))
	{
		return 0;
	}
	/*****************************************************************************************/
	
	
	/******************************DATA SEND APPLIED*******************************************/
	NRF_START_RX_MODE();
	
	if(pdPASS!=xSemaphoreTake(NRF_IRQ_SEMAPHORE,pdMS_TO_TICKS(3000)))
	{
		TEST_UART_SEND_str("SEND FAIL\n");
		NRF_STOP_RX_MODE();
		return 0;
	}
	
	NRF_CHECK_RX_PL(PACKET_TEMP,&RX_LENGTH);
	
	
	
	TEST_UART_SEND_str("counter=");
	TEST_UART_SEND_VALUE(COUNT_TIMER);
	TEST_UART_SEND_str("\n");
	NRF_STOP_RX_MODE();
	if (!PROTOCOL_CHECK_RECIVED_PACKET(PACKET_TEMP, SENSOR_ADDRESS,RX_LENGTH,SESSION_KEY))
	{
		return 0;
	}
	if(PACKET_TEMP[7]!=DATA_CHANGED_CODE)
	{
		return 0;
	}
	return 1;
}

unsigned char PROTOCOL_DATA_READ(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY,unsigned char *REGISTER_ADDRESS,unsigned char*DATA_READ_BUFFER)
{
	unsigned char PACKET_TEMP[14];
	unsigned char i,RX_LENGTH;
	unsigned char HASH_VALUE[2];
	volatile unsigned long COUNT_TIMER=0;
	
	
	
	/*********READ DATA**********/
	PACKET_TEMP[0]=12|0B00000000;
	PACKET_TEMP[1]=SENSOR_VERSION;
	for (i=0;i<SENSOR_ADDRESS_LENGTH;i++)
	{
		PACKET_TEMP[2+i]=MY_ADDRESS[i];
	}
	PACKET_TEMP[7]=MASTER_NEEDS_DATA_CODE;
	unsigned char PLAIN_TEXT[2],CIPHER_TEXT[2];
	PLAIN_TEXT[0]=REGISTER_ADDRESS[0];
	PLAIN_TEXT[1]=REGISTER_ADDRESS[1];
	ENCRYPT_ALGORISM_1_V1(PLAIN_TEXT,CIPHER_TEXT,2,SESSION_KEY);
	PACKET_TEMP[8]=CIPHER_TEXT[0];
	PACKET_TEMP[9]=CIPHER_TEXT[1];
	HASH_SESSION_KEY(PACKET_TEMP,HASH_VALUE,10,SESSION_KEY);
	PACKET_TEMP[10]=HASH_VALUE[0];
	PACKET_TEMP[11]=HASH_VALUE[1];
	if (!NRF_SEND_DATA_WITH_ACK(SENSOR_ADDRESS,SENSOR_ADDRESS_LENGTH,PACKET_TEMP,12))
	{
		return 0;
	}
	/*****************************************************************************************/
	
	
	/******************************DATA READ*******************************************/
	NRF_START_RX_MODE();
	if(pdPASS!=xSemaphoreTake(NRF_IRQ_SEMAPHORE,pdMS_TO_TICKS(3000)))
	{
		TEST_UART_SEND_str("BEACON FAIL\n");
		NRF_STOP_RX_MODE();
		return 0;
	}
	
	NRF_CHECK_RX_PL(PACKET_TEMP,&RX_LENGTH);
	
	TEST_UART_SEND_str("counter=");
	TEST_UART_SEND_VALUE(COUNT_TIMER);
	TEST_UART_SEND_str("\n");
	NRF_STOP_RX_MODE();
	if (!PROTOCOL_CHECK_RECIVED_PACKET(PACKET_TEMP, SENSOR_ADDRESS,RX_LENGTH,SESSION_KEY))
	{
		return 0;
	}
	if(PACKET_TEMP[7]!=DATA_READ_CODE)
	{
		return 0;
	}
	CIPHER_TEXT[0]=PACKET_TEMP[8];
	CIPHER_TEXT[1]=PACKET_TEMP[9];
	DECRYPT_ALGORISM_1_V1(DATA_READ_BUFFER,CIPHER_TEXT,2,SESSION_KEY);
	return 1;
}
