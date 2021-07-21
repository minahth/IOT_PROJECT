/*
 * ESP_WIFI_SIMPLIFIED.c
 *
 * Created: 22-Jan-21 12:22:34 AM
 *  Author: ideapad510
 */ 
#include "ESP_WIFI_SIMPLIFIED.h"
#include "TEST.h"
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "semphr.h"
#include "queue.h"
/*extern volatile SemaphoreHandle_t RX_ESP_SEMAPHORE;*/
extern volatile SemaphoreHandle_t TX_ESP_SEMAPHORE;
volatile unsigned long NEW_LAST_ENTRY; 


extern volatile QueueHandle_t RX_ESP_QUEUE;

unsigned char ESP_READ_CHAR_AND_CHECK(unsigned char X)
{
	unsigned char RECIVE_TEMP_DATA;
	unsigned char V;
	RECIVE_TEMP_DATA=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
	if (V==0)
	{
		return 1;//WIFI MODULE NOT CONNECTED
	}
	if (RECIVE_TEMP_DATA!=X)
	{
		
		return 2;//wrong data
	}
	return 0;
}



unsigned char ESP_AT_COMMAND()
{
	unsigned char RECIVE_TEMP_DATA;
	volatile unsigned char V;
	unsigned char TEST;
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT\r\n",4);
	RECIVE_TEMP_DATA=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
	if (V==0)
	{
		/*TEST_UART_SEND_str("B1");*/
		return 1;//WIFI MODULE NOT CONNECTED
	}
	
	if (RECIVE_TEMP_DATA=='b')
	{
		/*TEST_UART_SEND_str("B2");*/
		return 3;//busy
	}
	
	if (RECIVE_TEMP_DATA=='A')
	{
		/*
		WAITING AT/r/r/n 
		*/
		/*TEST_UART_SEND_str("B3");*/
		TEST=ESP_READ_CHAR_AND_CHECK('T');
		if (TEST)
		{
			return TEST;
		}
		/*TEST_UART_SEND_str("B4");*/
		TEST=ESP_READ_CHAR_AND_CHECK('\r');
		if (TEST)
		{
			return TEST;
		}
		/*TEST_UART_SEND_str("B5");*/
		TEST=ESP_READ_CHAR_AND_CHECK('\r');
		if (TEST)
		{
			return TEST;
		}
		TEST=ESP_READ_CHAR_AND_CHECK('\n');
		if (TEST)
		{
			return TEST;
		}
		TEST=ESP_READ_CHAR_AND_CHECK('\r');
		if (TEST)
		{
			return TEST;
		}
					
	}
		/*   /r/nOK/r/n	  */
		
		
		
		TEST=ESP_READ_CHAR_AND_CHECK('\n');
		if (TEST)
		{/*TEST_UART_SEND_VALUE(TEST);*/
			/*TEST_UART_SEND_str("G1");*/
			return TEST;
		}
		TEST=ESP_READ_CHAR_AND_CHECK('O');
		if (TEST)
		{/*TEST_UART_SEND_VALUE(TEST);*/
			/*TEST_UART_SEND_str("G2");*/
			return TEST;
		}
		TEST=ESP_READ_CHAR_AND_CHECK('K');
		if (TEST)
		{/*TEST_UART_SEND_VALUE(TEST);*/
			/*TEST_UART_SEND_str("G3");*/
			return TEST;
		}
		TEST=ESP_READ_CHAR_AND_CHECK('\r');
		if (TEST)
		{
		/*	TEST_UART_SEND_VALUE(TEST);
			TEST_UART_SEND_str("G4");*/
			return TEST;
		}
		TEST=ESP_READ_CHAR_AND_CHECK('\n');
		if (TEST)
		{/*TEST_UART_SEND_VALUE(TEST);
			TEST_UART_SEND_str("G5");*/
			return TEST;
		}
		return 0; /*done*/
}

unsigned char ESP_ATE_COMMAND()
{
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("ATE0\r\n",6);
	vTaskDelay(pdMS_TO_TICKS(1000));
	return 0;
}

unsigned char ESP_CWMODE_1()
{
	unsigned char test;
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT+CWMODE_CUR=1\r\n",17);
	test=ESP_CHECK_OK_RESP();
	if (test)
	{
		return test;
	}
	return 0; /*done*/
}

unsigned char ESP_CWLAPOPT_1()
{
	unsigned char test;
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT+CWLAPOPT=1,2\r\n",17);
	test=ESP_CHECK_OK_RESP();
	if (test)
	{
		return test;
	}
	return 0; /*done*/
}

unsigned char ESP_CHECK_OK_RESP()
{
	volatile unsigned char TEST;
	TEST=ESP_READ_CHAR_AND_CHECK('\r');
	if (TEST)
	{
		return TEST;
	}
	TEST=ESP_READ_CHAR_AND_CHECK('\n');
	if (TEST)
	{
		return TEST;
	}
	TEST=ESP_READ_CHAR_AND_CHECK('O');
	if (TEST)
	{
		return TEST;
	}
	TEST=ESP_READ_CHAR_AND_CHECK('K');
	if (TEST)
	{
		return TEST;
	}
	TEST=ESP_READ_CHAR_AND_CHECK('\r');
	if (TEST)
	{
		return TEST;
	}
	TEST=ESP_READ_CHAR_AND_CHECK('\n');
	if (TEST)
	{
		return TEST;
	}
	return 0; /*done*/
}


unsigned char ESP_CIPSTATUS(unsigned char *RETURN_VALUE)
{
	unsigned char RECIVE_TEMP_DATA[1];
	unsigned char V;
	unsigned char TEST;
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT+CIPSTATUS\r\n",14);
	TEST=ESP_READ_CHAR_AND_CHECK('S');
	if (TEST)
	{
		return TEST;
	}
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!=':');
	RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
	if (V==0)
	{
		return 1;//WIFI MODULE NOT CONNECTED
	}
	*RETURN_VALUE=RECIVE_TEMP_DATA[0]-48;
	TEST=ESP_READ_CHAR_AND_CHECK('\r');
	if (TEST)
	{
		return TEST;
	}
	TEST=ESP_READ_CHAR_AND_CHECK('\n');
	if (TEST)
	{
		return TEST;
	}
	
	if(*RETURN_VALUE==3)
	{
		do
		{
			RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
			if (V==0)
			{
				return 1;//WIFI MODULE NOT CONNECTED
			}
		} while (RECIVE_TEMP_DATA[0]!='\n');
	}
	
	TEST=ESP_CHECK_OK_RESP();
	if (TEST)
	{
		
		return TEST;
	}
	/**/
	
	
	/**/
	return 0; /*done*/
}

unsigned char ESP_CWJAP_CUR(unsigned char *AP_NAME,unsigned char AP_LENGTH,unsigned char *PASS_NAME,unsigned char PASS_LENGTH)
{
	
	unsigned char test;
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT+CWJAP_CUR=\"",14);
	ESP_UART_SEND_ARRAY((char*)AP_NAME,AP_LENGTH);
	ESP_UART_SEND_ARRAY("\",\"",3);
	ESP_UART_SEND_ARRAY((char*)PASS_NAME,PASS_LENGTH);
	ESP_UART_SEND_ARRAY("\"\r\n",3);
	do
	{
		vTaskDelay(pdMS_TO_TICKS(4000));
		vTaskDelay(pdMS_TO_TICKS(4000));
		
		ESP_UART_EMPTY_BUFFER();
		test=ESP_AT_COMMAND();
		
	} while (test==3);
	return test;
}


unsigned char ESP_OPEN_SOCKET() /*SIMPLIFIED*/
{
	unsigned char RECIVE_TEMP_DATA[1];
	unsigned char V;
	unsigned char TEST;
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n",43);
	/*CONNECT\r\n\r\nOK\r\n*/
	/*vTaskDelay(pdMS_TO_TICKS(1000));*/
	TEST=ESP_READ_CHAR_AND_CHECK('C');
	if (TEST)
	{	TEST_UART_SEND_VALUE(TEST);
		TEST_UART_SEND_str("SSS\n");
		return TEST;
	}
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
		if (V==0)
		{
			TEST_UART_SEND_str("FFF\n");
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!='\n');
	TEST=ESP_CHECK_OK_RESP();
	if (TEST)
	{
		TEST_UART_SEND_str("OKKKK\n");
		return TEST;
	}
	return 0;//done
}



unsigned char ESP_SEND_WRITE_HTTP_DATA(unsigned char *API_WRITE_KEY,unsigned char API_WRITE_KEY_LENGTH,unsigned char FIELD_NUM,unsigned char *DATA,unsigned char DATA_LENGTH)
{
	unsigned char TOTAL_LENGTH;
	unsigned char TEMP[5];
	unsigned char TEMP_length;
	unsigned char FIELD_NUM_LENGTH;
	unsigned char TEST;
	FIELD_NUM_LENGTH=inttostring(TEMP,FIELD_NUM);
	TOTAL_LENGTH=20+API_WRITE_KEY_LENGTH+6+FIELD_NUM_LENGTH+1+DATA_LENGTH+2;
	TEMP_length=inttostring(TEMP,TOTAL_LENGTH);
	ESP_UART_EMPTY_BUFFER();
	
	ESP_UART_SEND_ARRAY("AT+CIPSEND=",11);
	ESP_UART_SEND_ARRAY(TEMP,TEMP_length);
	ESP_UART_SEND_ARRAY("\r\n",2);
	TEST=ESP_CHECK_OK_RESP();
	if (TEST)
	{
		/*put for loop*/
		return TEST;
	}
	TEST=ESP_READ_CHAR_AND_CHECK('>');
	if (TEST)
	{
		return TEST;
	}
	
	FIELD_NUM_LENGTH=inttostring(TEMP,FIELD_NUM);
	ESP_UART_SEND_ARRAY("GET /update?api_key=",20);
	ESP_UART_SEND_ARRAY(API_WRITE_KEY,API_WRITE_KEY_LENGTH);
	ESP_UART_SEND_ARRAY("&field",6);
	ESP_UART_SEND_ARRAY(TEMP,FIELD_NUM_LENGTH);
	ESP_UART_SEND_BYTE('=');
	ESP_UART_SEND_ARRAY(DATA,DATA_LENGTH);	
	ESP_UART_SEND_ARRAY("\r\n",2);	
	/*ESCAPE*/
	vTaskDelay(pdMS_TO_TICKS(100));
	ESP_UART_EMPTY_BUFFER();
	
	return 0;//done
}







unsigned char ESP_SEND_READ_HTTP_DATA(unsigned char *CHANNEL_ID,unsigned char CHANNEL_ID_LENGTH,unsigned char FIELD_NUM,ESP_RECIVED_DATA_DT *DATA)
{
	unsigned char TOTAL_LENGTH;
	unsigned char TEMP[10];
	unsigned char TEMP_length;
	unsigned char FIELD_NUM_LENGTH;
	unsigned char RECIVE_TEMP_DATA[1];
	unsigned char V,i,k;
	unsigned char TEST;
	FIELD_NUM_LENGTH=inttostring(TEMP,FIELD_NUM);
	TOTAL_LENGTH=40+CHANNEL_ID_LENGTH+8+FIELD_NUM_LENGTH+16;
	TEMP_length=inttostring(TEMP,TOTAL_LENGTH);
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT+CIPSEND=",11);
	ESP_UART_SEND_ARRAY(TEMP,TEMP_length);
	ESP_UART_SEND_ARRAY("\r\n",2);
	TEST=ESP_CHECK_OK_RESP();
	if (TEST)
	{
		/*put for loop*/
		return TEST;
	}
	TEST=ESP_READ_CHAR_AND_CHECK('>');
	if (TEST)
	{
		return TEST;
	}
	
	FIELD_NUM_LENGTH=inttostring(TEMP,FIELD_NUM);
	ESP_UART_SEND_ARRAY("GET https://api.thingspeak.com/channels/" ,40);
	ESP_UART_SEND_ARRAY(CHANNEL_ID,CHANNEL_ID_LENGTH);
	ESP_UART_SEND_ARRAY("/fields/",8);
	ESP_UART_SEND_ARRAY(TEMP,FIELD_NUM_LENGTH);
	ESP_UART_SEND_ARRAY(".csv?results=5\r\n",16);
	
	/*vTaskDelay(pdMS_TO_TICKS(300));*/
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
		if (V==0)
		{
			TEST_UART_SEND_str("not hereeeeeeeeeeeeeeeeeeeeeeeee");
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!='+');
	
	
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!='\n');
	
	
	
	
	
	
	
	
	for (i=0;i<4;i++)
	{	/*TIME i*/
		do
		{
			RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
			if (V==0)
			{
				return 1;//WIFI MODULE NOT CONNECTED
			}
		} while (RECIVE_TEMP_DATA[0]!=',');
		/*ENTRY I*/
		do
		{
			RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
			if (V==0)
			{
				return 1;//WIFI MODULE NOT CONNECTED
			}
		} while (RECIVE_TEMP_DATA[0]!=',');
		/*DATA i*/
		k=0;
		do
		{
			
			RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
			if (V==0)
			{
				return 1;//WIFI MODULE NOT CONNECTED
			}
			DATA[i].DATA_ELEMENT[k]=RECIVE_TEMP_DATA[0];
			++k;
			if (k>MAX_CHAR_DATA)
			{
				break;
			}
		} while (RECIVE_TEMP_DATA[0]!='\n');
		DATA[i].length=k-1;

	}
	/*TIME 4*/
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!=',');
	
	k=0;
	/*LAST ENTRY*/
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
		TEMP[k]=RECIVE_TEMP_DATA[0];
		++k;
		if (k>10)
		{
			k--;
		}
	} while (RECIVE_TEMP_DATA[0]!=',');
	
	NEW_LAST_ENTRY=STRINGTOINT(TEMP,k-1);
	
	k=0;
	/*DATA 4*/
	do
	{
		
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
		DATA[4].DATA_ELEMENT[k]=RECIVE_TEMP_DATA[0];
		++k;
		if (k>MAX_CHAR_DATA)
		{
			break;
		}
	} while (RECIVE_TEMP_DATA[0]!='\n');
	DATA[4].length=k-1;

	/*CLOSED\r\n*/
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(&V, ESP_UART_COUNT_RECIVER1);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!='\n');
	
	return 0;//done
}


unsigned char ESP_READ_HTTP(unsigned char *CHANNEL_ID,unsigned char CHANNEL_ID_LENGTH,unsigned char FIELD_NUM,ESP_RECIVED_DATA_DT *DATA)
{
	unsigned char test,test2;
	test=ESP_AT_COMMAND();
	if (test)
	{
		/*TEST_UART_SEND_str("AT READ\n");*/
		return test;
	}
	test=ESP_CIPSTATUS(&test2);
	if (test)
	{
		/*TEST_UART_SEND_str("AT STATUS\n");*/
		return test;
	}
	
	if (test2==5)
	{
		/*TEST_UART_SEND_str("NOT CONNECTED STATUS\n");*/
		return 4;// not connected
	}
	if (test2==3)
	{
		/*TEST_UART_SEND_str("CONNECTED STATUS\n");*/
		return 5;// tcp connected
	}
	/*TEST_UART_SEND_str("PASSED\n");*/
	test=ESP_OPEN_SOCKET();
	if (test)
	{
		/*TEST_UART_SEND_str("OPEN SOCKET\n");*/
		return test;
	}
	/*TEST_UART_SEND_str("PASSED\n");*/
	test=ESP_SEND_READ_HTTP_DATA(CHANNEL_ID, CHANNEL_ID_LENGTH,FIELD_NUM,DATA);
	if (test)
	{
		/*TEST_UART_SEND_str("failed here\n");*/
		return test;
	}
	return 0;//done
}






















unsigned char ESP_SETUP()
{
	vTaskDelay(pdMS_TO_TICKS(100));
	
	
	unsigned char test;
	test=ESP_AT_COMMAND();
	if (test)
	{
		/*TEST_UART_SEND_str("X1");*/
		return test;
	}

	test=ESP_ATE_COMMAND();
	if (test)
	{/*TEST_UART_SEND_str("X2");*/
		return test;
	}
	
	test=ESP_CWMODE_1();
	if (test)
	{
		return test;
	}
	
	test=ESP_CWLAPOPT_1();
	if (test)
	{
		return test;
	}
	
	return 0;
}


unsigned char ESP_CONNECT_AP(unsigned char *AP_NAME,unsigned char AP_LENGTH,unsigned char *PASS_NAME,unsigned char PASS_LENGTH, unsigned char *connection_state)
{
	unsigned char test,test2;
	test=ESP_AT_COMMAND();
	if (test)
	{/*TEST_UART_SEND_str("A1");*/
		return test;
	}
	test=ESP_CIPSTATUS(&test2);
	if (test)
	{
		return test;
	}
	*connection_state=1;
	if (test2==5)
	{
		test=ESP_CWJAP_CUR(AP_NAME,AP_LENGTH,PASS_NAME,PASS_LENGTH);
		if (test)
		{/*TEST_UART_SEND_str("A3");*/
			return test;
		}
		test=ESP_CIPSTATUS(&test2);
		if (test)
		{/*TEST_UART_SEND_str("A4");*/
			return test;
		}
		if (test2==5)
		{
			*connection_state=0;
		}
		
	}
	/*TEST_UART_SEND_str("A5");*/
	return 0;//done
	
}



unsigned char ESP_WRITE_HTTP(unsigned char *API_WRITE_KEY,unsigned char API_WRITE_KEY_LENGTH,unsigned char FIELD_NUM,unsigned char *DATA,unsigned char DATA_LENGTH)
{
	unsigned char test,test2;
	test=ESP_AT_COMMAND();
	if (test)
	{
		return test;
	}
	test=ESP_CIPSTATUS(&test2);
	if (test)
	{
		return test;
	}
	
	if (test2==5)
	{
		return 4;// not connected
	}
	if (test2==3)
	{
		return 5;// tcp connected
	}
	test=ESP_OPEN_SOCKET();
	if (test)
	{
		return test;
	}
	test=ESP_SEND_WRITE_HTTP_DATA(API_WRITE_KEY,API_WRITE_KEY_LENGTH,FIELD_NUM,DATA, DATA_LENGTH);
	if (test)
	{
		return test;
	}
	return 0;//done
	
}


























void RX_ESP_ISR()
{
	/*TEST_UART_SEND_str("E");*/
	if (pdTRUE!=xQueueSendFromISR(RX_ESP_QUEUE,&UDR1,NULL))
	{
		/*TEST_UART_SEND_str("F");*/
	}
	
}
void TX_ESP_ISR()
{
	xSemaphoreGiveFromISR(TX_ESP_SEMAPHORE,NULL);
}

void ESP_UART_SEND_ARRAY(char *ptr,unsigned char size)
{
	unsigned char i=0;
	for(i=0;i<size;i++)
	{
		ESP_UART_SEND_BYTE(ptr[i]);
	}
}

void ESP_UART_SEND_BYTE(char data)
{
	
/*
	do 
	{
		;
	} while (pdTRUE!=xSemaphoreTake(TX_ESP_SEMAPHORE,portMAX_DELAY));
	UDR1=data;*/
	
	xSemaphoreTake(TX_ESP_SEMAPHORE,portMAX_DELAY);
	UDR1=data;
	
	
}

unsigned char ESP_UART_RECIVE_BYTE(volatile unsigned char*V,unsigned short COUNTER)
{
	unsigned char DATA;
	
	if (pdPASS==xQueueReceive(RX_ESP_QUEUE,&DATA,pdMS_TO_TICKS(COUNTER)))
	{
		/*TEST_UART_SEND_str("YES");*/
		*V=1;
	}
	else
	{
		/*TEST_UART_SEND_str("NO");*/
		*V=0;
	}
	return DATA;
	
	
	
	
	
	
	
	/*if (pdPASS==xSemaphoreTake(RX_ESP_SEMAPHORE,pdMS_TO_TICKS(COUNTER)))
	{
			TEST_UART_SEND_str("YES");	
		*V=1;
	}
	else
	{
		TEST_UART_SEND_str("NO");	
		*V=0;
	}
	return RX_BUFFER_VALUE;*/
}
