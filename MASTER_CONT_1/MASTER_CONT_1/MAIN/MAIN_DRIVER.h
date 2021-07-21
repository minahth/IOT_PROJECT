/*
 * MAIN_DRIVER.h
 *
 * Created: 05-Apr-21 4:07:34 PM
 *  Author: Mina Medhat
 */ 


#ifndef MAIN_DRIVER_H_
#define MAIN_DRIVER_H_
#include "BLUETOOTH.h"
#include "TIMER.h"
#include "WIFI.h"
#include "SENSOR_MANAGE.h"
#include "LED_GRID.h"
#include "protocol.h"
#include "spi.h"

void SETUP();
void LOOP();
void SENSORS_RESET_LOOP();
void RESET_ALL_LOOP();
void OTHER_SETUPS_IO();

#endif /* MAIN_DRIVER_H_ */