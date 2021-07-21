/*
 * APP.h
 *
 * Created: 25-Apr-21 1:58:31 PM
 *  Author: Mina Medhat
 */ 


#ifndef APP_H_
#define APP_H_

#include "microconfigrations.h"
#include "HARDWARE_CONNECTIONS.h"
#include "APP_DEFINE.h"
#include "APP_CONFIG.h"
#include "adcmodule.h"
#include "eeprom.h"


#define CLOSE_THE_RELAY() (setbit(*PORT_OUT_RELAY,RELAY_PIN))
#define OPEN_THE_RELAY()  (clearbit(*PORT_OUT_RELAY,RELAY_PIN))



#define TEMP_MEASURE(SHORT_ADC) ((float32)SHORT_ADC/2.048)



void APP_IO_SETUP();
void APP_SETUP();
void APP_LOOP();
void APP_TIMER1();


#endif /* APP_H_ */