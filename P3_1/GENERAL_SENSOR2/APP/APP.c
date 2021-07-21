/*
 * APP.c
 *
 * Created: 25-Apr-21 1:58:20 PM
 *  Author: Mina Medhat
 */ 
#include "APP.h"

extern volatile unsigned short MAIN_REGISTERS[20];
volatile unsigned short T_PIR_COUNTER=0;
volatile unsigned char F_PIR_DETECTED=0;


void APP_IO_SETUP()
{
	gpio_inputconfg(PORT_DDR_PIR,PORT_INMOD_PIR,FLOAT_mod,PIR_PIN);
	gpio_outputconfg(PORT_DDR_RELAY,PORT_OUTMOD_RELAY,OUTPASS,RELAY_PIN);		
}
void APP_SETUP()
{	
	MAIN_REGISTERS[7]=1;
	MAIN_REGISTERS[8]=0;
	OPEN_THE_RELAY();
	MAIN_REGISTERS[6]=T_PIR_COUNTER_MAX;/*as default*/
	APP_LOAD_EEEPROM();
}
void APP_LOAD_EEEPROM()
{
	if (readeeprom(MY_EEPROM_FIRST_USE)==NOT_FIRST_USE_VALUE)
	{
		MAIN_REGISTERS[6]=(readeeprom(1)<<8)|readeeprom(2);
		MAIN_REGISTERS[7]=(readeeprom(3)<<8)|readeeprom(4);
		MAIN_REGISTERS[8]=(readeeprom(5)<<8)|readeeprom(6);
	}
	else
	{
		writeeeprom(MY_EEPROM_FIRST_USE,NOT_FIRST_USE_VALUE);
		USART_SEND_str("first time\n");
	}
}
inline void APP_SAVE_EEEPROM()
{
	writeeeprom(1,MAIN_REGISTERS[6]>>8);
	writeeeprom(2,MAIN_REGISTERS[6]);
	writeeeprom(3,MAIN_REGISTERS[7]>>8);
	writeeeprom(4,MAIN_REGISTERS[7]);
	writeeeprom(5,MAIN_REGISTERS[8]>>8);
	writeeeprom(6,MAIN_REGISTERS[8]);
}
void inline FILL_COMPO_REGISTER()
{
	MAIN_REGISTERS[9]=MAIN_REGISTERS[8]|(MAIN_REGISTERS[7]<<1)|(MAIN_REGISTERS[5]<<2)|(MAIN_REGISTERS[6]<<3);
}

void APP_LOOP()
{
	
	APP_SAVE_EEEPROM();
	global_interupt_disable();
	if(F_PIR_DETECTED)
	{
		MAIN_REGISTERS[5]=1;
			
			if (MAIN_REGISTERS[7])
			{	
					
				CLOSE_THE_RELAY();
			}
	}
	global_interupt_enable();
	
	if (MAIN_REGISTERS[7]==0)
	{
		
		if (MAIN_REGISTERS[8])
		{
			
			CLOSE_THE_RELAY();
		}
		else
		{
			OPEN_THE_RELAY();
		}	
	}
	
	
	FILL_COMPO_REGISTER();
	
	/*test*/
	
	
	/*	USART_SEND_str("\nstate: ");
		TEST_UART_SEND_VALUE(MAIN_REGISTERS[5]);
		USART_SEND_str("\n");*/
	/**/
	
	
}
void APP_TIMER1()
{
	
	if (F_PIR_DETECTED)
	{
		T_PIR_COUNTER++;
		if(T_PIR_COUNTER>MAIN_REGISTERS[6])
		{
			F_PIR_DETECTED=0;
			T_PIR_COUNTER=0;
			MAIN_REGISTERS[5]=0;
			if (MAIN_REGISTERS[7]==1)
			{
				
				OPEN_THE_RELAY();
			}
		}
	}
	
	
	
	
}


void APP_TIMER0()
{
	if (isbitset(*PORT_IN_PIR,PIR_PIN))
	{
		
		T_PIR_COUNTER=0;
		F_PIR_DETECTED=1;
	
	}
}