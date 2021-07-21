/*
 * SENSOR_MANAGE_DEFINE.h
 *
 * Created: 31-Mar-21 5:41:47 PM
 *  Author: Mina Medhat
 */ 


#ifndef SENSOR_MANAGE_DEFINE_H_
#define SENSOR_MANAGE_DEFINE_H_


/*TIMER_PARAMETERS IN SEC*/

#define MAX_TIMER_CONNECT_COUNT_SENSOR 20 /*5 SEC*/

#define MAX_TIMER_BEACON_COUNTER 8 /*2 SEC*/
#define MAX_FAILED_BEACON_COUNTER 5
#define MAX_SESSION_KEY_TIMER_COUNT 40 /*10 SEC*/

#define CONTROL_CHANNEL_INDEX 0

#define MAX_SEND_FAIL_COUNTER 3
#define MAX_READ_FAIL_COUNTER 3







typedef enum{
			  P0_SENSOR,
			  P1_SENSOR,
			  P2_SENSOR,
			  P3_SENSOR,
			}SENSOR_TYPE_DT;

typedef struct {
					volatile unsigned char SENSOR_ADDRESS_LENGTH;/*5*/
					
					volatile unsigned char SENSOR_ADDRESS[5];
					volatile unsigned char SENSOR_PASSWORD[4];
					volatile unsigned char SENSOR_VERSION;
					volatile SENSOR_TYPE_DT SENSOR_TYPE;
					volatile unsigned char SENSOR_STATE;
					volatile unsigned char SENSOR_OCCUBIED;/*THIS VARIABLE REFERS TO IF THE SENSOR IS USED WILL BE 1 IF NO WILL BE 0 IF DELETED WILL BE ZERO */
					
					
					
					volatile unsigned char F_CONNECTED;
					volatile unsigned char C_T_TRY_CONNECTED;
					volatile unsigned char C_T_BEACON;
					volatile unsigned char C_FAILED_BEACON;
					volatile unsigned char C_T_SESSION_KEY;
					volatile unsigned char USED_CHANNEL;
					
					volatile unsigned char F_HAVE_SESSION_KEY;
					volatile unsigned char SESSION_KEY[2];
					
					volatile unsigned char SEND_FAIL_COUNTER;
					volatile unsigned char READ_FAIL_COUNTER;
		}SENSOR_DT;



typedef enum{
	IOT_WRITE_DATA='W',		
	IOT_READ_DATA='R',	
			
			
			
	}IOT_COMMAND_DT;


typedef struct {
				unsigned char SENSOR_INDEX;
				unsigned char ADDRESS_REGISTER[2];
				unsigned char NEW_VALUE[2];
	
				}SENSOR_SEND_BUFFER_DT;


typedef struct {
	unsigned char SENSOR_INDEX;
	unsigned char ADDRESS_REGISTER[2];
	unsigned char READ_VALUE[2];
	
}SENSOR_READ_BUFFER_DT;





#endif /* SENSOR_MANAGE_DEFINE_H_ */