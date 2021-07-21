/*
 * UART_2P.c
 *
 * Created: 09/04/2020 09:13:10 م
 *  Author: ideapad510
 */ 
#include "UART_2P.h"
unsigned char USART0_RECEIVE_byte(volatile unsigned char*V,unsigned long long COUNTER)
{
volatile	unsigned long i=0;
	while(isbitclear(UCSR0A,RXC0))
	{   ++i;
		if (COUNTER==i)
		{
			*V=0;
			return 0;
		}
	}
	*V=1;
	return UDR0;
}

unsigned char USART1_RECEIVE_byte(volatile unsigned char*V,unsigned long COUNTER)
{
volatile	 unsigned long i=0;
	while(isbitclear(UCSR1A,RXC1))
	{   ++i;
		if (COUNTER==i)
		{
			*V=0;
			return 0;
		}
	}
	
	*V=1;
	return UDR1;
}

unsigned char USART0_SEND_byte(unsigned char DATA)
{ volatile  unsigned long i=0;
	/*if(DATA==0)
	{
		USART0_SEND_byte('d');
	}*/
	UDR0=DATA;
	while(isbitclear(UCSR0A,UDRE0))
	{   ++i;
		if (i==max_count_uart0)
		{
			return 0;
		}
	}	
	return 1;
	
}
unsigned char USART1_SEND_byte(unsigned char DATA)
{  volatile unsigned long i=0;
	UDR1=DATA;
	while(isbitclear(UCSR1A,UDRE1))
	{++i;
		if (i==max_count_uart1)
		{
			return 0;
		}
	}
	return 1;
	
}

void USART0_INIT(USART_MODE mode_of_sync,USART_INT_MODE TX_INT,USART_INT_MODE RX_INT,USART_INT_MODE UDRIE_INT/*NORMALY OFF*/,USART_ENABLE usarttype_TX_RX,USART_CONTORL_FRAME controlframe,USART_DATA_FRAME dataframe,USART_BAUDE_RATES buade_rate)
{
	uint16 ubrr_=0;
	gpio_outputconfg(PORTE_DDRA_ADDRESS,PORTE_OUTMOD_ADDRESS,OUTPASS,1);
	gpio_inputconfg(PORTE_DDRA_ADDRESS,PORTE_INMOD_ADDRESS,FLOAT_mod,0);
	UCSR0B=0;
	UCSR0C=0x80|mode_of_sync|controlframe|dataframe;
	UCSR0B|=(RX_INT<<7)|(TX_INT<<6)|(UDRIE_INT<<5)|(usarttype_TX_RX)|(dataframe>>8);
	UCSR0A|=(mode_of_sync>>8);
	  switch (mode_of_sync)
	  {   case ASYNCHRONOUS_NORMAL:ubrr_= (F_CPU/(16UL*buade_rate)-1);
		  break;
		  case ASYNCHRONOUS_DOUBLE:ubrr_= (F_CPU/(8UL*buade_rate)-1);
		  break;
		  
		  case SYNC_sample_RISING:
		  case SYNC_sample_FALLING:
		  ubrr_= (F_CPU/(2.0*buade_rate)-1);
		  break;
		  
	  } 
	  UBRR0L=0xff & ubrr_;
	  UBRR0H=0x0f & (ubrr_>>8);
	  
	  
}

void USART1_INIT(USART_MODE mode_of_sync,USART_INT_MODE TX_INT,USART_INT_MODE RX_INT,USART_INT_MODE UDRIE_INT/*NORMALY OFF*/,USART_ENABLE usarttype_TX_RX,USART_CONTORL_FRAME controlframe,USART_DATA_FRAME dataframe,USART_BAUDE_RATES buade_rate)
{   
	uint16 ubrr_=0;
	gpio_outputconfg(PORTD_DDRA_ADDRESS,PORTD_OUTMOD_ADDRESS,OUTPASS,3);
	gpio_inputconfg(PORTD_DDRA_ADDRESS,PORTD_INMOD_ADDRESS,FLOAT_mod,2);
	UCSR1B=0;
	UCSR1C=0x80|mode_of_sync|controlframe|dataframe;
	UCSR1B|=(RX_INT<<7)|(TX_INT<<6)|(UDRIE_INT<<5)|(usarttype_TX_RX)|(dataframe>>8);
    UCSR1A|=(mode_of_sync>>8);	
	  switch (mode_of_sync)
	  {case ASYNCHRONOUS_NORMAL:ubrr_= (F_CPU/(16UL*buade_rate)-1);
		  break;
		  case ASYNCHRONOUS_DOUBLE:ubrr_= (F_CPU/(8UL*buade_rate)-1);
		  break;
		  
		  case SYNC_sample_RISING:
		  case SYNC_sample_FALLING:
		  ubrr_= (F_CPU/(2.0*buade_rate)-1);
		  break;
		  
	  }
	  UBRR1L=0xff & ubrr_;
	  UBRR1H=0x0f & (ubrr_>>8);
	  
}
void USART0_DISABLE()
{
	UCSR0B=0;
	UCSR0C=0;
}
void USART1_DISABLE()
{
	UCSR1B=0;
	UCSR1C=0;
}

unsigned char USART0_SEND_str(char *DATA)
{ volatile unsigned short i=0;
	while(DATA[i]!='\0')
	{  
		if (USART0_SEND_byte(DATA[i])==0)
		{
			return 0;
		}
		++i;
	}
	DATA[i]=ending_char_0;
	if (USART0_SEND_byte(DATA[i])==0)
	{
		return 0;
	}
	return 1;
}

unsigned char USART1_SEND_str(unsigned char *DATA)
{ volatile unsigned short i=0;
	while(DATA[i]!='\0')
	{
		if (USART1_SEND_byte(DATA[i])==0)
		{
			return 0;
		}
		++i;
	}
	DATA[i]=ending_char_1;
	if (USART1_SEND_byte(DATA[i])==0)
	{
		return 0;
	}
	return 1;
}

unsigned char USART0_SEND_ARRAY(unsigned char *DATA,unsigned short N)
{
	unsigned short i=0;
	for(i=0;i<N;i++)
	{
		if(USART0_SEND_byte(DATA[i])!=1)
		{
			return 0;
		}
	
	}
	
return 1;
}

unsigned char USART1_SEND_ARRAY(unsigned char *DATA,unsigned short N)
{
	unsigned short i=0;
	for(i=0;i<N;i++)
	{
		if(USART1_SEND_byte(DATA[i])!=1)
		{
			return 0;
		}
		
	}
	
	return 1;
}
