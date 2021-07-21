/*
 * protocol.h
 *
 * Created: 02-Apr-21 2:29:04 PM
 *  Author: Mina Medhat
 */ 

/******************** THIS PROTOCOL FOR SENSOR ********************/
#ifndef PROTOCOL_H_
#define PROTOCOL_H_
#include "protocol_define.h"
#include "SECURITY_DRIVER.h"
#include "NRF24L01.h"
#include "RANDOM_NUM_GEN.h"







#define PROTOCOL_READ_PACKET_TYPE() (DATA_BUFFER[7])
#define PROTOCOL_READ_VER_NUM() (DATA_BUFFER[1])




void PROTOCOL_SETUP();
void PROTOCOL_IO_SETUP();
unsigned char PROTOCOL_CHECK_RECIVED_PACKET(unsigned char *PACKET, unsigned char CHECK_ADDRESS, unsigned char *MASTER_ADDRESS,unsigned char LENGTH);








unsigned char PROTOCOL_BEACON();
unsigned char PROTOCOL_SESSION_KEY();
unsigned char PROTOCOL_CONNECT();
unsigned char PROTOCOL_READ_DATA();
unsigned char PROTOCOL_CHANGE_DATA();
unsigned char PROTOCOL_CHANGE_CHANNEL();

void PROTOCOL_SWITCH();











#endif /* PROTOCOL_H_ */