/*
 * WIFI.c
 *
 * Created: 30-Mar-21 6:55:41 PM
 *  Author: Mina Medhat
 */ 
#include "WIFI.h"
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "semphr.h"


extern SemaphoreHandle_t QUEUE_SEMAPHORE;

/*flags*/ /*F_ MEANS FLAG*/ /*F_T_ MEANS FLAG UPDATED IN TIMER */ /*F_B_ MEAN FLAG IN BLUETOOTH*/
volatile unsigned char F_VALID_USER_PASS;/*THIS FLAG INDICATES THAT USER NAMES AND PASWORDS READY*/
volatile unsigned char F_VALID_IOT_PAR;/*THIS FLAG INDICATES THAT iot par READY*/
volatile unsigned char F_WIFI_CONNECTED=0;
volatile unsigned char F_T_TRY_TO_CONNECT;/*5 SEC*/
volatile unsigned char F_T_READY_TO_READ;/*2 SEC*/
volatile unsigned char F_T_READY_TO_SEND;/*5 SEC BASED ON SERVER*/
volatile unsigned char F_LAST_RECIVED_DATA_NOT_HANDLED;							/*GO TO SENSOR LIBIRARY*/

volatile unsigned char F_B_NEW_USER_PASS=0;
volatile unsigned char F_B_NEW_IOT_PAR=0;

volatile unsigned char FAILED_READ_HTTP_COUNTER=0;
/************/


/*BUFFERS FROM BLUETOOTH*/

/*wifi*/
volatile unsigned char F_B_USER_NAME[15];
volatile unsigned char F_B_PASSWORD[15];
/*IOT*/
volatile unsigned char F_B_CH_ID[10];
volatile unsigned char F_B_WRITE_KEY[25];
volatile unsigned char F_B_FIELD_NUM;
volatile unsigned char F_B_CH_ID_LENGTH;
volatile unsigned char F_B_WRITE_KEY_LENGTH;

/**/
/*SHARED DATA FROM AND TO THE SENSOR FROM INTERNET*/
ESP_RECIVED_DATA_DT G_RECIVED_DATA[5];
volatile QUEUE_ARRAY_UINT8 G_SEND_DATA_QUEUE;
/**/

/*wifi*/
volatile unsigned char USER_NAME[15];
volatile unsigned char PASSWORD[15];
volatile unsigned char USER_LENGTH;
volatile unsigned char PASS_LENGTH;
/*IOT*/
volatile unsigned char G_CH_ID[10];
volatile unsigned char G_WRITE_KEY[25];
volatile unsigned char G_FIELD_NUM;
volatile unsigned char G_CH_ID_LENGTH;
volatile unsigned char G_WRITE_KEY_LENGTH;



SemaphoreHandle_t ESP_SEMAPHORE; 
/*volatile SemaphoreHandle_t RX_ESP_SEMAPHORE;*/
volatile SemaphoreHandle_t TX_ESP_SEMAPHORE;
volatile QueueHandle_t RX_ESP_QUEUE;
void WIFI_TASK(void * pd)
{
	/*RX_ESP_SEMAPHORE=xSemaphoreCreateBinary();
	xSemaphoreGive(RX_ESP_SEMAPHORE);*/
	TX_ESP_SEMAPHORE=xSemaphoreCreateBinary();
	xSemaphoreGive(TX_ESP_SEMAPHORE);
	ESP_SEMAPHORE=xSemaphoreCreateBinary();
	RX_ESP_QUEUE=xQueueCreate(255,1);
	TEST_UART_SEND_str("que");
	TEST_UART_SEND_VALUE(RX_ESP_QUEUE);
	
	
	if (ESP_SETUP()==0)
	{
		xSemaphoreGive(ESP_SEMAPHORE);
		TEST_UART_SEND_str("ESP READY\n");
	}
	else
	{
		TEST_UART_SEND_str("ESP NOT READY\n");
	}
	WIFI_LOAD_PARAMETERS();
	TEST_ADD_USER_PASS();
	TEST_ADD_IOT_PARAMETERS();
	while (1)
	{
		 if (xSemaphoreTake(ESP_SEMAPHORE,0)==pdFAIL)
		 {
			 if (ESP_SETUP()==0)
			 {
				 xSemaphoreGive(ESP_SEMAPHORE);
			 }
		 }
		 else
		 {
			WIFI_LOOP();
			xSemaphoreGive(ESP_SEMAPHORE);
		 }
		 
		 vTaskDelay(pdMS_TO_TICKS(10));
	}
}



void WIFI_LOOP()
{
	/*to connect*/
	if (!F_WIFI_CONNECTED)
	{
		clearbit(*WIFI_CONNECTED_OUT_ADDRESS,WIFI_CONNECTED_PIN);
		if (F_T_TRY_TO_CONNECT==MAX_F_T_TRY_TO_CONNECT)
		{
			F_T_TRY_TO_CONNECT=0;
			if (F_VALID_USER_PASS)
			{
				unsigned char LOCAL_CONNECTION_STATE=0;			
				TEST_UART_SEND_str("ap try to connect\n");
				if (ESP_CONNECT_AP(USER_NAME,USER_LENGTH,PASSWORD,PASS_LENGTH,&LOCAL_CONNECTION_STATE)==0)
				{
					if (LOCAL_CONNECTION_STATE==1)
					{
						TEST_UART_SEND_str("ap connected\n");
						setbit(*WIFI_CONNECTED_OUT_ADDRESS,WIFI_CONNECTED_PIN);
						F_WIFI_CONNECTED=1;
						/*ADD LED TO INDICATE CONNECTION TO AP*/
					}
					else
					{
						/*TEST_UART_SEND_str("ap not connected\n");*/
						clearbit(*WIFI_CONNECTED_OUT_ADDRESS,WIFI_CONNECTED_PIN);
						/*ADD LED TO INDICATE NOT CONNECTION TO AP SAME AS UP BUT TURN OFF*/
					}
				}
				else
				{
					//error esp out
					
				}		
			}
		}
	}
	else
	{
		setbit(*WIFI_CONNECTED_OUT_ADDRESS,WIFI_CONNECTED_PIN);
		/*to read*/
		if(F_T_READY_TO_READ==MAX_T_READY_TO_READ)
		{
			if(F_VALID_IOT_PAR)
			{
				if(!F_LAST_RECIVED_DATA_NOT_HANDLED)
				{
					
					/*TEST_UART_SEND_str("READ SERVER\n");*/
					F_T_READY_TO_READ=0;
					if(ESP_READ_HTTP(G_CH_ID , G_CH_ID_LENGTH,G_FIELD_NUM,G_RECIVED_DATA)==0)
					{
						/*ADD LED NO ERROR SERVER*/
						
						/*TESTING*/
						
						/*TEST_UART_SEND_str("\n");
						TEST_UART_SEND_ARRAY(G_RECIVED_DATA[0].DATA_ELEMENT,G_RECIVED_DATA[0].length);
						TEST_UART_SEND_str("\n");
						TEST_UART_SEND_ARRAY(G_RECIVED_DATA[1].DATA_ELEMENT,G_RECIVED_DATA[1].length);
						TEST_UART_SEND_str("\n");
						TEST_UART_SEND_ARRAY(G_RECIVED_DATA[2].DATA_ELEMENT,G_RECIVED_DATA[2].length);
						TEST_UART_SEND_str("\n");
						TEST_UART_SEND_ARRAY(G_RECIVED_DATA[3].DATA_ELEMENT,G_RECIVED_DATA[3].length);
						TEST_UART_SEND_str("\n");
						TEST_UART_SEND_ARRAY(G_RECIVED_DATA[4].DATA_ELEMENT,G_RECIVED_DATA[4].length);
						TEST_UART_SEND_str("\n");	*/				
							TEST_UART_SEND_str("DATA READ FROM SERVER\n");
						
						/*****************************/
						FAILED_READ_HTTP_COUNTER=0;
						F_LAST_RECIVED_DATA_NOT_HANDLED=1;/*MAKE SENSOR WORK ON IT*/
					}	
					else
					{
						FAILED_READ_HTTP_COUNTER++;
						/*ADD LED ERROR SERVER*/
						if(FAILED_READ_HTTP_COUNTER==FAILED_READ_HTTP_COUNTER_MAX)
						{
							FAILED_READ_HTTP_COUNTER=0;
							F_WIFI_CONNECTED=0;
						}
						
						
					}
				}
			}
			
		}
		/*to send*/
		if(F_T_READY_TO_SEND==MAX_T_READY_TO_SEND)
		{
			if(F_VALID_IOT_PAR)
			{
				if(G_SEND_DATA_QUEUE.COUNT!=0)
				{
					unsigned char *LOCAL_DATA_SEND;
					unsigned char LOCAL_DATA_SEND_LENGTH=0;
					TEST_UART_SEND_str("WRITE\n");
					xSemaphoreTake(QUEUE_SEMAPHORE,portMAX_DELAY);
					LOCAL_DATA_SEND=QUEUE_GET_FRONT_ARRAY_U8(&G_SEND_DATA_QUEUE);
					xSemaphoreGive(QUEUE_SEMAPHORE);
					while (LOCAL_DATA_SEND[LOCAL_DATA_SEND_LENGTH]!=0)
					{
						LOCAL_DATA_SEND_LENGTH++;
					}
					F_T_READY_TO_SEND=0;
					TEST_UART_SEND_str("WRITE1\n");
					if(ESP_WRITE_HTTP(G_WRITE_KEY,G_WRITE_KEY_LENGTH,G_FIELD_NUM,LOCAL_DATA_SEND,LOCAL_DATA_SEND_LENGTH)==0)
					{
						xSemaphoreTake(QUEUE_SEMAPHORE,portMAX_DELAY);
						QUEUE_DELETE_FRONT_ARRAY_U8(&G_SEND_DATA_QUEUE);
						xSemaphoreGive(QUEUE_SEMAPHORE);
					}
					else
					{
						F_WIFI_CONNECTED=0;
					}
				}
				
			}
		}

	}
	
	if(F_B_NEW_USER_PASS)
	{
		F_B_NEW_USER_PASS=0;
		WIFI_UPDATE_USER_PASS(F_B_USER_NAME,F_B_PASSWORD);
	}
	
	if(F_B_NEW_IOT_PAR)
	{
		F_B_NEW_IOT_PAR=0;
		WIFI_UPDATE_IOT_PAR(F_B_CH_ID,F_B_CH_ID_LENGTH,F_B_WRITE_KEY,F_B_WRITE_KEY_LENGTH,F_B_FIELD_NUM);
	}
}



























void WIFI_SETUP_IO()
{
	gpio_outputconfg(WIFI_CONNECTED_DDR_ADDRESS,WIFI_CONNECTED_OUTMOD_ADDRESS,OUTPASS,WIFI_CONNECTED_PIN);
}





void WIFI_LOAD_PARAMETERS()
{
	/*CHECK USER AND PASS*/
	if (readeeprom(MY_EEPROM_CHECK_AVAILABLE_USER_PASS)==CHECK_AVAILABLE_USER_PASS_VALUE)
	{
		USER_LENGTH=readeeprom(MY_EEPROM_USER_LENGTH);
		PASS_LENGTH=readeeprom(MY_EEPROM_PASS_LENGTH);
		unsigned char i=0;
		for (i=0;i<USER_LENGTH;++i)
		{
			USER_NAME[i]=readeeprom(MY_EEPROM_START_OF_USER+i);
		}
		for (i=0;i<PASS_LENGTH;++i)
		{
			PASSWORD[i]=readeeprom(MY_EEPROM_START_OF_PASS+i);
		}
		F_VALID_USER_PASS=1;
	}
	
	
	/*CHECK IOT PAR*/
		if (readeeprom(MY_EEPROM_CHECK_AVAILABLE_IOT_PAR)==CHECK_AVAILABLE_IOT_PAR_VALUE)
		{
			G_CH_ID_LENGTH=readeeprom(MY_EEPROM_CH_ID_LENGTH);
			G_WRITE_KEY_LENGTH=readeeprom(MY_EEPROM_WRITE_KEY_LENGTH);
			G_FIELD_NUM=readeeprom(MY_EEPROM_FIELD_NUM);
			
			unsigned char i=0;
			for (i=0;i<G_CH_ID_LENGTH;++i)
			{
				G_CH_ID[i]=readeeprom(MY_EEPROM_START_OF_CH_ID+i);
			}
			for (i=0;i<G_WRITE_KEY_LENGTH;++i)
			{
				G_WRITE_KEY[i]=readeeprom(MY_EEPROM_START_OF_WRITE_KEY+i);
			}	
			F_VALID_IOT_PAR=1;
			/*ADD LED TO INDICATE CONNECTION TO SERVER*/
		}
		
}

void WIFI_UPDATE_USER_PASS(volatile unsigned char*LOCAL_USER,volatile unsigned char*LOCAL_PASS)  
{
	unsigned char i=0;
	/*save user name*/
	while(LOCAL_USER[i]!=0)
	{
		writeeeprom(MY_EEPROM_START_OF_USER+i,LOCAL_USER[i]);
		USER_NAME[i]=LOCAL_USER[i];
		i++;
	}
	writeeeprom(MY_EEPROM_USER_LENGTH,i);
	USER_LENGTH=i;
	/*save user name*/
	i=0;
	while(LOCAL_PASS[i]!=0)
	{
		writeeeprom(MY_EEPROM_START_OF_PASS+i,LOCAL_PASS[i]);
		PASSWORD[i]=LOCAL_PASS[i];
		i++;
	}
	writeeeprom(MY_EEPROM_PASS_LENGTH,i);
	PASS_LENGTH=i;
	writeeeprom(MY_EEPROM_CHECK_AVAILABLE_USER_PASS,CHECK_AVAILABLE_USER_PASS_VALUE);	
	F_VALID_USER_PASS=1;
}

void WIFI_UPDATE_IOT_PAR(volatile unsigned char *CHANNEL_ID,volatile unsigned char CHANNEL_ID_LENGTH,volatile unsigned char *API_WRITE_KEY,volatile unsigned char API_WRITE_KEY_LENGTH,volatile unsigned char FIELD_NUM)
{
	unsigned char i=0;
	/*save channel id*/
	for(i=0;i<CHANNEL_ID_LENGTH;i++)
	{
		writeeeprom(MY_EEPROM_START_OF_CH_ID+i,CHANNEL_ID[i]);
		G_CH_ID[i]=CHANNEL_ID[i];
	}
	writeeeprom(MY_EEPROM_CH_ID_LENGTH,i);
	G_CH_ID_LENGTH=i;
	/*save write api key*/
	
	for(i=0;i<API_WRITE_KEY_LENGTH;i++)
	{
		writeeeprom(MY_EEPROM_START_OF_WRITE_KEY+i,API_WRITE_KEY[i]);
		G_WRITE_KEY[i]=API_WRITE_KEY[i];
	}
	writeeeprom(MY_EEPROM_WRITE_KEY_LENGTH,i);
	G_WRITE_KEY_LENGTH=i;
	/*save field*/
	writeeeprom(MY_EEPROM_FIELD_NUM,FIELD_NUM);
	G_FIELD_NUM=FIELD_NUM;
	writeeeprom(MY_EEPROM_CHECK_AVAILABLE_IOT_PAR,CHECK_AVAILABLE_IOT_PAR_VALUE);
	F_VALID_IOT_PAR=1;
	/*ADD LED TO INDICATE CONNECTION TO SERVER*/
}

void WIFI_TIMER3()
{
	
	if (F_T_TRY_TO_CONNECT!=MAX_F_T_TRY_TO_CONNECT)
	{
		F_T_TRY_TO_CONNECT++;
	}
	if (F_T_READY_TO_READ!=MAX_T_READY_TO_READ)
	{
		F_T_READY_TO_READ++;
	}
	if (F_T_READY_TO_SEND!=MAX_T_READY_TO_SEND)
	{
		F_T_READY_TO_SEND++;
	}
}

void WIFI_RESET()
{
	
	
		writeeeprom(MY_EEPROM_CHECK_AVAILABLE_USER_PASS,0XFF);
		writeeeprom(MY_EEPROM_CHECK_AVAILABLE_IOT_PAR,0XFF);
		F_WIFI_CONNECTED=0;
		F_VALID_USER_PASS=0;
		F_VALID_IOT_PAR=0;


}



