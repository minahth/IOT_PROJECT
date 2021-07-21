/*
 * WIFI.h
 *
 * Created: 30-Mar-21 6:55:34 PM
 *  Author: Mina Medhat
 */ 
#ifndef WIFI_H_
#define WIFI_H_
#include "HARDWARE_CONNECTIONS.h"
#include "ESP_WIFI_SIMPLIFIED.h"
#include "WIFI_DEFINE.h"
#include "EEPROM_ALLOCATIONS.h"
#include "eeprom.h"
#include "queue.h"
void WIFI_SETUP_IO();
void WIFI_SETUP();
void WIFI_LOAD_PARAMETERS();
void WIFI_LOOP();
void WIFI_UPDATE_USER_PASS(volatile unsigned char*LOCAL_USER,volatile unsigned char*LOCAL_PASS);
void WIFI_UPDATE_IOT_PAR(volatile unsigned char *CHANNEL_ID,volatile unsigned char CHANNEL_ID_LENGTH,volatile unsigned char *API_WRITE_KEY,volatile unsigned char API_WRITE_KEY_LENGTH,volatile unsigned char FIELD_NUM);
void WIFI_TIMER1();

void WIFI_RESET();


#endif /* WIFI_H_ */