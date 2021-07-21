/*
 * SENSOR_MANAGE.h
 *
 * Created: 31-Mar-21 5:41:29 PM
 *  Author: Mina Medhat
 */ 


#ifndef SENSOR_MANAGE_H_
#define SENSOR_MANAGE_H_
#include "SENSOR_MANAGE_DEFINE.h"
#include "EEPROM_ALLOCATIONS.h"
#include "eeprom.h"
#include "protocol.h"
#include "RANDOM_NUM_GEN.h"
#include "WIFI.h"
/*
DATA BASE 
NUMBER OF SENSORS 10
SENSOR 1 1
SENSOR 2 1
SENSOR 3 1
SENSOR 4 1
SENSOR 5 1
SENSOR 6 1
SENSOR 7 1
SENSOR 8 1
SENSOR 9 1
SENSOR 10 1
*/







void SENSOR_MANAGE_TASK(void * pd);


























void SENSORS_IO_SETUP();


void SENSORS_LOAD_EEPROM();
char FIND_SENSOR(unsigned char* ADDRESS,unsigned char ADDRESS_LENGTH);


/*functions in loop*/
char ADD_NEW_SENSOR(SENSOR_DT NEW_SENSOR);
void DELETE_SENSOR(unsigned char* ADDRESS,unsigned char ADDRESS_LENGTH);
void DELETE_ALL_SENSOR();
void SENSOR_STATE_ENABLE(unsigned char* ADDRESS,unsigned char ADDRESS_LENGTH);
void SENSOR_STATE_DISABLE(unsigned char* ADDRESS,unsigned char ADDRESS_LENGTH);
void SENSORS_CONNECT();
void SENSORS_BEACON();
void SENSORS_NEW_SESSIONS();
void SENSORS_CHANGE_CHANNEL();
void SENSORS_FILL_SENSOR_BUFFERS();
void SENSORS_APPLY_SEND_BUFFER();
void SENSORS_APPLY_READ_BUFFER();

/**/
void SENSORS_RESET_LOOP();
void RESET_ALL_LOOP();
void LOOP_SENSORS_MANAGE();
/*local*/
void SENSOR_SAVE(unsigned char index);
void SENSOR_SAVE_ALL();
void NUM_SENSOR_SAVE();

void SAVE_OLD_LAST_ENTRY();
void LOAD_OLD_LAST_ENTRY();
void INT6_ISR();

void SENSOR_TIMER3();

#endif /* SENSOR_MANAGE_H_ */