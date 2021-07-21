/*
 * SENSOR_MANAGE.h
 *
 * Created: 05-Apr-21 6:44:31 PM
 *  Author: Mina Medhat
 */ 


#ifndef SENSOR_MANAGE_H_
#define SENSOR_MANAGE_H_
#include "SENSOR_MANAGE_DEFINE.h"
#include "protocol.h"





void SENSOR_IO_SETUP();
void SENSOR_SETUP();
void SENSOR_MANAGE_LOOP();
void SENSOR_TIMER1();


#endif /* SENSOR_MANAGE_H_ */