/*
 * uart.h
 *
 * Created: 16/11/2018 01:47:23 م
 *  Author: ideapad510
 */ 


#ifndef UART_H_
#define UART_H_
#include "microconfigrations.h"
#include "macros.h"

#define uart_max_count 50000u
 

#define UDR_ADDRESS    ((volatile uint8*)0x2c)
#define UCSRA_ADDRESS  ((volatile uint8*)0x2B)
#define UCSRB_ADDRESS  ((volatile uint8*)0x2A)
#define UCSRC_ADDRESS  ((volatile uint8*)0x40)
#define UBRRH_ADDRESS  ((volatile uint8*)0x40)
#define UBRRL_ADDRESS  ((volatile uint8*)0x29)

#define UDR_Access     (*UDR_ADDRESS)
#define UCSRA_Access   (*UCSRA_ADDRESS)
#define UCSRB_Access   (*UCSRB_ADDRESS)
#define UCSRC_Access   (*UCSRC_ADDRESS)
#define UBRRH_Access   (*UBRRH_ADDRESS)
#define UBRRL_Access   (*UBRRL_ADDRESS)

#define _MASTER_ (1)/*in sync mode*/

#define RXC_ 7  /*ucsra intrupts flags*/
#define  TXC 6
#define UDRE_ 5






#define mult_mcus_comm_mode (0) /*0>>disable 1>>enable  multiprosscer communication*/

typedef enum {  ASYNCHRONOUS_NORMAL=0x00,  /*at first UCSRC &=10111110 UCSRA &=11111101 */
	 ASYNCHRONOUS_DOUBLE=0x200,
	 SYNC_sample_FALLING=0x41,
	  SYNC_sample_RISING=0x40,
	 }usart_mode;
typedef enum {  
	INT_disable =0x00,
	TXC_INT_ENABLE=0x40, /*UCSRB &=10011111*/
	RXE_INT_ENABLE=0x80,
	UDRE_INT_ENABLE=0x20,
	RXE_TXC_INT_ENABLE=0xC0,
	RXE_UDRE_INT_ENABLE=0xa0,
	UDRE_TXC_INT_ENABLE=0x60,
	RXE_TXC_UDRE_INT_ENABLE=0xE0,
}usart_int;
typedef enum {  DISABLE=0x00, /*ucsrb &=11100111*/
	TX_ENABLE=0X08,
	RX_ENABLE=0x10,
	TX_RX_ENABLE=0x18,
}USART_ENABLE;
typedef enum
{ PARITY_DISABLE_1STOP=0x00, /*ucsrc=11000111*/
	PARITY_EVEN_1STOP=0x20,
	PARITY_DISABLE_2STOP=0x08,
	PARITY_EVEN_2STOP=0x28,
	PARITY_ODD_1STOP=0x30,
	PARITY_ODD_2STOP=0x38,
	}USART_CONTORL_FRAME; /*STOP BIT PARITY*/
typedef enum /*ucsrc =11111001 uscrb =11111011*/
{ DATA_5=0x00,
	DATA_6=0x2,
	DATA_7=0x4,
	DATA_8=0x6,
	DATA_9=0x406,
	}USART_DATA_FRAME; 
typedef enum
{   USART_BAUD_100=100,
	USART_BAUD_200=200,
	USART_BAUD_300=300,
	USART_BAUD_1200=1200,
	USART_BAUD_2400=2400,
	USART_BAUD_4800=4800,
	USART_BAUD_9600=9600,
	USART_BAUD_14400=14400,
	USART_BAUD_19200=19200,
	USART_BAUD_28800=28800,
	USART_BAUD_38400=38400,
	USART_BAUD_57600=57600,
	USART_BAUD_76800=76800,
	USART_BAUD_115200=115200,
	USART_BAUD_230400=230400,
	USART_BAUD_250k=250000,
	USART_BAUD_500K=500000,
	USART_BAUD_1M=1000000,
	}USART_BAUDE_RATES;
	void USART_INIT(usart_mode mode_of_sync,usart_int intrupts,USART_ENABLE usarttype_TX_RX,USART_CONTORL_FRAME controlframe,USART_DATA_FRAME dataframe,USART_BAUDE_RATES buade_rate);
	void USART_SEND_BYTE(uint8 data);
	uint8 USART_RECV_BYTE(unsigned long uart_wait_count, volatile unsigned char *vaildity);
	void USART_SEND_ARRAY(uint8* ptr,uint8 size);
void USART_SEND_str(char* ptr);
void USART_RECV_ARRAY(uint8*ptr);
#endif /* UART_H_ */