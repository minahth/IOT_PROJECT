/*
 * ESP_WIFI_SIMPLIFIED_DEFINE.h
 *
 * Created: 22-Jan-21 12:51:17 AM
 *  Author: ideapad510
 */ 


#ifndef ESP_WIFI_SIMPLIFIED_DEFINE_H_
#define ESP_WIFI_SIMPLIFIED_DEFINE_H_



#define ESP_UART_COUNT_RECIVER1 0x02dc6c00
/*#define ESP_UART_COUNT_RECIVER1 0x0212d9*/
#define ESP_UART_COUNT_RECIVER2 0xc6c00
#define MAX_CHAR_DATA 30

#define ESP_UART_SEND_ARRAY(ptr,size) (USART1_SEND_ARRAY((uint8*)ptr,size))
#define ESP_UART_SEND_BYTE(data) (USART1_SEND_byte(data))
#define ESP_UART_RECIVE_BYTE(uart_wait_count,vaildity) (USART1_RECEIVE_byte(vaildity,uart_wait_count))
#define ESP_UART_EMPTY_BUFFER() {\
									do \
									{\
										ESP_UART_RECIVE_BYTE(5,&V);\
									} while (V==1);\
								}



typedef struct{
				volatile unsigned char DATA_ELEMENT[MAX_CHAR_DATA];  /*DATA ENDED BY /n*/
				volatile unsigned char length;/*LENGTH OF DATA WITHOUT /n*/
				}ESP_RECIVED_DATA_DT;


#endif /* ESP_WIFI_SIMPLIFIED_DEFINE_H_ */