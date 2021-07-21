/*
 * spi.c
 *
 * Created: 23/11/2018 11:41:18 ص
 *  Author: ENG MINA MEDHAT
 */ 

#include "spi.h"
 volatile  SPI_DATA data;
void SPI_INT(SPI_OPMODE opmode, SPI_PRESCALE prescale,SPI_MODE mode, SPI_INTURUPT intupt,SPI_DATA_ORDER order)
{	switch (opmode)
	{
		case SPI_MASTER: gpio_outputconfg(DDR_MOSI_ADDRESS,PORT_OUTMOD_MOSI_ADDRESS,OUTPASS,MOSI_PIN);
		                 gpio_outputconfg(DDR_SS_ADDRESS,PORT_OUTMOD_SS_ADDRESS,OUTPASS,SS_PIN);
						 gpio_outputconfg(DDR_SCK_ADDRESS,PORT_OUTMOD_SCK_ADDRESS,OUTPASS,SCK_PIN);
                        clearbit(*PORT_OUT_SCK_ADDRESS,SCK_PIN);
						SPSR_ACCESS &=0xFE;
						SPSR_ACCESS |=(prescale>>8);
						SPCR_ACCESS=opmode|prescale|mode|intupt|order|0X40;
						
						 break;	                 
		
		case SPI_SLAVE: gpio_outputconfg(DDR_MISO_ADDRESS,PORT_OUTMOD_MISO_ADDRESS,OUTPASS,MISO_PIN);
		              SPSR_ACCESS &=0xFE;
					  SPCR_ACCESS=opmode|prescale|mode|intupt|order|0X40;
					  
					   break;  
	}
	

	

/*	setbit(*PORT_OUT_SS_ADDRESS,SS_PIN);*/
/*	setbit(*PORT_OUT_SCK_ADDRESS,SCK_PIN);*/
}



void SPI_EXCHANGE_PULL(volatile SPI_DATA *data)
{ 
	SPDR_ACCESS=data->data_send;
	
	while(isbitclear(SPSR_ACCESS,7));
	
	data->data_recive=SPDR_ACCESS;
}

void SPI_SEND_INTR(volatile SPI_DATA *data)
{
	SPDR_ACCESS=data->data_send;
	clearbit(*PORT_OUT_SS_ADDRESS,SS_PIN);
	
}
void SPI_RECV_INTR(volatile SPI_DATA *data)
{   
	data->data_recive=SPDR_ACCESS;
}

