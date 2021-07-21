/*
 * SENSOR_MANAGE.c
 *
 * Created: 05-Apr-21 6:44:23 PM
 *  Author: Mina Medhat
 */ 
#include "SENSOR_MANAGE.h"

extern volatile SENSOR_STATE_DT F_SENSOR_STATE;
extern volatile unsigned char MASTER_ADDRESS[5];
extern NRF_RF_CHANNEL_DT MY_CHANNELS[];


volatile unsigned char TEMP=0;
extern volatile unsigned char DATA_BUFFER[18];
extern volatile unsigned char LENGTH;






volatile unsigned char C_T_BEACON=0;

volatile unsigned short MAIN_REGISTERS[20];


void SENSOR_MANAGE_LOOP()
{	
	if (NRF_CHECK_RX_PL(DATA_BUFFER,&LENGTH))
	{
		USART_SEND_str("R\n");
		volatile unsigned char TEMP=0;
		if (F_SENSOR_STATE==CONNECTED_STATE)
		{
			TEMP=1;	
		}		
		if (PROTOCOL_CHECK_RECIVED_PACKET(DATA_BUFFER,TEMP,MASTER_ADDRESS, LENGTH))
		{
		/*USART_SEND_str("checked1\n");*/
			PROTOCOL_SWITCH();
		}		
		NRF_START_RX_MODE();
	}
	
	if (F_SENSOR_STATE==CONNECTED_STATE)
	{
		
		if (C_T_BEACON==MAX_C_T_BEACON)
		{
			C_T_BEACON=0;
			F_SENSOR_STATE=NOTCONNECTED_STATE;
			NRF_STOP_RX_MODE();
			NRF_UPDATE_USED_CHANNEL(MY_CHANNELS[0]);
			NRF_START_RX_MODE();
			clearbit(*PORT_OUT_CONNECTED_LED,CONNECTED_LED_PIN);
			USART_SEND_str("DISCONNECTED\n");
		}
	}
	

			
		
	
}

void SENSOR_IO_SETUP()
{
	PROTOCOL_IO_SETUP();
	gpio_outputconfg(PORT_DDR_CONNECTED_LED,PORT_OUTMOD_CONNECTED_LED,OUTPASS,CONNECTED_LED_PIN);
}

void SENSOR_SETUP()
{
	 PROTOCOL_SETUP();
	 
}

void SENSOR_TIMER1()
{
	if (C_T_BEACON!=MAX_C_T_BEACON)
	{
		C_T_BEACON++;
	}
}