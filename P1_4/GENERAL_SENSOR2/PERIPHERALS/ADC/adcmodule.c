/*
 * adcmodule.c
 *
 * Created: 19/10/2018 11:07:46 ص
 *  Author: ideapad510
 */ 
#include "adcmodule.h"

	void ADC_INIT (ADC_PRESCALE P,ADC_VOLT_REF VF,ADC_MODE MODE ,ADC_INTERRUPT_MODE INT_STATE,ADC_ADJUST ADC_DATA_ORDER)
	{
		ADMUX_ACCESS=0x00;
		ADMUX_ACCESS|=VF|ADC_DATA_ORDER;	
		ADC_CTRL_STATUS_ACCESS = P|MODE|(1<<7)|INT_STATE;
		switch (MODE)
		{case SINGLE_MODE:break;
			default:
				  SFIOR&=0x1F;
			      SFIOR|=(MODE>>8);
				  break;
		}
		
	}
	void ADC_DISABLE()
	{
		clearbit(ADC_CTRL_STATUS_ACCESS,7);/*disable*/
		
	}
	uint16 ADC_READ(ADC_CHANNEL channel,ADC_RESOLUTION resolution)
	{ 
		ADMUX_ACCESS&=0xE0;
		ADMUX_ACCESS|=channel;
		setbit(ADC_CTRL_STATUS_ACCESS,6);/* adc start*/
		while(isbitclear(ADC_CTRL_STATUS_ACCESS,4))/*ADIF*/
		{
			
		}
		/* to clear flag 7ot 1*/
		setbit(ADC_CTRL_STATUS_ACCESS,4);
		
		
		switch(resolution)
		{case _8_BIT:return ADC_8_BIT;
				case _10_BIT:return ADC_10_BIT;
		}
	return -1;
	}

void ADC_START_READ_INT(ADC_CHANNEL channel)
{
	ADMUX_ACCESS&=0xE0;
	ADMUX_ACCESS|=channel;
	setbit(ADC_CTRL_STATUS_ACCESS,6);/* adc start*/
}

	