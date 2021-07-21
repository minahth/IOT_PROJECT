/*
 * TIMER.c
 *
 * Created: 31-Mar-21 11:24:16 PM
 *  Author: Mina Medhat
 */ 
#include "TIMER.h"



void TIMERS_SETUP()
{
	
	TIMER1_ENABLE(T1_OCA1_DISCONNECT,T1_OCB1_DISCONNECT,T1_NORMAL,T1_TIMER_256,0,0,ENABLE_TIMER1_INT,DISABLE_TIMER1_INT,DISABLE_TIMER1_INT,DISABLE_TIMER1_INT);
	TIMER0_ENABLE(TIMER_64,NORMAL_MOD,OC0_DISCONNECTED,OVER_FLOW_INT_ENABLE,0);
	global_interupt_enable();
}

void TIMER0_INT_EVENT()
{
	APP_TIMER0();
	
}

void TIMER1_INT_EVENT()
{
	SENSOR_TIMER1();
	APP_TIMER1();
}
