/*
 * EXTERN_FOR_BLUETOOTH.h
 *
 * Created: 30-Mar-21 10:52:53 PM
 *  Author: Mina Medhat
 */ 


#ifndef EXTERN_FOR_BLUETOOTH_H_
#define EXTERN_FOR_BLUETOOTH_H_

extern volatile unsigned char F_B_NEW_USER_PASS;
extern volatile unsigned char F_B_NEW_IOT_PAR;

extern volatile unsigned char F_B_USER_NAME[15];
extern volatile unsigned char F_B_PASSWORD[15];
/*IOT*/
extern volatile unsigned char F_B_CH_ID[10];
extern volatile unsigned char F_B_WRITE_KEY[25];
extern volatile unsigned char F_B_FIELD_NUM;
extern volatile unsigned char F_B_CH_ID_LENGTH;
extern volatile unsigned char F_B_WRITE_KEY_LENGTH;




/*SENSORS*/
extern volatile unsigned char F_B_ADD_NEW_SENSOR;
extern volatile unsigned char F_B_DELETE_SENSOR;
extern volatile unsigned char F_B_ENABLE_SENSOR;
extern volatile unsigned char F_B_DISABLE_SENSOR;

extern volatile SENSOR_DT B_NEW_SENSOR_BUFFER;
extern volatile unsigned char B_SENSOR_ADDRESS_DELETE[5];
extern volatile unsigned char B_SENSOR_ADDRESS_L_DELETE;
extern volatile unsigned char B_SENSOR_ADDRESS_ENABLE[5];
extern volatile unsigned char B_SENSOR_ADDRESS_L_ENABLE;
extern volatile unsigned char B_SENSOR_ADDRESS_DISABLE[5];
extern volatile unsigned char B_SENSOR_ADDRESS_L_DISABLE;

extern SENSOR_DT MY_SENSORS[NUMBER_OF_SENSORS_IN_DATA_BASE];
extern volatile unsigned char NUMBER_OF_SENSORS;



/*MAIN*/
extern volatile unsigned char F_B_RESET_SENSORS;
extern volatile unsigned char F_B_RESET_ALL;

/*LED GRID*/

extern volatile unsigned char F_B_NEW_LED_GRID;
extern volatile unsigned char LED_GRID_ARRAY[8];

/***********/
#endif /* EXTERN_FOR_BLUETOOTH_H_ */