/*
 * SENSOR_MANAGE.c
 *
 * Created: 31-Mar-21 5:41:21 PM
 *  Author: Mina Medhat
 */ 
#include "SENSOR_MANAGE.h"
#include "EXINTEREPUT128A.h"
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "semphr.h"

 volatile unsigned char F_B_RESET_SENSORS;
 volatile unsigned char F_B_RESET_ALL;

volatile SENSOR_DT MY_SENSORS[NUMBER_OF_SENSORS_IN_DATA_BASE];
NRF_RF_CHANNEL_DT MY_CHANNELS[]={NRF_USED_CH_1,NRF_USED_CH_2,NRF_USED_CH_3,NRF_USED_CH_4,NRF_USED_CH_5,NRF_USED_CH_6,NRF_USED_CH_7,NRF_USED_CH_8};
volatile unsigned char NUMBER_OF_SENSORS=0;
volatile unsigned char DELETE_INT_FLAG=0;
volatile unsigned char DELETE_COUNTER=0;
volatile unsigned char DELETE_FLAG=0;
volatile unsigned char DELETE_COUNTER_FLAG=0;;

volatile unsigned char FIRST_READ_FLAG=1;/*FLAG THAT SHOWS THAT THIS IS THE FIRST READ FROM THE SERVER*/



/*BUFFERS OF EXCHANGE DATA SENSORS */
volatile SENSOR_SEND_BUFFER_DT SENSOR_SEND_BUFFER[NUMBER_OF_SENSORS_IN_DATA_BASE];
volatile unsigned char SENSOR_SEND_BUFFER_LENGTH=0;

volatile SENSOR_READ_BUFFER_DT SENSOR_READ_BUFFER[NUMBER_OF_SENSORS_IN_DATA_BASE];
volatile unsigned char SENSOR_READ_BUFFER_LENGTH=0;

volatile unsigned long OLD_LAST_ENTRY;
extern volatile unsigned long NEW_LAST_ENTRY; 

extern volatile unsigned char F_LAST_RECIVED_DATA_NOT_HANDLED;	

extern ESP_RECIVED_DATA_DT G_RECIVED_DATA[5];
extern volatile QUEUE_ARRAY_UINT8 G_SEND_DATA_QUEUE;

/***************************************/




volatile unsigned char F_B_ADD_NEW_SENSOR;
volatile unsigned char F_B_DELETE_SENSOR;
volatile unsigned char F_B_ENABLE_SENSOR;
volatile unsigned char F_B_DISABLE_SENSOR;

volatile SENSOR_DT B_NEW_SENSOR_BUFFER;
volatile unsigned char B_SENSOR_ADDRESS_DELETE[5];
volatile unsigned char B_SENSOR_ADDRESS_L_DELETE;
volatile unsigned char B_SENSOR_ADDRESS_ENABLE[5];
volatile unsigned char B_SENSOR_ADDRESS_L_ENABLE;
volatile unsigned char B_SENSOR_ADDRESS_DISABLE[5];
volatile unsigned char B_SENSOR_ADDRESS_L_DISABLE;




//os
SemaphoreHandle_t QUEUE_SEMAPHORE;



void SENSOR_MANAGE_TASK(void * pd)
{
	QUEUE_SEMAPHORE=xSemaphoreCreateBinary();
	xSemaphoreGive(QUEUE_SEMAPHORE);
	PROTOCOL_SETUP();
	external_int_enable(INT6_ENABLE ,INT_FALLING);
	LOAD_OLD_LAST_ENTRY();
	SENSORS_LOAD_EEPROM();
	QUEUE_INTILIZE_ARRAY_U8(&G_SEND_DATA_QUEUE);

	
	while(1)
	{
		TEST_ADD_SENSOR();
		 LOOP_SENSORS_MANAGE();
	}
}






void LOOP_SENSORS_MANAGE()
{
	if (F_B_ADD_NEW_SENSOR)
	{
		F_B_ADD_NEW_SENSOR=0;
		ADD_NEW_SENSOR(B_NEW_SENSOR_BUFFER);
	}
	if (F_B_DELETE_SENSOR)
	{
		F_B_DELETE_SENSOR=0;
		DELETE_SENSOR(B_SENSOR_ADDRESS_DELETE,5/*B_SENSOR_ADDRESS_L_DELETE*/);
	}
	if (F_B_ENABLE_SENSOR)
	{
		F_B_ENABLE_SENSOR=0;
		SENSOR_STATE_ENABLE(B_SENSOR_ADDRESS_ENABLE,5/*B_SENSOR_ADDRESS_L_ENABLE*/);
	}
	if (F_B_DISABLE_SENSOR)
	{
		F_B_DISABLE_SENSOR=0;
		SENSOR_STATE_DISABLE(B_SENSOR_ADDRESS_DISABLE,5/*B_SENSOR_ADDRESS_L_DISABLE*/);
	}
	if (DELETE_FLAG)
	{
		DELETE_FLAG=0;
		TEST_UART_SEND_str("delete all\n");
		DELETE_ALL_SENSOR();/*TO BE CONTINUED*/
	}
	
	TEST2_SET();
	SENSORS_CONNECT();
	SENSORS_BEACON();
	SENSORS_NEW_SESSIONS();
	SENSORS_CHANGE_CHANNEL();
	SENSORS_FILL_SENSOR_BUFFERS();
	SENSORS_APPLY_SEND_BUFFER();
	SENSORS_APPLY_READ_BUFFER();
	SENSORS_RESET_LOOP();
	RESET_ALL_LOOP();
	toggybit(*PORT_OUT_TOGGLE_LED,TOGGLE_LED_PIN);
	TEST2_CLEAR();
	vTaskDelay(pdMS_TO_TICKS(10));
}

void SENSORS_RESET_LOOP()
{
	if (F_B_RESET_SENSORS)
	{
		F_B_RESET_SENSORS=0;
		DELETE_ALL_SENSOR();
	}
}


void RESET_ALL_LOOP()
{
	if (F_B_RESET_ALL)
	{
		F_B_RESET_ALL=0;
		DELETE_ALL_SENSOR();
		WIFI_RESET();
	}
}

void SENSORS_CONNECT()
{
	/*TEST_UART_SEND_str("CON\n");*/
	unsigned char i;
	for (i=0;i<NUMBER_OF_SENSORS_IN_DATA_BASE;i++)
	{
		if (MY_SENSORS[i].SENSOR_OCCUBIED==1)
		{
			if (MY_SENSORS[i].SENSOR_STATE==1)
			{
				if (MY_SENSORS[i].F_CONNECTED!=1)
				{
					if (MY_SENSORS[i].C_T_TRY_CONNECTED==MAX_TIMER_CONNECT_COUNT_SENSOR)
					{
						global_interupt_disable();
						MY_SENSORS[i].C_T_TRY_CONNECTED=0;
						global_interupt_enable();
						NRF_UPDATE_USED_CHANNEL(MY_CHANNELS[CONTROL_CHANNEL_INDEX]);
						/*TESTING*/
						TEST_UART_SEND_str("try CONNECT\n");
						TEST_UART_SEND_str("ADDRESS: ");
						TEST_UART_SEND_VALUE(MY_SENSORS[i].SENSOR_ADDRESS[0]);
						TEST_UART_SEND_byte(' ');
						TEST_UART_SEND_VALUE(MY_SENSORS[i].SENSOR_ADDRESS[1]);
						TEST_UART_SEND_byte(' ');
						TEST_UART_SEND_VALUE(MY_SENSORS[i].SENSOR_ADDRESS[2]);
						TEST_UART_SEND_byte(' ');
						TEST_UART_SEND_VALUE(MY_SENSORS[i].SENSOR_ADDRESS[3]);
						TEST_UART_SEND_byte(' ');
						TEST_UART_SEND_VALUE(MY_SENSORS[i].SENSOR_ADDRESS[4]);
						TEST_UART_SEND_str("\n");
						TEST_UART_SEND_str("PASS: ");
						TEST_UART_SEND_VALUE(MY_SENSORS[i].SENSOR_PASSWORD[0]);
						TEST_UART_SEND_byte(' ');
						TEST_UART_SEND_VALUE(MY_SENSORS[i].SENSOR_PASSWORD[1]);
						TEST_UART_SEND_byte(' ');
						TEST_UART_SEND_VALUE(MY_SENSORS[i].SENSOR_PASSWORD[2]);
						TEST_UART_SEND_byte(' ');
						TEST_UART_SEND_VALUE(MY_SENSORS[i].SENSOR_PASSWORD[3]);
						TEST_UART_SEND_str("\nNUMBER: ");
						TEST_UART_SEND_VALUE(i);
						TEST_UART_SEND_byte('\n');
						/*TESTING*/
						if (PROTOCOL_MASTER_CONNECT_TO_SENSOR(MY_SENSORS[i].SENSOR_ADDRESS,MY_SENSORS[i].SENSOR_VERSION,MY_SENSORS[i].SENSOR_ADDRESS_LENGTH,MY_SENSORS[i].SENSOR_PASSWORD))
						{
							/*TESTING*/
							TEST_UART_SEND_str("CONNECTED ");
							TEST_UART_SEND_VALUE(i);
							TEST_UART_SEND_byte('\n');
							/*TESTING*/
							MY_SENSORS[i].READ_FAIL_COUNTER=0;
							MY_SENSORS[i].SEND_FAIL_COUNTER=0;
							MY_SENSORS[i].F_CONNECTED=1;
							MY_SENSORS[i].USED_CHANNEL=CONTROL_CHANNEL_INDEX;
							MY_SENSORS[i].C_T_BEACON=0;
							MY_SENSORS[i].F_HAVE_SESSION_KEY=0;
							MY_SENSORS[i].C_FAILED_BEACON=0;
							MY_SENSORS[i].C_T_SESSION_KEY=0;
							
						}
					}
				}
			}
		}
	}
}



void SENSORS_BEACON()
{

	unsigned char i;
	for (i=0;i<NUMBER_OF_SENSORS_IN_DATA_BASE;i++)
	{
		if (MY_SENSORS[i].SENSOR_OCCUBIED==1)
		{
			if (MY_SENSORS[i].SENSOR_STATE==1)
			{
				if (MY_SENSORS[i].F_CONNECTED==1)
				{
					if (MY_SENSORS[i].C_T_BEACON==MAX_TIMER_BEACON_COUNTER)
					{
							global_interupt_disable();
							MY_SENSORS[i].C_T_BEACON=0;
							global_interupt_enable();
						if (MY_SENSORS[i].F_HAVE_SESSION_KEY==1)
						{
							NRF_UPDATE_USED_CHANNEL(MY_CHANNELS[MY_SENSORS[i].USED_CHANNEL]);
							/*TESTING*/
							TEST_UART_SEND_str("try BEACON: ");
							TEST_UART_SEND_VALUE(i);
							TEST_UART_SEND_byte('\n');
							/*TESTING*/
							if (PROTOCOL_SEND_BEACON(MY_SENSORS[i].SENSOR_ADDRESS,MY_SENSORS[i].SENSOR_VERSION,MY_SENSORS[i].SENSOR_ADDRESS_LENGTH,MY_SENSORS[i].SESSION_KEY))
							{
								/*TESTING*/
								TEST_UART_SEND_str("BEACON: ");
								TEST_UART_SEND_VALUE(i);
								TEST_UART_SEND_byte('\n');
								/*TESTING*/
								MY_SENSORS[i].C_FAILED_BEACON=0;
							}
							else
							{
								MY_SENSORS[i].C_FAILED_BEACON++;
							}
						}
						else
						{
							MY_SENSORS[i].C_FAILED_BEACON++;
						}
						
						
						if (MY_SENSORS[i].C_FAILED_BEACON==MAX_FAILED_BEACON_COUNTER)
						{
							/*SENSOR DISCONNECT*/
							MY_SENSORS[i].C_FAILED_BEACON=0;
							MY_SENSORS[i].F_CONNECTED=0;
							MY_SENSORS[i].USED_CHANNEL=0;
							MY_SENSORS[i].C_T_BEACON=0;
							MY_SENSORS[i].C_T_TRY_CONNECTED=0;
							MY_SENSORS[i].C_T_SESSION_KEY=0;
							MY_SENSORS[i].F_HAVE_SESSION_KEY=0;
							/*TESTING*/
							TEST_UART_SEND_str("SENSOR DISCONNECTED ");
							TEST_UART_SEND_VALUE(i);
							TEST_UART_SEND_byte('\n');
							/*TESTING*/
						}
					}
				}
			}
		}
	}
}
void SENSORS_NEW_SESSIONS()
{
	
	unsigned char i;
	for (i=0;i<NUMBER_OF_SENSORS_IN_DATA_BASE;i++)
	{
		if (MY_SENSORS[i].SENSOR_OCCUBIED==1)
		{
			if (MY_SENSORS[i].SENSOR_STATE==1)
			{
				if (MY_SENSORS[i].F_CONNECTED==1)
				{
					if ((MY_SENSORS[i].F_HAVE_SESSION_KEY==0)||(MY_SENSORS[i].C_T_SESSION_KEY==MAX_SESSION_KEY_TIMER_COUNT))
					{
						NRF_UPDATE_USED_CHANNEL(MY_CHANNELS[MY_SENSORS[i].USED_CHANNEL]);
						volatile unsigned short SESSION_KEY_BUFFER;
						volatile unsigned char SESSION_KEY_ARRAY[2];
						SESSION_KEY_BUFFER=GENNERATE_RANDOM_NUMBER();
						SESSION_KEY_ARRAY[0]=SESSION_KEY_BUFFER>>8;
						SESSION_KEY_ARRAY[1]=SESSION_KEY_BUFFER&0X00FF;
						/*TESTING*/
						TEST_UART_SEND_str("try new session: ");
						TEST_UART_SEND_VALUE(i);
						TEST_UART_SEND_byte('\n');
						/*TESTING*/
						
						if (PROTOCOL_SEND_SESSION_KEY(MY_SENSORS[i].SENSOR_ADDRESS,MY_SENSORS[i].SENSOR_VERSION,MY_SENSORS[i].SENSOR_ADDRESS_LENGTH,SESSION_KEY_ARRAY,MY_SENSORS[i].SENSOR_PASSWORD))
						{
							
						
							
							MY_SENSORS[i].F_HAVE_SESSION_KEY=1;
							MY_SENSORS[i].C_T_SESSION_KEY=0;
							MY_SENSORS[i].SESSION_KEY[0]=SESSION_KEY_ARRAY[0];
							MY_SENSORS[i].SESSION_KEY[1]=SESSION_KEY_ARRAY[1];
							MY_SENSORS[i].C_FAILED_BEACON=0;
							/*TESTING*/
							TEST_UART_SEND_str("SESSION: ");
							TEST_UART_SEND_VALUE((unsigned short)((MY_SENSORS[i].SESSION_KEY[0]<<8)|MY_SENSORS[i].SESSION_KEY[1]));
							TEST_UART_SEND_str(" NUM: ");
							TEST_UART_SEND_VALUE(i);
							TEST_UART_SEND_byte('\n');
							/*TESTING*/
							
						}	
					}
				}
			}
		}
	}
					
					
					
					
}

void SENSORS_CHANGE_CHANNEL()
{
	
	unsigned char i;
	for (i=0;i<NUMBER_OF_SENSORS_IN_DATA_BASE;i++)
	{
		if (MY_SENSORS[i].SENSOR_OCCUBIED==1)
		{
			if (MY_SENSORS[i].SENSOR_STATE==1)
			{
				if (MY_SENSORS[i].F_CONNECTED==1)
				{
					if ((MY_SENSORS[i].F_HAVE_SESSION_KEY==1))
					{
						if ((MY_SENSORS[i].USED_CHANNEL==0))/*CONTROL CHANNEL*/
						{
							NRF_UPDATE_USED_CHANNEL(MY_CHANNELS[0]);
							unsigned short new_ch= GENNERATE_RANDOM_NUMBER();
							new_ch=(new_ch%7)+1;/*random from 1 to 7*/
							/*TESTING*/
							TEST_UART_SEND_str("try channel: ");
							TEST_UART_SEND_VALUE(i);
							TEST_UART_SEND_byte('\n');
							/*TESTING*/
							if (PROTOCOL_CHANGE_CHANNEL(MY_SENSORS[i].SENSOR_ADDRESS,MY_SENSORS[i].SENSOR_VERSION,MY_SENSORS[i].SENSOR_ADDRESS_LENGTH,MY_SENSORS[i].SESSION_KEY,(unsigned char)new_ch))
							{
								
								MY_SENSORS[i].USED_CHANNEL=(unsigned char)new_ch;
								MY_SENSORS[i].C_FAILED_BEACON=0;
								
								/*TESTING*/
								TEST_UART_SEND_str("CHANNEL: ");
								TEST_UART_SEND_VALUE(new_ch);
								TEST_UART_SEND_str(" NUM: ");
								TEST_UART_SEND_VALUE(i);
								TEST_UART_SEND_byte('\n');
								/*TESTING*/
								
							
							}	
						}	
					}
				}
			}
		}
	}
}

void SENSORS_FILL_SENSOR_BUFFERS()
{
	
	if(F_LAST_RECIVED_DATA_NOT_HANDLED)   /*MAKE SENSOR WORK ON IT*/
	{
		
		if (FIRST_READ_FLAG)
		{
			OLD_LAST_ENTRY=NEW_LAST_ENTRY;
			FIRST_READ_FLAG=0;
		}
		
		if (OLD_LAST_ENTRY<NEW_LAST_ENTRY)
		{
			unsigned long TEMP_DIFF=NEW_LAST_ENTRY-OLD_LAST_ENTRY;
			OLD_LAST_ENTRY=NEW_LAST_ENTRY;
			SAVE_OLD_LAST_ENTRY();
			if(TEMP_DIFF>5)
			{
				TEMP_DIFF=5;
			}
			unsigned char i=0,k=0,J=0;
			unsigned char NUMBER_STRING_BUFFER[20];
			unsigned long long NUMBER_BUFFER=0;
			/*M:ADDRESS SENSOR(decimal):COMMAND(W or R):DATA1:DATA2:*/
			/*M:ADDRESS SENSOR:COMMAND:DATA1:*/
			for (i=5-TEMP_DIFF;i<5;++i)
			{	
				if (G_RECIVED_DATA[i].DATA_ELEMENT[0]=='M')/*MEANS THAT ANDROID SEND THIS TO MASTER*/
				{
					if (G_RECIVED_DATA[i].DATA_ELEMENT[1]==':')/*COTATION*/
					{
						k=0;
						/*ADDRESS SENSOR*/
						while(G_RECIVED_DATA[i].DATA_ELEMENT[2+k]!=':')/*COTATION*/
						{
							NUMBER_STRING_BUFFER[k]=G_RECIVED_DATA[i].DATA_ELEMENT[2+k];
							k++;
							
						}
						NUMBER_BUFFER=STRINGTOINT(NUMBER_STRING_BUFFER,k);
						
							unsigned char ARRAY_BUFFER[5];
							ARRAY_BUFFER[0]=NUMBER_BUFFER&0XFF;
							ARRAY_BUFFER[1]=(NUMBER_BUFFER>>8)&0XFF;
							ARRAY_BUFFER[2]=(NUMBER_BUFFER>>16)&0XFF;
							ARRAY_BUFFER[3]=(NUMBER_BUFFER>>24)&0XFF;
							ARRAY_BUFFER[4]=(NUMBER_BUFFER>>32)&0XFF;
						char SENSOR_INDEX_BUFFER=FIND_SENSOR(ARRAY_BUFFER,5);
						if (SENSOR_INDEX_BUFFER!=(char)(-1))
						{
							
					
						
						
						if(G_RECIVED_DATA[i].DATA_ELEMENT[3+k]=='W')/*MEANS ANDROID NEED TO CHANGE DATA*/
						{
							if (G_RECIVED_DATA[i].DATA_ELEMENT[4+k]==':')/*COTATION*/
							{
								k=5+k;
								
								
								SENSOR_SEND_BUFFER[SENSOR_SEND_BUFFER_LENGTH].SENSOR_INDEX=SENSOR_INDEX_BUFFER;
								
									J=0;
									/*ADDRESS REGISTER*/
									while(G_RECIVED_DATA[i].DATA_ELEMENT[k+J]!=':')/*COTATION*/
									{
										NUMBER_STRING_BUFFER[J]=G_RECIVED_DATA[i].DATA_ELEMENT[J+k];
										J++;
										
									}
									NUMBER_BUFFER=STRINGTOINT(NUMBER_STRING_BUFFER,J);
									SENSOR_SEND_BUFFER[SENSOR_SEND_BUFFER_LENGTH].ADDRESS_REGISTER[0]=NUMBER_BUFFER>>8;
									SENSOR_SEND_BUFFER[SENSOR_SEND_BUFFER_LENGTH].ADDRESS_REGISTER[1]=NUMBER_BUFFER;
									k=J+k+1;
									J=0;
									/*DATA VALUE*/
									while(G_RECIVED_DATA[i].DATA_ELEMENT[k+J]!=':')
									{
										NUMBER_STRING_BUFFER[J]=G_RECIVED_DATA[i].DATA_ELEMENT[J+k];
										J++;
										
									}
									NUMBER_BUFFER=STRINGTOINT(NUMBER_STRING_BUFFER,J);
									SENSOR_SEND_BUFFER[SENSOR_SEND_BUFFER_LENGTH].NEW_VALUE[0]=NUMBER_BUFFER>>8;
									SENSOR_SEND_BUFFER[SENSOR_SEND_BUFFER_LENGTH].NEW_VALUE[1]=NUMBER_BUFFER;
									
									SENSOR_SEND_BUFFER_LENGTH++;
									/*TEST_UART_SEND_str("BUFFER ADDED\n");*/
								
								
								
							}
						}
						else
						{
							if(G_RECIVED_DATA[i].DATA_ELEMENT[3+k]=='R')/*MEANS ANDROID NEED TO READ DATA*/
							{
								
									if (G_RECIVED_DATA[i].DATA_ELEMENT[4+k]==':')
									{
										k=5+k;
										SENSOR_READ_BUFFER[SENSOR_READ_BUFFER_LENGTH].SENSOR_INDEX=SENSOR_INDEX_BUFFER;
										J=0;
										/*ADDRESS REGISTER*/
										while(G_RECIVED_DATA[i].DATA_ELEMENT[k+J]!=':')
										{
											NUMBER_STRING_BUFFER[J]=G_RECIVED_DATA[i].DATA_ELEMENT[J+k];
											J++;
										
										}
										NUMBER_BUFFER=STRINGTOINT(NUMBER_STRING_BUFFER,J);
										SENSOR_READ_BUFFER[SENSOR_READ_BUFFER_LENGTH].ADDRESS_REGISTER[0]=NUMBER_BUFFER>>8;
										SENSOR_READ_BUFFER[SENSOR_READ_BUFFER_LENGTH].ADDRESS_REGISTER[1]=NUMBER_BUFFER;
									
										SENSOR_READ_BUFFER_LENGTH++;
									
									
									}
								}
							}
						}			
					}	
				}	
			}
		}	
		else
		{
			F_LAST_RECIVED_DATA_NOT_HANDLED=0;
		}
		
		
		
	}
}

void SENSORS_APPLY_SEND_BUFFER()
{
	if (SENSOR_SEND_BUFFER_LENGTH!=0)
	{	
		volatile unsigned char DATA_SEND_BUFFER[30],LENGTH;
		volatile unsigned char i=0,BUFFER_INDEX,RESULT_STATE=0;
		volatile unsigned long long NUMBER_BUFFER=0;
		volatile unsigned char NUMBER_STRING_BUFFER[20];
		
		for(i=0;i<SENSOR_SEND_BUFFER_LENGTH;i++)
		{
			BUFFER_INDEX=SENSOR_SEND_BUFFER[i].SENSOR_INDEX;
			if (MY_SENSORS[BUFFER_INDEX].SENSOR_STATE)
			{
				if (MY_SENSORS[BUFFER_INDEX].F_CONNECTED)
				{
					if (MY_SENSORS[BUFFER_INDEX].F_HAVE_SESSION_KEY)
					{
						/*TEST*/
						volatile unsigned short test_address =(SENSOR_SEND_BUFFER[i].ADDRESS_REGISTER[0]<<8)|SENSOR_SEND_BUFFER[i].ADDRESS_REGISTER[1];
						volatile unsigned short test_data =(SENSOR_SEND_BUFFER[i].NEW_VALUE[0]<<8)|SENSOR_SEND_BUFFER[i].NEW_VALUE[1];
						
						TEST_UART_SEND_str("DATA SENDing address:");
						TEST_UART_SEND_VALUE(test_address);
						TEST_UART_SEND_str(" DATA SENDing value:");
						TEST_UART_SEND_VALUE(test_data);
						TEST_UART_SEND_str("\n");
						
						
						/**************/
						
						
						NRF_UPDATE_USED_CHANNEL(MY_CHANNELS[MY_SENSORS[BUFFER_INDEX].USED_CHANNEL]);
						if (PROTOCOL_DATA_SEND(MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS,MY_SENSORS[BUFFER_INDEX].SENSOR_VERSION,MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS_LENGTH,MY_SENSORS[BUFFER_INDEX].SESSION_KEY,SENSOR_SEND_BUFFER[i].ADDRESS_REGISTER,SENSOR_SEND_BUFFER[i].NEW_VALUE))
						{
							/*TESTING*/
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("DATA SEND DONE ");
							TEST_UART_SEND_VALUE(BUFFER_INDEX);
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							/*TESTING*/
							
							
							MY_SENSORS[BUFFER_INDEX].C_FAILED_BEACON=0;
							MY_SENSORS[BUFFER_INDEX].SEND_FAIL_COUNTER=0;
							
							RESULT_STATE=1;
							
							
							DATA_SEND_BUFFER[0]='A';/*MEANS THE MASTER SEND TO ANDROID*/
							DATA_SEND_BUFFER[1]=':';/*COTATION*/
							
							
							/*
							LEAST ADDRESS[0]
								  ADDRESS[1]
								  ADDRESS[2]
								  ADDRESS[3]
							MOST  ADDRESS[4]
							*/
							volatile char count;
							for (count=(MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS_LENGTH);count>0;count--)
							{
								NUMBER_BUFFER=(NUMBER_BUFFER<<8)|MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS[count-1];
							}
							
							
							
							unsigned char ADDRESS_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
							/*SENSOR ADDRESS*/
							for(count=0;count<ADDRESS_LENGTH_DECIMAL;count++)
							{
								DATA_SEND_BUFFER[2+(unsigned char)count]=NUMBER_STRING_BUFFER[(unsigned char)count];
							}
							LENGTH=2+count;
							DATA_SEND_BUFFER[LENGTH]=':';
							DATA_SEND_BUFFER[LENGTH+1]='D';/*MASTER RETURN WRITE DONE TO ANDROID*/
							DATA_SEND_BUFFER[LENGTH+2]=':';
							LENGTH=LENGTH+3;
							
							NUMBER_BUFFER=0;
							for (count=0;count<2;count++)
							{
								NUMBER_BUFFER=(NUMBER_BUFFER<<8)|SENSOR_SEND_BUFFER[i].ADDRESS_REGISTER[(unsigned char)count];
							}
							
							unsigned char REG_ADDRESS_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
							/*ADDRESS REGISTER*/
							for(count=0;count<REG_ADDRESS_LENGTH_DECIMAL;count++)
							{
								DATA_SEND_BUFFER[LENGTH+(unsigned char)count]=NUMBER_STRING_BUFFER[(unsigned char)count];
							}
							LENGTH=LENGTH+count;
							DATA_SEND_BUFFER[LENGTH]=':';
							LENGTH++;
							
							NUMBER_BUFFER=0;
							for (count=0;count<2;count++)
							{
								NUMBER_BUFFER=(NUMBER_BUFFER<<8)|SENSOR_SEND_BUFFER[i].NEW_VALUE[(unsigned char)count];
							}
							
							unsigned char DATA_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
							/*DATA*/
							for(count=0;count<DATA_LENGTH_DECIMAL;count++)
							{
								DATA_SEND_BUFFER[LENGTH+count]=NUMBER_STRING_BUFFER[(unsigned char)count];
							}
							LENGTH=LENGTH+count;
							DATA_SEND_BUFFER[LENGTH]=':';
							DATA_SEND_BUFFER[LENGTH+1]=0;
							xSemaphoreTake(QUEUE_SEMAPHORE,portMAX_DELAY);
							QUEUE_ADD_ARRAY_U8(&G_SEND_DATA_QUEUE,DATA_SEND_BUFFER,LENGTH+2);
							xSemaphoreGive(QUEUE_SEMAPHORE);
							/*SEND DONE*/
						}
					}
				}
			}
			if(!RESULT_STATE)
			{
				MY_SENSORS[BUFFER_INDEX].SEND_FAIL_COUNTER++;
				if(MY_SENSORS[BUFFER_INDEX].SEND_FAIL_COUNTER==MAX_SEND_FAIL_COUNTER)
				{
					
					MY_SENSORS[BUFFER_INDEX].SEND_FAIL_COUNTER=0;
					/*TESTING*/
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("DATA NOT SEND ");
					TEST_UART_SEND_VALUE(BUFFER_INDEX);
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					/*TESTING*/
				
					DATA_SEND_BUFFER[0]='A';/*MEANS THE MASTER SEND TO ANDROID*/
					DATA_SEND_BUFFER[1]=':';/*COTATION*/
					NUMBER_BUFFER=0;
				
					/*
								LEAST ADDRESS[0]
									  ADDRESS[1]
									  ADDRESS[2]
									  ADDRESS[3]
								MOST  ADDRESS[4]
								*/
					char count;
					for (count=(MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS_LENGTH);count>0;count--)
					{
						NUMBER_BUFFER=(NUMBER_BUFFER<<8)|MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS[count-1];
					}
				
				
				
					unsigned char ADDRESS_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
					/*SENSOR ADDRESS*/
					for(count=0;count<ADDRESS_LENGTH_DECIMAL;count++)
					{
						DATA_SEND_BUFFER[2+(unsigned char)count]=NUMBER_STRING_BUFFER[(unsigned char)count];
					}
					LENGTH=2+count;
					DATA_SEND_BUFFER[LENGTH]=':';
					DATA_SEND_BUFFER[LENGTH+1]='T';/*MASTER RETURN WRITE FAILED TO ANDROID*/
					DATA_SEND_BUFFER[LENGTH+2]=':';
					LENGTH=LENGTH+3;
				
					NUMBER_BUFFER=0;
					for (count=0;count<2;count++)
					{
						NUMBER_BUFFER=(NUMBER_BUFFER<<8)|SENSOR_SEND_BUFFER[i].ADDRESS_REGISTER[(unsigned char)count];
					}
				
					unsigned char REG_ADDRESS_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
					/*ADDRESS REGISTER*/
					for(count=0;count<REG_ADDRESS_LENGTH_DECIMAL;count++)
					{
						DATA_SEND_BUFFER[LENGTH+(unsigned char)count]=NUMBER_STRING_BUFFER[(unsigned char)count];
					}
					LENGTH=LENGTH+count;
					DATA_SEND_BUFFER[LENGTH]=':';
					LENGTH++;
					NUMBER_BUFFER=0;
					for (count=0;count<2;count++)
					{
						NUMBER_BUFFER=(NUMBER_BUFFER<<8)|SENSOR_SEND_BUFFER[i].NEW_VALUE[(unsigned char)count];
					}
				
					unsigned char DATA_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
					/*DATA*/
					for(count=0;count<DATA_LENGTH_DECIMAL;count++)
					{
						DATA_SEND_BUFFER[LENGTH+(unsigned char)count]=NUMBER_STRING_BUFFER[(unsigned char)count];
					}
					LENGTH=LENGTH+count;
					DATA_SEND_BUFFER[LENGTH]=':';
					DATA_SEND_BUFFER[LENGTH+1]=0;
					xSemaphoreTake(QUEUE_SEMAPHORE,portMAX_DELAY);
					QUEUE_ADD_ARRAY_U8(&G_SEND_DATA_QUEUE,DATA_SEND_BUFFER,LENGTH+2);
					xSemaphoreGive(QUEUE_SEMAPHORE);
					/*SEND FAILED*/
				}
				
			}
			
			
		}
		SENSOR_SEND_BUFFER_LENGTH=0;
	}
}

void SENSORS_APPLY_READ_BUFFER()
{
	if (SENSOR_READ_BUFFER_LENGTH!=0)
	{
		unsigned char DATA_SEND_BUFFER[30],LENGTH;
		unsigned char i=0,BUFFER_INDEX,RESULT_STATE=0;
		unsigned char NUMBER_STRING_BUFFER[20];
		unsigned long long NUMBER_BUFFER=0;
		
		for(i=0;i<SENSOR_READ_BUFFER_LENGTH;i++)
		{
			BUFFER_INDEX=SENSOR_READ_BUFFER[i].SENSOR_INDEX;
			if (MY_SENSORS[BUFFER_INDEX].SENSOR_STATE)
			{
				if (MY_SENSORS[BUFFER_INDEX].F_CONNECTED)
				{
					if (MY_SENSORS[BUFFER_INDEX].F_HAVE_SESSION_KEY)
					{
						
						NRF_UPDATE_USED_CHANNEL(MY_CHANNELS[MY_SENSORS[BUFFER_INDEX].USED_CHANNEL]);
						if (PROTOCOL_DATA_READ(MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS,MY_SENSORS[BUFFER_INDEX].SENSOR_VERSION,MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS_LENGTH,MY_SENSORS[BUFFER_INDEX].SESSION_KEY,SENSOR_READ_BUFFER[i].ADDRESS_REGISTER,SENSOR_READ_BUFFER[i].READ_VALUE))
						{
							RESULT_STATE=1;
							MY_SENSORS[BUFFER_INDEX].READ_FAIL_COUNTER=0;
							/*TESTING*/
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("DATA READ");
							TEST_UART_SEND_VALUE(BUFFER_INDEX);
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							TEST_UART_SEND_str("\n");
							/*TESTING*/
							
							MY_SENSORS[BUFFER_INDEX].C_FAILED_BEACON=0;
							
							DATA_SEND_BUFFER[0]='A';/*MEANS THE MASTER SEND TO ANDROID*/
							DATA_SEND_BUFFER[1]=':';/*COTATION*/
							
							
							/*
							LEAST ADDRESS[0]
								  ADDRESS[1]
								  ADDRESS[2]
								  ADDRESS[3]
							MOST  ADDRESS[4]
							*/
							
							char count;
							for (count=(MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS_LENGTH);count>0;count--)
							{
								NUMBER_BUFFER=(NUMBER_BUFFER<<8)|MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS[count-1];
							}
							
							
							/*SENSOR ADDRESS*/
							unsigned char ADDRESS_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
							
							for(count=0;count<ADDRESS_LENGTH_DECIMAL;count++)
							{
								DATA_SEND_BUFFER[(unsigned char)(2+(char)count)]=NUMBER_STRING_BUFFER[(unsigned char)(count)];
							}
							
							LENGTH=2+count;
							DATA_SEND_BUFFER[LENGTH]=':';
							DATA_SEND_BUFFER[LENGTH+1]='K';/*MASTER RETURN DATA TO ANDROID*/
							DATA_SEND_BUFFER[LENGTH+2]=':';
							LENGTH=LENGTH+3;
							NUMBER_BUFFER=0;
							/*ADDRESS REGISTER*/
							for (count=0;count<2;count++)
							{
								NUMBER_BUFFER=(NUMBER_BUFFER<<8)|SENSOR_READ_BUFFER[i].ADDRESS_REGISTER[(unsigned char)count];
							}
							
							unsigned char REG_ADDRESS_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
							for(count=0;count<REG_ADDRESS_LENGTH_DECIMAL;count++)
							{
								DATA_SEND_BUFFER[(unsigned char)(LENGTH+count)]=NUMBER_STRING_BUFFER[(unsigned char)count];
							}
							LENGTH=LENGTH+count;
							DATA_SEND_BUFFER[LENGTH]=':';
							LENGTH++;
							NUMBER_BUFFER=0;
							for (count=0;count<2;count++)
							{
								NUMBER_BUFFER=(NUMBER_BUFFER<<8)|SENSOR_READ_BUFFER[i].READ_VALUE[(unsigned char)count];
							}
							/*DATA*/
							unsigned char DATA_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
							for(count=0;count<DATA_LENGTH_DECIMAL;count++)
							{
								DATA_SEND_BUFFER[(unsigned char)(LENGTH+count)]=NUMBER_STRING_BUFFER[(unsigned char)count];
							}
							LENGTH=LENGTH+count;
							DATA_SEND_BUFFER[LENGTH]=':';
							DATA_SEND_BUFFER[LENGTH+1]=0;
							xSemaphoreTake(QUEUE_SEMAPHORE,portMAX_DELAY);
							QUEUE_ADD_ARRAY_U8(&G_SEND_DATA_QUEUE,DATA_SEND_BUFFER,LENGTH+2);
							xSemaphoreGive(QUEUE_SEMAPHORE);
							/*READ SUCCESS*/
						}
					}
				}
			}
			
			if(!RESULT_STATE)
			{
				MY_SENSORS[BUFFER_INDEX].READ_FAIL_COUNTER++;
				if(MY_SENSORS[BUFFER_INDEX].READ_FAIL_COUNTER==MAX_READ_FAIL_COUNTER)
				{
					MY_SENSORS[BUFFER_INDEX].READ_FAIL_COUNTER=0;
				/*TESTING*/
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("DATA NOT READ ");
					TEST_UART_SEND_VALUE(BUFFER_INDEX);
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
					TEST_UART_SEND_str("\n");
				/*TESTING*/
						DATA_SEND_BUFFER[0]='A';/*MEANS THE MASTER SEND TO ANDROID*/
						DATA_SEND_BUFFER[1]=':';/*COTATION*/
						
						
						
						
						
						/*
							LEAST ADDRESS[0]
								  ADDRESS[1]
								  ADDRESS[2]
								  ADDRESS[3]
							MOST  ADDRESS[4]
							*/
						NUMBER_BUFFER=0;
						char count;
						for (count=(MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS_LENGTH);count>0;count--)
						{
							NUMBER_BUFFER=(NUMBER_BUFFER<<8)|MY_SENSORS[BUFFER_INDEX].SENSOR_ADDRESS[count-1];
						}
							
						unsigned char ADDRESS_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
						/*SENSOR ADDRESS*/
						for(count=0;count<ADDRESS_LENGTH_DECIMAL;count++)
						{
							DATA_SEND_BUFFER[2+(unsigned char)count]=NUMBER_STRING_BUFFER[(unsigned char)count];
						}
						LENGTH=2+count;
						DATA_SEND_BUFFER[LENGTH]=':';
						DATA_SEND_BUFFER[LENGTH+1]='F';/*MASTER RETURN FAILED TO ANDROID*/
						DATA_SEND_BUFFER[LENGTH+2]=':';
						LENGTH=LENGTH+3;
							
							NUMBER_BUFFER=0;
						for (count=0;count<2;count++)
						{
							NUMBER_BUFFER=(NUMBER_BUFFER<<8)|SENSOR_READ_BUFFER[i].ADDRESS_REGISTER[(unsigned char)count];
						}
							
						unsigned char REG_ADDRESS_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
						/*ADDRESS REGISTER*/
						for(count=0;count<REG_ADDRESS_LENGTH_DECIMAL;count++)
						{
							DATA_SEND_BUFFER[LENGTH+(unsigned char)count]=NUMBER_STRING_BUFFER[(unsigned char)count];
						}
						LENGTH=LENGTH+count;
						DATA_SEND_BUFFER[LENGTH]=':';
						LENGTH++;
						NUMBER_BUFFER=0;
						for (count=0;count<2;count++)
						{
							NUMBER_BUFFER=(NUMBER_BUFFER<<8)|SENSOR_READ_BUFFER[i].READ_VALUE[(unsigned char)count];
						}
							
						unsigned char DATA_LENGTH_DECIMAL=inttostring(NUMBER_STRING_BUFFER,NUMBER_BUFFER);
						/*DATA*/
						for(count=0;count<DATA_LENGTH_DECIMAL;count++)
						{
							DATA_SEND_BUFFER[LENGTH+(unsigned char)count]=NUMBER_STRING_BUFFER[(unsigned char)count];
						}
						LENGTH=LENGTH+count;
						DATA_SEND_BUFFER[LENGTH]=':';
						DATA_SEND_BUFFER[LENGTH+1]=0;	
						xSemaphoreTake(QUEUE_SEMAPHORE,portMAX_DELAY);
						QUEUE_ADD_ARRAY_U8(&G_SEND_DATA_QUEUE,DATA_SEND_BUFFER,LENGTH+2);
						xSemaphoreGive(QUEUE_SEMAPHORE);
						/*SEND FAILED*/
					}	
			}
		}
		SENSOR_READ_BUFFER_LENGTH=0;
		if((SENSOR_READ_BUFFER_LENGTH==0)&&(SENSOR_SEND_BUFFER_LENGTH==0))
		{
			F_LAST_RECIVED_DATA_NOT_HANDLED=0;
		}
	}
}








































void SENSORS_IO_SETUP()
{
	gpio_inputconfg(PORTE_DDRA_ADDRESS,PORTE_INMOD_ADDRESS,PULLUP_mod,6);
}

void SENSORS_LOAD_EEPROM()
{
	if (readeeprom(MY_EEPROM_FIRST_USE)==NOT_FIRST_USE_VALUE)
	{
	
		NUMBER_OF_SENSORS=readeeprom(MY_EEPROM_NUMBER_OF_SENSORS);
		unsigned char i=0;
		for (i=0;i<NUMBER_OF_SENSORS_IN_DATA_BASE;i++)
		{
			MY_SENSORS[i].SENSOR_ADDRESS_LENGTH=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_ADDRESS[0]=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+1+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_ADDRESS[1]=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+2+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_ADDRESS[2]=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+3+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_ADDRESS[3]=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+4+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_ADDRESS[4]=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+5+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_PASSWORD[0]=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+6+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_PASSWORD[1]=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+7+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_PASSWORD[2]=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+8+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_PASSWORD[3]=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+9+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_VERSION=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+10+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_TYPE=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+11+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_STATE=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+12+(SIZE_OF_SENSOR_DATA_EEPROM*i));
			MY_SENSORS[i].SENSOR_OCCUBIED=readeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+13+(SIZE_OF_SENSOR_DATA_EEPROM*i));		
		}
	}
	else
	{
		unsigned char i=0;
		NUMBER_OF_SENSORS=0;
		/*set the num of sensors to zero*/
		writeeeprom(MY_EEPROM_NUMBER_OF_SENSORS,0);
		/*set the sensor array to zeros*/
		for (i=0;i<SIZE_OF_SENSOR_DATA_EEPROM*NUMBER_OF_SENSORS_IN_DATA_BASE;++i) 
		{
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+i,0);
		}
		/*set the value of checkfirst use to its value*/
		writeeeprom(MY_EEPROM_FIRST_USE,NOT_FIRST_USE_VALUE);
	}
}

void SAVE_OLD_LAST_ENTRY()
{
	writeeeprom(MY_EEPROM_START_SAVED_OLD_ENTRY,OLD_LAST_ENTRY>>24);
	writeeeprom(MY_EEPROM_START_SAVED_OLD_ENTRY+1,OLD_LAST_ENTRY>>16);
	writeeeprom(MY_EEPROM_START_SAVED_OLD_ENTRY+2,OLD_LAST_ENTRY>>8);
	writeeeprom(MY_EEPROM_START_SAVED_OLD_ENTRY+3,OLD_LAST_ENTRY);
}
void LOAD_OLD_LAST_ENTRY()
{
	OLD_LAST_ENTRY=0;
	if (readeeprom(MY_EEPROM_FLAG_SAVED_OLD_ENTRY)==SAVED_OLD_ENTRY_VALUE)
	{
		unsigned char i=0;
		
		for (i=0;i<4;i++)
		{
			OLD_LAST_ENTRY=(OLD_LAST_ENTRY<<8)|readeeprom(MY_EEPROM_START_SAVED_OLD_ENTRY+i);
		}
		
		
	}
	else
	{
			writeeeprom(MY_EEPROM_FLAG_SAVED_OLD_ENTRY,SAVED_OLD_ENTRY_VALUE);
			writeeeprom(MY_EEPROM_START_SAVED_OLD_ENTRY,0);
			writeeeprom(MY_EEPROM_START_SAVED_OLD_ENTRY+1,0);
			writeeeprom(MY_EEPROM_START_SAVED_OLD_ENTRY+2,0);
			writeeeprom(MY_EEPROM_START_SAVED_OLD_ENTRY+3,0);
	}
}


void SENSOR_SAVE(unsigned char index)
{
	
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM,MY_SENSORS[index].SENSOR_ADDRESS_LENGTH);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+1,MY_SENSORS[index].SENSOR_ADDRESS[0]);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+2,MY_SENSORS[index].SENSOR_ADDRESS[1]);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+3,MY_SENSORS[index].SENSOR_ADDRESS[2]);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+4,MY_SENSORS[index].SENSOR_ADDRESS[3]);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+5,MY_SENSORS[index].SENSOR_ADDRESS[4]);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+6,MY_SENSORS[index].SENSOR_PASSWORD[0]);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+7,MY_SENSORS[index].SENSOR_PASSWORD[1]);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+8,MY_SENSORS[index].SENSOR_PASSWORD[2]);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+9,MY_SENSORS[index].SENSOR_PASSWORD[3]);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+10,MY_SENSORS[index].SENSOR_VERSION);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+11,MY_SENSORS[index].SENSOR_TYPE);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+12,MY_SENSORS[index].SENSOR_STATE);
			writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+13,MY_SENSORS[index].SENSOR_OCCUBIED);
	
	
}

void SENSOR_SAVE_ALL()
{
	writeeeprom(MY_EEPROM_NUMBER_OF_SENSORS,NUMBER_OF_SENSORS);
	unsigned char index;
	
	for (index=0;index<NUMBER_OF_SENSORS_IN_DATA_BASE;++index)
	{
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM,MY_SENSORS[index].SENSOR_ADDRESS_LENGTH);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+1,MY_SENSORS[index].SENSOR_ADDRESS[0]);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+2,MY_SENSORS[index].SENSOR_ADDRESS[1]);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+3,MY_SENSORS[index].SENSOR_ADDRESS[2]);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+4,MY_SENSORS[index].SENSOR_ADDRESS[3]);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+5,MY_SENSORS[index].SENSOR_ADDRESS[4]);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+6,MY_SENSORS[index].SENSOR_PASSWORD[0]);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+7,MY_SENSORS[index].SENSOR_PASSWORD[1]);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+8,MY_SENSORS[index].SENSOR_PASSWORD[2]);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+9,MY_SENSORS[index].SENSOR_PASSWORD[3]);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+10,MY_SENSORS[index].SENSOR_VERSION);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+11,MY_SENSORS[index].SENSOR_TYPE);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+12,MY_SENSORS[index].SENSOR_STATE);
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+index*SIZE_OF_SENSOR_DATA_EEPROM+13,MY_SENSORS[index].SENSOR_OCCUBIED);
	}
}
/*RETURN ITS INDEX OR RETURN -1 OF FAILED*/
char ADD_NEW_SENSOR(SENSOR_DT NEW_SENSOR)
{
	char index=0;
	if (NUMBER_OF_SENSORS==NUMBER_OF_SENSORS_IN_DATA_BASE)
	{
		 index=-1;
	}
	else
	{ 
		if (FIND_SENSOR(NEW_SENSOR.SENSOR_ADDRESS,5)==(char)(-1))/*if the same address was found*/
		{
			while(MY_SENSORS[(unsigned char)index].SENSOR_OCCUBIED==1)
			{
				++index;
			}
			MY_SENSORS[(unsigned char)index].SENSOR_ADDRESS_LENGTH=5/*NEW_SENSOR.SENSOR_ADDRESS_LENGTH*/;
			MY_SENSORS[(unsigned char)index].SENSOR_ADDRESS[0]=NEW_SENSOR.SENSOR_ADDRESS[0];
			MY_SENSORS[(unsigned char)index].SENSOR_ADDRESS[1]=NEW_SENSOR.SENSOR_ADDRESS[1];
			MY_SENSORS[(unsigned char)index].SENSOR_ADDRESS[2]=NEW_SENSOR.SENSOR_ADDRESS[2];
			MY_SENSORS[(unsigned char)index].SENSOR_ADDRESS[3]=NEW_SENSOR.SENSOR_ADDRESS[3];
			MY_SENSORS[(unsigned char)index].SENSOR_ADDRESS[4]=NEW_SENSOR.SENSOR_ADDRESS[4];
		
			MY_SENSORS[(unsigned char)index].SENSOR_PASSWORD[0]=NEW_SENSOR.SENSOR_PASSWORD[0];
			MY_SENSORS[(unsigned char)index].SENSOR_PASSWORD[1]=NEW_SENSOR.SENSOR_PASSWORD[1];
			MY_SENSORS[(unsigned char)index].SENSOR_PASSWORD[2]=NEW_SENSOR.SENSOR_PASSWORD[2];
			MY_SENSORS[(unsigned char)index].SENSOR_PASSWORD[3]=NEW_SENSOR.SENSOR_PASSWORD[3];
		
			MY_SENSORS[(unsigned char)index].SENSOR_VERSION=NEW_SENSOR.SENSOR_VERSION;
			MY_SENSORS[(unsigned char)index].SENSOR_TYPE=NEW_SENSOR.SENSOR_TYPE;
			MY_SENSORS[(unsigned char)index].SENSOR_STATE=1;
			MY_SENSORS[(unsigned char)index].SENSOR_OCCUBIED=1;
			++NUMBER_OF_SENSORS;
			SENSOR_SAVE((unsigned char)index);
			NUM_SENSOR_SAVE();
		}
		else
		{
			 index=-1;
		}
		
		
		
	}
	return index;
	
}

void NUM_SENSOR_SAVE()
{
	writeeeprom(MY_EEPROM_NUMBER_OF_SENSORS,NUMBER_OF_SENSORS);
}

char FIND_SENSOR(unsigned char* ADDRESS,unsigned char ADDRESS_LENGTH)/*RETURN INDEX OR -1*/
{
	char index=0,counter_address,miss_match=0;
	if (NUMBER_OF_SENSORS==0)/*ARRAY IS EMPTY*/
	{
		return -1;
	}
	
	for (index=0;index<NUMBER_OF_SENSORS_IN_DATA_BASE;++index)
	{
		if (MY_SENSORS[(unsigned char)index].SENSOR_OCCUBIED==1)
		{
			miss_match=0;
			for (counter_address=0;counter_address<ADDRESS_LENGTH;++counter_address)
			{
				if (MY_SENSORS[(unsigned char)index].SENSOR_ADDRESS[(unsigned char)counter_address]!=ADDRESS[(unsigned char)counter_address])
				{
					miss_match=1;	
					break;
				}
			}
			if (miss_match==0)
			{
				return index;
			}
		}
	}	
	return -1;
	
}

void DELETE_SENSOR(unsigned char* ADDRESS,unsigned char ADDRESS_LENGTH)
{
	char index=FIND_SENSOR(ADDRESS,ADDRESS_LENGTH);
	if (index!=-1)
	{
		MY_SENSORS[(unsigned char)index].SENSOR_OCCUBIED=0;
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+((unsigned char)index)*SIZE_OF_SENSOR_DATA_EEPROM+13,0);
		--NUMBER_OF_SENSORS;
		NUM_SENSOR_SAVE();
	}
}

void DELETE_ALL_SENSOR()
{
	volatile unsigned char index=0;
	for(index=0;index<NUMBER_OF_SENSORS_IN_DATA_BASE;++index)
	{
		MY_SENSORS[(unsigned char)index].SENSOR_OCCUBIED=0;
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+((unsigned char)index)*SIZE_OF_SENSOR_DATA_EEPROM+13,0);
	}
	NUMBER_OF_SENSORS=0;
	NUM_SENSOR_SAVE();
}

void SENSOR_STATE_ENABLE(unsigned char* ADDRESS,unsigned char ADDRESS_LENGTH)
{
	char index=FIND_SENSOR(ADDRESS,ADDRESS_LENGTH);
	if (index!=-1)
	{
		MY_SENSORS[(unsigned char)index].SENSOR_STATE=1;
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+((unsigned char)index)*SIZE_OF_SENSOR_DATA_EEPROM+12,1);
	}
}

void SENSOR_STATE_DISABLE(unsigned char* ADDRESS,unsigned char ADDRESS_LENGTH)
{
	char index=FIND_SENSOR(ADDRESS,ADDRESS_LENGTH);
	if (index!=-1)
	{
		MY_SENSORS[(unsigned char)index].SENSOR_STATE=0;
		writeeeprom(MY_EEPROM_START_OF_SENSOR_ARRAY+((unsigned char)index)*SIZE_OF_SENSOR_DATA_EEPROM+12,0);
	}
}



void INT6_ISR()
{
	DELETE_COUNTER=0;
	DELETE_COUNTER_FLAG=1;
}

void SENSOR_TIMER3()
{
	unsigned char index;
	for (index=0;index<NUMBER_OF_SENSORS_IN_DATA_BASE;++index)
	{
		if (MY_SENSORS[index].SENSOR_OCCUBIED)
		{
			if (MY_SENSORS[index].SENSOR_STATE)
			{
				if (MY_SENSORS[index].F_CONNECTED)
				{
					if (MY_SENSORS[index].C_T_BEACON!=MAX_TIMER_BEACON_COUNTER)
					{
						MY_SENSORS[index].C_T_BEACON++;
					}
					if (MY_SENSORS[index].C_T_SESSION_KEY!=MAX_SESSION_KEY_TIMER_COUNT)
					{
						MY_SENSORS[index].C_T_SESSION_KEY++;
					}
				}
				else
				{
					if (MY_SENSORS[index].C_T_TRY_CONNECTED!=MAX_TIMER_CONNECT_COUNT_SENSOR)
					{
						MY_SENSORS[index].C_T_TRY_CONNECTED++;
					}
				}
			}
		}
	}
	
	if(DELETE_COUNTER_FLAG)
	{
		if (isbitclear(*PORTE_IN_ADDRESS,6))
		{
			DELETE_COUNTER++;
			if (DELETE_COUNTER==8)
			{
				DELETE_COUNTER_FLAG=0;
				DELETE_COUNTER=0;
				DELETE_FLAG=1;
			}
		}
		else
		{
			DELETE_COUNTER_FLAG=0;		
			DELETE_COUNTER=0;
			DELETE_INT_FLAG=0;
		}
	}
	
}
