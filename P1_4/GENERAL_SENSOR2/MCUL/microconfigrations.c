/*
 * microconfigrations.c
 *
 * Created: 04/09/2018 01:28:50 م
 *  Author: ideapad510
 */ 
#include "microconfigrations.h"
	void gpio_inputconfg(volatile uint8 * DiR_address,volatile uint8 * inputmodreg_address,INPUT_MOD mod,uint8 pin)
	{
		clearbit(*DiR_address,pin);
		clearbit(*inputmodreg_address,pin);
				*inputmodreg_address|=mod<<pin;
				
	}
	
	
	
	
	void gpio_outputconfg(volatile uint8 * DiR_address,volatile uint8 * outputmodreg_address,OUTPUTMOD mod,uint8 pin)
	{
		
		setbit(*DiR_address,pin);
		clearbit(*outputmodreg_address,pin);
		*outputmodreg_address|=mod<<pin;
		
	}
	
		void DIGwrite(volatile uint8 * PORT_address,uint8 pin, IO_SIGNAL signal)
		{
				clearbit(*PORT_address,pin);
				*PORT_address|=signal<<pin;
		}
		IO_SIGNAL DIGread(volatile uint8 * PIN_address,uint8 pin)
				{
					if(isbitset(*PIN_address,pin))
					return HIGH;
			return LOW;
			
		}
	