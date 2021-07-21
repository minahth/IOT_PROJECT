/*
 * TEST.c
 *
 * Created: 09-Apr-21 4:48:19 PM
 *  Author: Mina Medhat
 */ 
#include "TEST.h"
#include "minohelper.h"
extern volatile unsigned short MAIN_REGISTERS[20];
void TEST_IO_SETUP()
{
	gpio_outputconfg(PORT_DDR_TEST_1,PORT_OUTMOD_TEST_1,OUTPASS,TEST_1_PIN);
	gpio_outputconfg(PORT_DDR_TEST_2,PORT_OUTMOD_TEST_2,OUTPASS,TEST_2_PIN);
	gpio_outputconfg(PORT_DDR_TEST_3,PORT_OUTMOD_TEST_3,OUTPASS,TEST_3_PIN);
}

void TEST_SETUP()
{
	USART_INIT(ASYNCHRONOUS_NORMAL,INT_disable,TX_RX_ENABLE,PARITY_DISABLE_1STOP,DATA_8,9600);
	MAIN_REGISTERS[7]=0;
	MAIN_REGISTERS[8]=1;
}

void TEST_UART_SEND_VALUE(long long X)
{
	unsigned char VALUE[15];
	USART_SEND_ARRAY(VALUE,inttostring(VALUE,X));
}
