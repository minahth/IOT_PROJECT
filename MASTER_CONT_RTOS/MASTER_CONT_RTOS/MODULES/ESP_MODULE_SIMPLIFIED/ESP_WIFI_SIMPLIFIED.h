/*
 * ESP_WIFI_SIMPLIFIED.h
 *
 * Created: 22-Jan-21 12:22:24 AM
 *  Author: ideapad510
 */ 


#ifndef ESP_WIFI_SIMPLIFIED_H_
#define ESP_WIFI_SIMPLIFIED_H_
#include "UART_2P.h"
#include "microconfigrations.h"
#include "ESP_WIFI_SIMPLIFIED_DEFINE.h"
#include "minohelper.h"


unsigned char ESP_READ_CHAR_AND_CHECK(unsigned char X);


unsigned char ESP_CHECK_OK_RESP();

void ESP_UART_SEND_ARRAY(char *ptr,unsigned char size);
void ESP_UART_SEND_BYTE(char data); 
unsigned char ESP_UART_RECIVE_BYTE(volatile unsigned char*V,unsigned short COUNTER);





unsigned char ESP_AT_COMMAND();
unsigned char ESP_ATE_COMMAND();
unsigned char ESP_CWMODE_1();
unsigned char ESP_CWLAPOPT_1();
unsigned char ESP_CIPSTATUS(unsigned char *RETURN_VALUE);
unsigned char ESP_CWJAP_CUR(unsigned char *AP_NAME,unsigned char AP_LENGTH,unsigned char *PASS_NAME,unsigned char PASS_LENGTH);
unsigned char ESP_OPEN_SOCKET(); /*SIMPLIFIED*/
unsigned char ESP_SEND_WRITE_HTTP_DATA(unsigned char *API_WRITE_KEY,unsigned char API_WRITE_KEY_LENGTH,unsigned char FIELD_NUM,unsigned char *DATA,unsigned char DATA_LENGTH);




/*USED FUNCTIONS*/
unsigned char ESP_SETUP();
unsigned char ESP_SHOW_AP_LIST(unsigned char *AP_NAMES,unsigned short* LENGTH);
unsigned char ESP_CONNECT_AP(unsigned char *AP_NAME,unsigned char AP_LENGTH,unsigned char *PASS_NAME,unsigned char PASS_LENGTH, unsigned char *connection_state);
unsigned char ESP_WRITE_HTTP(unsigned char *API_WRITE_KEY,unsigned char API_WRITE_KEY_LENGTH,unsigned char FIELD_NUM,unsigned char *DATA,unsigned char DATA_LENGTH);
unsigned char ESP_READ_HTTP(unsigned char *CHANNEL_ID,unsigned char CHANNEL_ID_LENGTH,unsigned char FIELD_NUM,ESP_RECIVED_DATA_DT *DATA);




void RX_ESP_ISR();
void TX_ESP_ISR();

#endif /* ESP_WIFI_SIMPLIFIED_H_ */