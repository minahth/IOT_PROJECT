/*
 * ESP_WIFI_SIMPLIFIED.c
 *
 * Created: 22-Jan-21 12:22:34 AM
 *  Author: ideapad510
 */ 
#include "ESP_WIFI_SIMPLIFIED.h"
#include "TEST.h"
#include "WIFI_DEFINE.h"

volatile unsigned long NEW_LAST_ENTRY; 
volatile unsigned char F_WIFI_CONNECT_STAGE_COUNTER=0;
volatile unsigned char WIFI_CONNECT_STAGE=0;
unsigned char ESP_READ_CHAR_AND_CHECK(unsigned char X)
{
	unsigned char RECIVE_TEMP_DATA;
	unsigned char V;
	RECIVE_TEMP_DATA=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
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
unsigned char ESP_AT_COMMAND()
{
	unsigned char RECIVE_TEMP_DATA;
	volatile unsigned char V;
	unsigned char TEST;
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT\r\n",4);
	RECIVE_TEMP_DATA=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
	if (V==0)
	{
		return 1;//WIFI MODULE NOT CONNECTED
	}
	
	if (RECIVE_TEMP_DATA=='b')
	{
		return 3;//busy
	}
	
	if (RECIVE_TEMP_DATA=='A')
	{
		/*
		WAITING AT/r/r/n 
		*/
		
		TEST=ESP_READ_CHAR_AND_CHECK('T');
		if (TEST)
		{
			return TEST;
		}
		TEST=ESP_READ_CHAR_AND_CHECK('\r');
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
		TEST=ESP_READ_CHAR_AND_CHECK('\r');
		if (TEST)
		{
			return TEST;
		}
					
	}
		/*   /r/nOK/r/n	  */
		
		
		
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
unsigned char ESP_ATE_COMMAND()
{
	volatile unsigned char V;
	/*unsigned char i=0;*/
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("ATE0\r\n",6);
	
	_delay_ms(1000);
	return 0;
	/*ESCAPE
	do
	{
		i++;
		ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER2,&V);
		
	}
	while(V==1);
	
	if (i>2)
	{
		return 0;//done
	}
	else
	{
		return 1;//WIFI MODULE NOT CONNECTED
	}*/
	/**/
}
unsigned char ESP_CWMODE_1()
{
	unsigned char test;
	volatile unsigned char V;
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
	volatile unsigned char V;
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT+CWLAPOPT=1,2\r\n",17);
	test=ESP_CHECK_OK_RESP();
	if (test)
	{
		return test;
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
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!=':');
	RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
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
	TEST=ESP_CHECK_OK_RESP();
	if (TEST)
	{
		return TEST;
	}
	return 0; /*done*/
}
/*here we split the function into 2 stages because it consumes alot of time in delay .. so we make the first stage 0 then move to the program while waiting 8 sec using the timer then make the second stage*/
unsigned char ESP_CWJAP_CUR(unsigned char *AP_NAME,unsigned char AP_LENGTH,unsigned char *PASS_NAME,unsigned char PASS_LENGTH)
{
	unsigned char V;
	if (WIFI_CONNECT_STAGE==0)
	{
		ESP_UART_EMPTY_BUFFER();
		ESP_UART_SEND_ARRAY("AT+CWJAP_CUR=\"",14);
		ESP_UART_SEND_ARRAY(AP_NAME,AP_LENGTH);
		ESP_UART_SEND_ARRAY("\",\"",3);
		ESP_UART_SEND_ARRAY(PASS_NAME,PASS_LENGTH);
		ESP_UART_SEND_ARRAY("\"\r\n",3);
		
		F_WIFI_CONNECT_STAGE_COUNTER=0;
		
		WIFI_CONNECT_STAGE=1;
	}
	if (F_WIFI_CONNECT_STAGE_COUNTER==MAX_WIFI_STAGE_COUNTER)
	{
		
		F_WIFI_CONNECT_STAGE_COUNTER=0;
		ESP_UART_EMPTY_BUFFER();
		return ESP_AT_COMMAND();
	}
	
	return 3;
}
/*//old this was simple but take delay 8 sec that disconnect the sensors
unsigned char ESP_CWJAP_CUR(unsigned char *AP_NAME,unsigned char AP_LENGTH,unsigned char *PASS_NAME,unsigned char PASS_LENGTH)
{
	unsigned char V;
	unsigned char test;
	
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT+CWJAP_CUR=\"",14);
	ESP_UART_SEND_ARRAY(AP_NAME,AP_LENGTH);
	ESP_UART_SEND_ARRAY("\",\"",3);
	ESP_UART_SEND_ARRAY(PASS_NAME,PASS_LENGTH);
	ESP_UART_SEND_ARRAY("\"\r\n",3);
	
	do 
	{
		_delay_ms(4000);
		_delay_ms(4000);
		
		ESP_UART_EMPTY_BUFFER();
		test=ESP_AT_COMMAND();
		
	} while (test==3);
	
	
	
	
	return test;
}*/

unsigned char ESP_OPEN_SOCKET() /*SIMPLIFIED*/
{
	unsigned char RECIVE_TEMP_DATA[1];
	unsigned char V;
	unsigned char TEST;
	ESP_UART_EMPTY_BUFFER();
	ESP_UART_SEND_ARRAY("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n",43);
	/*CONNECT\r\n\r\nOK\r\n*/
	TEST=ESP_READ_CHAR_AND_CHECK('C');
	if (TEST)
	{
		return TEST;
	}
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!='\n');
	TEST=ESP_CHECK_OK_RESP();
	if (TEST)
	{
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
	unsigned char V;
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
	_delay_ms(100);
	ESP_UART_EMPTY_BUFFER();
	
	return 0;//done
	
	/*
	RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER,&V);
	if (V==0)
	{
		return 1;//WIFI MODULE NOT CONNECTED
	}
	if (RECIVE_TEMP_DATA[0]!="\r")
	{
		return 2;//wrong data
	}
	
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER,&V);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!="R");
	
	for (i=0;i<5;++i)
	{
		do
		{
			RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER,&V);
			if (V==0)
			{
				return 1;//WIFI MODULE NOT CONNECTED
			}
		} while (RECIVE_TEMP_DATA[0]!="\n");
	}*/
	
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
	
	
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!='+');
	
	
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!='\n');
	
	
	
	
	
	
	
	
	for (i=0;i<4;i++)
	{	/*TIME i*/
		do
		{
			RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
			if (V==0)
			{
				return 1;//WIFI MODULE NOT CONNECTED
			}
		} while (RECIVE_TEMP_DATA[0]!=',');
		/*ENTRY I*/
		do
		{
			RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
			if (V==0)
			{
				return 1;//WIFI MODULE NOT CONNECTED
			}
		} while (RECIVE_TEMP_DATA[0]!=',');	
		/*DATA i*/
		k=0;
		do
		{
		
			RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
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
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!=',');
	
	k=0;
	/*LAST ENTRY*/
	do
	{
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
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
		
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
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
		RECIVE_TEMP_DATA[0]=ESP_UART_RECIVE_BYTE(ESP_UART_COUNT_RECIVER1,&V);
		if (V==0)
		{
			return 1;//WIFI MODULE NOT CONNECTED
		}
	} while (RECIVE_TEMP_DATA[0]!='\n');
	
	return 0;//done
}




















/*USED OUT IN PUBLIC*/

unsigned char ESP_SETUP()
{
	unsigned char test;
	test=ESP_AT_COMMAND();
	if (test)
	{
		return test;
	}

	test=ESP_ATE_COMMAND();
	if (test)
	{
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
// connect

unsigned char ESP_CONNECT_AP(unsigned char *AP_NAME,unsigned char AP_LENGTH,unsigned char *PASS_NAME,unsigned char PASS_LENGTH, unsigned char *connection_state)
{
	unsigned char test,test2;
	if (WIFI_CONNECT_STAGE==0)
	{
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
		*connection_state=1;
		if (test2==5)
		{
			test=ESP_CWJAP_CUR(AP_NAME,AP_LENGTH,PASS_NAME,PASS_LENGTH);
			if (test)
			{
				return test;
			}
			WIFI_CONNECT_STAGE=0;
			test=ESP_CIPSTATUS(&test2);
			if (test)
			{
				return test;
			}
			if (test2==5)
			{
				*connection_state=0;
			}
		
		}
		return 0;//done
	}
	if (WIFI_CONNECT_STAGE==1)
	{
		test=ESP_CWJAP_CUR(AP_NAME,AP_LENGTH,PASS_NAME,PASS_LENGTH);
		if (test)
		{
			return test;
		}
		WIFI_CONNECT_STAGE=0;
		test=ESP_CIPSTATUS(&test2);
		if (test)
		{
			return test;
		}
		if (test2==5)
		{
			*connection_state=0;
		}
		return 0;//done
	}
	return 0;
}
/* //old
unsigned char ESP_CONNECT_AP(unsigned char *AP_NAME,unsigned char AP_LENGTH,unsigned char *PASS_NAME,unsigned char PASS_LENGTH, unsigned char *connection_state)
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
	*connection_state=1;
	if (test2==5)
	{
			test=ESP_CWJAP_CUR(AP_NAME,AP_LENGTH,PASS_NAME,PASS_LENGTH);
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
				*connection_state=0;
			}
			
	}
	return 0;//done
	
}*/

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

unsigned char ESP_READ_HTTP(unsigned char *CHANNEL_ID,unsigned char CHANNEL_ID_LENGTH,unsigned char FIELD_NUM,ESP_RECIVED_DATA_DT *DATA)
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
	test=ESP_SEND_READ_HTTP_DATA(CHANNEL_ID, CHANNEL_ID_LENGTH,FIELD_NUM,DATA);
	if (test)
	{
		return test;
	}
	return 0;//done
}
