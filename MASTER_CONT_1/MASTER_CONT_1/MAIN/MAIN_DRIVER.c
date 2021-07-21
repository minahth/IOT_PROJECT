/*
 * MAIN_DRIVER.c
 *
 * Created: 05-Apr-21 4:07:18 PM
 *  Author: Mina Medhat
 */ 
#include "MAIN_DRIVER.h"

volatile unsigned char F_B_RESET_SENSORS=0;
volatile unsigned char F_B_RESET_ALL=0;

void SETUP()
{
	_delay_ms(1000);
	SPI_INT(SPI_MASTER,SPI_SCALE_16,SPI_MODE_0,SPI_INT_DISABLE,SPI_MSF);
	BLUETOOTH_IO_SETUP();
	LED_GRID_IO_SETUP();
	PROTOCOL_IO_SETUP();
	SENSORS_IO_SETUP();
	WIFI_SETUP_IO();
	OTHER_SETUPS_IO();
	TEST_IO_SETUP();
	
	
	BLUETOOTH_SETUP();
	
	/*TESTING*/
	/*USART0_INIT(ASYNCHRONOUS_DOUBLE,UART_INT_DISABLE,UART_INT_DISABLE,UART_INT_DISABLE,TX_RX_ENABLE,PARITY_DISABLE_1STOP,DATA_8,500000U);*/
	/*TESING*/
	
	
	
	
	/*WIFI*/
	USART1_INIT(ASYNCHRONOUS_NORMAL,UART_INT_DISABLE,UART_INT_DISABLE,UART_INT_DISABLE,TX_RX_ENABLE,PARITY_DISABLE_1STOP,DATA_8,UART1_BAUD_RATE);
	/**************/
	
	
	
	TEST3_SET();
	LED_GRID_SETUP();
	TIMERS_SETUP();
	SENSORS_SETUP();
	TEST_UART_SEND_str("READY1\n");
	PROTOCOL_SETUP();
	WIFI_SETUP();
	setbit(*PORT_OUT_READY_LED,READY_LED_PIN);
	
	 
	/* TEST_ADD_USER_PASS();
	 TEST_ADD_IOT_PARAMETERS();*/
	TEST_UART_SEND_str("READY2\n");
}

void LOOP()
{
	/* TEST_ADD_SENSOR();*/
	 LOOP_LED_GRID();
	 WIFI_LOOP();
	 LOOP_SENSORS_MANAGE();
	 BLUETOOTH_LOOP();
	 SENSORS_RESET_LOOP();
	  RESET_ALL_LOOP();
	 toggybit(*PORT_OUT_TOGGLE_LED,TOGGLE_LED_PIN);
}

void OTHER_SETUPS_IO()
{
	gpio_outputconfg(PORT_DDR_READY_LED,PORT_OUTMOD_READY_LED,OUTPASS,READY_LED_PIN);
	gpio_outputconfg(PORT_DDR_TOGGLE_LED,PORT_OUTMOD_TOGGLE_LED,OUTPASS,TOGGLE_LED_PIN);
	
}

void SENSORS_RESET_LOOP()
{
	if (F_B_RESET_SENSORS)
	{
		F_B_RESET_SENSORS=0;
		DELETE_ALL_SENSOR();
	}
}


void RESET_ALL_LOOP()
{
	if (F_B_RESET_ALL)
	{
		F_B_RESET_ALL=0;
		DELETE_ALL_SENSOR();
		WIFI_RESET();
	}
}