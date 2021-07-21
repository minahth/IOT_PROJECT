/*
 * GENERAL_SENSOR1.c
 *
 * Created: 05-Apr-21 6:06:33 PM
 *  Author: Mina Medhat
 */ 
#include "microconfigrations.h"
#include "MAIN_DRIVER.h"
#include <avr/io.h>
#include <avr/interrupt.h>




/*test*/
volatile unsigned short z=0;
/*check*/
ISR(INT2_vect)
{
	USART_SEND_str("BADISR 1\n");
	
}
ISR(TIMER2_COMP_vect)
{
		USART_SEND_str("BADISR 2\n");
}


ISR(TIMER1_CAPT_vect)
{
		USART_SEND_str("BADISR 3\n");
}
ISR(TIMER1_COMPA_vect)
{
	USART_SEND_str("BADISR 4\n");
}
ISR(TIMER1_COMPB_vect)
{
		USART_SEND_str("BADISR 5\n");
}
ISR(TIMER0_COMP_vect)
{
		USART_SEND_str("BADISR 6\n");
}
ISR(SPI_STC_vect)
{
	USART_SEND_str("BADISR 7\n");
}
ISR(USART_RXC_vect)
{
		USART_SEND_str("BADISR 8\n");
}
ISR(USART_UDRE_vect)
{
		USART_SEND_str("BADISR 9\n");
}
ISR(USART_TXC_vect)
{
		USART_SEND_str("BADISR 10\n");
}
ISR(ADC_vect)
{
		USART_SEND_str("BADISR 11\n");
}
ISR(EE_RDY_vect)
{
		USART_SEND_str("BADISR 12\n");
}
ISR(ANA_COMP_vect)
{
		USART_SEND_str("BADISR 13\n");
}
ISR(TWI_vect)
{
		USART_SEND_str("BADISR 14\n");
}

ISR(SPM_RDY_vect)
{
	USART_SEND_str("BADISR 15\n");
}



ISR(INT0_vect)
{
		USART_SEND_str("BADISR 16\n");
}

ISR(TIMER2_OVF_vect)
{
		USART_SEND_str("BADISR 17\n");
}
ISR(INT1_vect)
{
		USART_SEND_str("BADISR 18\n");
}

/*test*/


ISR(TIMER1_OVF_vect)
{
	TIMER1_INT_EVENT();
}
ISR(TIMER0_OVF_vect)
{
	TIMER0_INT_EVENT();
}
ISR(BADISR_vect)
{
	USART_SEND_str("BADISR\n");
}
int main(void)
{
   SETUP();
   while(1)
   {
	   LOOP();
   }
}