/*
 * TEST.c
 *
 * Created: 09-Apr-21 4:48:19 PM
 *  Author: Mina Medhat
 */ 
#include "TEST.h"
#include "UART_2P.h"
#include "SENSOR_MANAGE_DEFINE.h"
extern volatile unsigned char F_B_ADD_NEW_SENSOR;
extern volatile SENSOR_DT B_NEW_SENSOR_BUFFER;
extern volatile unsigned char F_B_USER_NAME[15];
extern volatile unsigned char F_B_PASSWORD[15];
extern volatile unsigned char F_B_NEW_USER_PASS;

extern volatile unsigned char F_B_CH_ID[10];
extern volatile unsigned char F_B_WRITE_KEY[25];
extern volatile unsigned char F_B_FIELD_NUM;
extern volatile unsigned char F_B_CH_ID_LENGTH;
extern volatile unsigned char F_B_WRITE_KEY_LENGTH;
extern volatile unsigned char F_B_NEW_IOT_PAR;

void TEST_IO_SETUP()
{
	gpio_outputconfg(PORT_DDR_TEST_1,PORT_OUTMOD_TEST_1,OUTPASS,TEST_1_PIN);
	gpio_outputconfg(PORT_DDR_TEST_2,PORT_OUTMOD_TEST_2,OUTPASS,TEST_2_PIN);
	gpio_outputconfg(PORT_DDR_TEST_3,PORT_OUTMOD_TEST_3,OUTPASS,TEST_3_PIN);
}

void TEST_ADD_SENSOR()
{
	static volatile unsigned char i=0;
	if (i==0)
	{
		B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[0]=0x50;/*least*/
		B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[1]=0x50;
		B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[2]=0x50;
		B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[3]=0x50;
		B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[4]=0x50;/*MOST*/
		
	
	
		B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[0]=0X47;
		B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[1]=0X47;
		B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[2]=0X47;
		B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[3]=0X47;
		B_NEW_SENSOR_BUFFER.SENSOR_VERSION=1;
		B_NEW_SENSOR_BUFFER.SENSOR_TYPE=3;
	
		F_B_ADD_NEW_SENSOR=1;
		++i;
	}
	if (i==1)
	{
		if (!F_B_ADD_NEW_SENSOR)
		{
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[0]=0X40;/*LEAST*/
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[1]=0x40;
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[2]=0X40;
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[3]=0X40;
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[4]=0X40;/*MOST*/
			
		
		
		
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[0]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[1]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[2]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[3]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_VERSION=1;
			B_NEW_SENSOR_BUFFER.SENSOR_TYPE=3;
		
			F_B_ADD_NEW_SENSOR=1;
			++i;
		}
		
	}
	if (i==2)
	{
		if (!F_B_ADD_NEW_SENSOR)
		{
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[0]=0X30;/*LEAST*/
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[1]=0x30;
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[2]=0X30;
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[3]=0X30;
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[4]=0X30;/*MOST*/
			
			
			
			
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[0]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[1]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[2]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[3]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_VERSION=1;
			B_NEW_SENSOR_BUFFER.SENSOR_TYPE=1;
			
			F_B_ADD_NEW_SENSOR=1;
			++i;
		}
		
	}
	if (i==3)
	{
		if (!F_B_ADD_NEW_SENSOR)
		{
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[0]=0X20;/*LEAST*/
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[1]=0x20;
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[2]=0X20;
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[3]=0X20;
			B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[4]=0X20;/*MOST*/
			
			
			
			
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[0]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[1]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[2]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_PASSWORD[3]=0X47;
			B_NEW_SENSOR_BUFFER.SENSOR_VERSION=1;
			B_NEW_SENSOR_BUFFER.SENSOR_TYPE=1;
			
			F_B_ADD_NEW_SENSOR=1;
			++i;
		}
		
	}
/*
	B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[0]=0x50;
	B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[1]=0x50;
	B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[2]=0x50;
	B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[3]=0x50;
	B_NEW_SENSOR_BUFFER.SENSOR_ADDRESS[4]=0x50;*/
	
	
}

void TEST_ADD_IOT_PARAMETERS()
{

		volatile unsigned char i;
		volatile unsigned char ID_TEST[]="135629200000000";
		volatile unsigned char KEY_TEST[]="9KKBO2YKRFLTXKD69KKBO2YKRFLTXKD6";
		
		
		F_B_CH_ID_LENGTH=7;
		for (i=0;i<10;++i)
		{
			F_B_CH_ID[i]=ID_TEST[i];
		}
		F_B_WRITE_KEY_LENGTH=16;
		
		for (i=0;i<25;++i)
		{
			F_B_WRITE_KEY[i]=KEY_TEST[i];
		}
		F_B_FIELD_NUM=1;
		F_B_NEW_IOT_PAR=1;

}

void TEST_ADD_USER_PASS()
{
	volatile unsigned char USER_TEST[]="engmina";
	volatile unsigned char PASS_TEST[]="4TSKMN3af";
	
	/*volatile unsigned char USER_TEST[]="king_jesus";
	volatile unsigned char PASS_TEST[]="ZF19CH75ak";*/
	
	
	
	unsigned char i=0;
	do 
	{
		F_B_USER_NAME[i]=USER_TEST[i];
		++i;
	} while (USER_TEST[i-1]!=0);
	i=0;
	do
	{
		F_B_PASSWORD[i]=PASS_TEST[i];
		++i;
	} while (F_B_PASSWORD[i-1]!=0);
	
	F_B_NEW_USER_PASS=1;
}

void TEST_UART_SEND_ARRAY(char*str,unsigned char LENGTH)
{
	/*USART0_SEND_ARRAY(str,LENGTH);*/
}

void TEST_UART_SEND_str(char*str)
{
	/*USART0_SEND_str(str);*/
}

void TEST_UART_SEND_byte(volatile unsigned char byte)
{
	/*USART0_SEND_byte(byte);*/
}
void TEST_UART_SEND_VALUE(long long X)
{
	unsigned char VALUE[15];
	/*USART0_SEND_ARRAY(VALUE,inttostring(VALUE,X));*/
}