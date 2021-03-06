#include "APP.h"

extern volatile unsigned short MAIN_REGISTERS[20];
ADC_APP_SELECTOR_DT ADC_APP_SELECTOR=LDR_SENSOR;
volatile unsigned char T_PIR_COUNTER=0;
volatile unsigned char F_PIR_DETECTED=0;


void APP_IO_SETUP()
{
	gpio_inputconfg(PORT_DDR_HEAT,PORT_INMOD_HEAT,FLOAT_mod,HEAT_PIN);
	gpio_inputconfg(PORT_DDR_LDR,PORT_INMOD_LDR,FLOAT_mod,LDR_PIN);
	gpio_inputconfg(PORT_DDR_PIR,PORT_INMOD_PIR,FLOAT_mod,PIR_PIN);
	
}
void APP_SETUP()
{
	ADC_INIT(ADC_DIVISION_128,ADC_AVCC,SINGLE_MODE,ADC_INTERRUPT_DISABLE,right);
	ADC_START_READ_INT(LDR_ADC_PIN);/*EGNITS THE FIRST ADC CONVERTION*/
}
void APP_LOOP()
{
	if (isbitset(ADC_CTRL_STATUS_ACCESS,4))/*CHECK FOR ADC CONVERTION COMPLETED STATE*/
	{
	/*	USART_SEND_str("X\n");*/
		setbit(ADC_CTRL_STATUS_ACCESS,4);/*CLEAR ADC CONVERTION COMPLETED STATE FLAG*/
		
		switch (ADC_APP_SELECTOR)
		{
			/*case HEAT_SENSOR:MAIN_REGISTERS[5]=(unsigned char)TEMP_MEASURE(ADC_10_BIT);
			ADC_APP_SELECTOR=LDR_SENSOR;
			ADC_START_READ_INT(LDR_ADC_PIN);
			break;*/
			case LDR_SENSOR:MAIN_REGISTERS[6]=(unsigned short)(ADC_10_BIT);
			ADC_APP_SELECTOR=LDR_SENSOR;
			ADC_START_READ_INT(LDR_SENSOR);
			break;
		}
	}
	
	if (isbitset(*PORT_IN_PIR,PIR_PIN))
	{
		_delay_ms(1);
		if (isbitset(*PORT_IN_PIR,PIR_PIN))
		{
			T_PIR_COUNTER=0;
			F_PIR_DETECTED=1;
			MAIN_REGISTERS[7]=1;
		}
	}
	
	
	
	
	
}
void APP_TIMER1()
{
	
	if (F_PIR_DETECTED)
	{
		T_PIR_COUNTER++;
		if(T_PIR_COUNTER==T_PIR_COUNTER_MAX)
		{
			F_PIR_DETECTED=0;
			T_PIR_COUNTER=0;
			MAIN_REGISTERS[7]=0;
		}
	}
	
	
	
	
}