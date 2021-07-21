/*
 * ESP_WIFI_SIMPLIFIED_DEFINE.h
 *
 * Created: 22-Jan-21 12:51:17 AM
 *  Author: ideapad510
 */ 


#ifndef ESP_WIFI_SIMPLIFIED_DEFINE_H_
#define ESP_WIFI_SIMPLIFIED_DEFINE_H_



#define ESP_UART_COUNT_RECIVER1 2000U //2000 milli sec
#define ESP_UART_COUNT_RECIVER2 0xc6c00
#define MAX_CHAR_DATA 30




#define ESP_UART_EMPTY_BUFFER() {volatile unsigned char v;\
									do\
									{\
									ESP_UART_RECIVE_BYTE(&v,1);\
									} while(v);\
								}

/*
#define ESP_UART_EMPTY_BUFFER() {\
									do\
									{\
									UDR1;\
									} while(pdPASS==xSemaphoreTake(RX_ESP_SEMAPHORE,pdMS_TO_TICKS(1)));\
								}*/



typedef struct{
				volatile unsigned char DATA_ELEMENT[MAX_CHAR_DATA];  /*DATA ENDED BY /n*/
				volatile unsigned char length;/*LENGTH OF DATA WITHOUT /n*/
				}ESP_RECIVED_DATA_DT;


#endif /* ESP_WIFI_SIMPLIFIED_DEFINE_H_ */