/*
 * protocol_define.h
 *
 * Created: 02-Apr-21 2:29:18 PM
 *  Author: Mina Medhat
 */ 


#ifndef PROTOCOL_DEFINE_H_
#define PROTOCOL_DEFINE_H_

/*SENSOR DATA*/



#define ADDRESS3030303030  0




#if ADDRESS3030303030
/*SENSOR DATA*/
#define MY_PROTOCOL_ADDRESS ((unsigned long long)0X3030303030)/*MY ADDRESS PROTOCOL*/ /*LET IN THIS VERSION ALL ADDRESSES ARE 5 BYTES*/

#define MY_PROTOCOL_ADDRESS_ARRAY_0 0X30 /*LEAST*/
#define MY_PROTOCOL_ADDRESS_ARRAY_1 0X30
#define MY_PROTOCOL_ADDRESS_ARRAY_2 0X30
#define MY_PROTOCOL_ADDRESS_ARRAY_3 0X30
#define MY_PROTOCOL_ADDRESS_ARRAY_4 0X30 /*(MOST)*/
#else

#define MY_PROTOCOL_ADDRESS ((unsigned long long)0X2020202020)/*MY ADDRESS PROTOCOL*/ /*LET IN THIS VERSION ALL ADDRESSES ARE 5 BYTES*/

#define MY_PROTOCOL_ADDRESS_ARRAY_0 0x20/*(LEAST)*/
#define MY_PROTOCOL_ADDRESS_ARRAY_1 0x20
#define MY_PROTOCOL_ADDRESS_ARRAY_2 0x20
#define MY_PROTOCOL_ADDRESS_ARRAY_3 0x20
#define MY_PROTOCOL_ADDRESS_ARRAY_4 0x20/*(MOST)*/


#endif





#define MY_PROTOCOL_PASSWORD_ARRAY_0 0X47
#define MY_PROTOCOL_PASSWORD_ARRAY_1 0X47
#define MY_PROTOCOL_PASSWORD_ARRAY_2 0X47
#define MY_PROTOCOL_PASSWORD_ARRAY_3 0X47

#define PROTOCOL_CURRENT_VERSION 1


/****************/









#define MAX_COUNT_WAIT 0XFFFFF
#define CHALLANGE_RESULT_SUCCESS 0X44
#define CHALLANGE_RESULT_FAILED 0X55










typedef enum{
	NOTCONNECTED_STATE,
	CONNECTED_STATE,
	
	
	
}SENSOR_STATE_DT;






typedef enum{
				MASTER_REQUEST_CONNECT_CODE=0X01,
				SENSOR_CHALLENGE_CODE=0X02,
				CHALLENGE_ANSWER_CODE=0X03,
				CHALLENGE_RESULT_CODE=0X04,
				BEACON_CODE=0X05,
				BEACON_RECEIVED_CODE=0X06,
				NEW_SESSION_KEY_CODE=0X07,
				NEW_SESSION_KEY_APPLIED_CODE=0X08,
				MASTER_CHANGE_CHANNEL_CODE=0X09,
				CHANNEL_CHANGED_CODE=0X0A,
				MASTER_CHANGE_DATA_CODE=0X10,
				DATA_CHANGED_CODE=0X11,
				MASTER_NEEDS_DATA_CODE=0X12,
				DATA_READ_CODE=0X13,
			}PACKET_TYPE_DT;

#endif /* PROTOCOL_DEFINE_H_ */