/*
 * MAIN_DRIVER.c
 *
 * Created: 05-Apr-21 4:07:18 PM
 *  Author: Mina Medhat
 */ 
#include "MAIN_DRIVER.h"
void SETUP()
{
	_delay_ms(1000);
	SPI_INT(SPI_MASTER,SPI_SCALE_16,SPI_MODE_0,SPI_INT_DISABLE,SPI_MSF);
	SENSOR_IO_SETUP();
	OTHER_SETUPS_IO();
	TEST_IO_SETUP();
	APP_IO_SETUP();
	
	
	
	
	TEST_SETUP();
	 
	 
	 
	APP_SETUP(); 
	TIMERS_SETUP();
	USART_SEND_str("NEW here start");
	SENSOR_SETUP();
	setbit(*PORT_OUT_READY_LED,READY_LED_PIN);

}

void LOOP()
{
	
	 SENSOR_MANAGE_LOOP();
	 APP_LOOP();
	 /*USART_SEND_str("loop\n");*/
	 toggybit(*PORT_OUT_READY_LED,READY_LED_PIN);
}

void OTHER_SETUPS_IO()
{
	gpio_outputconfg(PORT_DDR_READY_LED,PORT_OUTMOD_READY_LED,OUTPASS,READY_LED_PIN);
	
}
