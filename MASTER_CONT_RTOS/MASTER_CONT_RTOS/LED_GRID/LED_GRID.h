/*
 * LED_GRID.h
 *
 * Created: 01-Apr-21 1:25:17 PM
 *  Author: Mina Medhat
 */ 


#ifndef LED_GRID_H_
#define LED_GRID_H_
#include "microconfigrations.h"
#include "HARDWARE_CONNECTIONS.h"
#include "LED_GRID_DEFINE.h"
#include "EEPROM_ALLOCATIONS.h"
#include "eeprom.h"

#define CHANGE_LED_GRID(VALUE,INDEX)(LED_GRID_ARRAY[INDEX]=VALUE) /*USED IN BLUETOOTH INTERUPT*/



void LED_GRID_SETUP();
void LED_GRID_IO_SETUP();
void LED_GRID_LOAD();

void LOOP_LED_GRID()/* __attribute__((always_inline))*/;
void LED_GRID_TIMER_INT();
#endif /* LED_GRID_H_ */