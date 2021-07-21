/*
 * protocol.h
 *
 * Created: 02-Apr-21 2:29:04 PM
 *  Author: Mina Medhat
 */ 

/******************** THIS PROTOCOL FOR MASTER ********************/
#ifndef PROTOCOL_H_
#define PROTOCOL_H_
#include "protocol_define.h"
#include "SECURITY_DRIVER.h"
#include "NRF24L01.h"

void PROTOCOL_SETUP();
void PROTOCOL_IO_SETUP();
unsigned char PROTOCOL_CHECK_RECIVED_PACKET(unsigned char *PACKET, unsigned char *SENSOR_ADDRESS,unsigned char LENGTH,unsigned char*KEY);

unsigned char PROTOCOL_MASTER_CONNECT_TO_SENSOR(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*PASSWORD_KEY);
unsigned char PROTOCOL_SEND_BEACON(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY);
unsigned char PROTOCOL_SEND_SESSION_KEY(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY,unsigned char*PASSWORD_KEY);
unsigned char PROTOCOL_CHANGE_CHANNEL(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY,unsigned char CHANNEL);
unsigned char PROTOCOL_DATA_SEND(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY,unsigned char *REGISTER_ADDRESS,unsigned char*DATA_SEND_BUFFER);
unsigned char PROTOCOL_DATA_READ(unsigned char*SENSOR_ADDRESS,unsigned char SENSOR_VERSION,unsigned char SENSOR_ADDRESS_LENGTH,unsigned char*SESSION_KEY,unsigned char *REGISTER_ADDRESS,unsigned char*DATA_READ_BUFFER);




#endif /* PROTOCOL_H_ */