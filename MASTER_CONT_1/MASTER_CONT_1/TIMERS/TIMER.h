/*
 * TIMER.h
 *
 * Created: 31-Mar-21 11:24:23 PM
 *  Author: Mina Medhat
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "LED_GRID.h"
#include "WIFI.h"
#include "SENSOR_MANAGE.h"
#include "BLUETOOTH.h"

#include "TIMER0.h"
#include "TIMER1.h"
#include "TIMER2.h"



void TIMERS_SETUP();
void TIMER0_INT_EVENT();
void TIMER1_INT_EVENT();




#endif /* TIMER_H_ */