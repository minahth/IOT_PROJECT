/*
 * MASTER_CONT_RTOS.c
 *
 * Created: 05-Jul-21 3:26:50 AM
 *  Author: Mina Medhat
 */ 




#include "microconfigrations.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "MAIN_DRIVER.h"
/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"


/*testing rtos*/
volatile  char r;


ISR(INT0_vect)
{
	INT0_NRF_ISR();
}

ISR(USART1_RX_vect)
{

	RX_ESP_ISR();
}
ISR(USART1_TX_vect)
{

	TX_ESP_ISR();
}


ISR(TIMER0_OVF_vect)
{
	TIMER0_INT_EVENT();
}

ISR(TIMER3_OVF_vect)
{
	TIMER3_INT_EVENT();
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
;
}



xTaskHandle WIFI_handle,SENSOR_MANAGE_handle;


int main(void)
{
	SETUP();
	TEST_UART_SEND_str("WIFI STACK=");
	TEST_UART_SEND_VALUE(xTaskCreate(WIFI_TASK,"W",400,NULL,1,&WIFI_handle));
	TEST_UART_SEND_str("SENSOR STACK=");
	TEST_UART_SEND_VALUE(xTaskCreate(SENSOR_MANAGE_TASK,"k",550,NULL,1,&SENSOR_MANAGE_handle));
	
	
	// Start the RTOS kernel
	vTaskStartScheduler();
	/* Do nothing here and just run infinite loop */
	
	while(1)
	{
		
		
	};
	return 0;
}

