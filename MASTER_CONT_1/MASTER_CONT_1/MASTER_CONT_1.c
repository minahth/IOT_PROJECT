/*
 * MASTER_CONT_1.c
 *
 * Created: 05-Feb-21 11:22:12 PM
 *  Author: MINA MEDHAT
 */ 

#include "MAIN_DRIVER.h"
#include <avr/interrupt.h>
#include <avr/io.h>

ISR(TIMER1_OVF_vect)
{TEST3_TOGGLE();
	TIMER1_INT_EVENT();
}
ISR(TIMER0_OVF_vect)
{
	 TIMER0_INT_EVENT();
}
ISR(USART0_RX_vect)
{

	RX0_ISR();
}

ISR(TIMER2_OVF_vect)
{
	TIMER2_ISR();
}

ISR(INT6_vect)
{
	INT6_ISR();
}

ISR(USART0_TX_vect)
{
	TX0_ISR();
}

ISR(BADISR_vect)
{
	
}

int main(void)
{
	
	SETUP();
	while(1)
	{
		LOOP();
	}
}