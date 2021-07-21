/*
 * BLUETOOTH_DEFINE.h
 *
 * Created: 30-Mar-21 10:51:37 PM
 *  Author: Mina Medhat
 */ 


#ifndef BLUETOOTH_DEFINE_H_
#define BLUETOOTH_DEFINE_H_

#define UART0_BAUD_RATE 115200U
#define ACK_COMMAND 0XAA
#define NACK_COMMAND 0XDD
/*RECIVEING ANDROID TO MASTER*/ 
#define RECIVE_USER_PASS_COMMAND 1
#define RECIVE_IOT_PAR_COMMAND 2
#define RECIVE_ADD_NEW_SENSOR_COMMAND 3
#define RECIVE_ENABLE_SENSOR_COMMAND 4
#define RECIVE_DISABLE_SENSOR_COMMAND 5
#define RECIVE_DELETE_SENSOR_COMMAND 6
#define RECIVE_ADJUST_LED_COMMAND 7

/*READING MASTER TO ANDROID*/
#define READ_STATE_OF_SENSOR_COMMAND 8
#define READ_CONNECTION_OF_SENSOR_COMMAND 9
#define READ_NUM_OF_SENSORS_COMMAND 10
#define READ_MASTER_VER_COMMAND 11
#define READ_MASTER_ADDRESS_COMMAND 12
#define READ_SENSOR_INFO_COMMAND 13
#define DELETE_ALL_SENSORS_COMMAND 14
#define RESET_ALL_COMMAND 15

#define ROBUST_ACK_MODE 1

#define RESET_SURE_FLAG 45

typedef enum {
	BLUETOOTH_IDLE,
	/*EVENTS*/
	BLUETOOTH_DELETE_ALL_SESNORS,
	BLUETOOTH_RESET_ALL,
	/*RECIVEING*/
	BLUETOOTH_RECIVING_USER_PASS,
	BLUETOOTH_RECIVING_IOT_PAR,
	BLUETOOTH_RECIVING_ADD_SENSOR,
	BLUETOOTH_RECIVING_ENABLE_SENSOR,
	BLUETOOTH_RECIVING_DISABLE_SENSOR,
	BLUETOOTH_RECIVING_DELETE_SENSOR,
	BLUETOOTH_RECIVING_ADJUST_LED,
	
	
	/*READING*/
	BLUETOOTH_READING_STATE_SENSOR,
	BLUETOOTH_READING_CONNECT_SENSOR,
	BLUETOOTH_READING_SENSOR_INFO,
	
	
}BLUETOOTH_STATE_TD;

#endif /* BLUETOOTH_DEFINE_H_ */