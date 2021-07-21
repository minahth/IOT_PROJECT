/*
 * spi.h
 *
 * Created: 23/11/2018 11:41:59 ص
 *  Author: ideapad510
 */ 


#ifndef SPI_H_
#define SPI_H_
#include "microconfigrations.h"
/*#include "interuptatmega32a.h"*/


typedef enum
{  SPI_DISABLE,
	SPI_FREE,
	SPI_BUSY_TX,
	SPI_BUSY_RX,
	
	
	}SPI_STATUS;



typedef struct {  volatile uint8 data_send;
	              volatile uint8 data_recive;
	              SPI_STATUS STATUS;
	              }volatile SPI_DATA;


typedef enum{ SPI_INT_ENABLE=0X80 ,
	          SPI_INT_DISABLE=0x00,
			  }SPI_INTURUPT;
 typedef enum{ SPI_MSF ,
	 SPI_LSF=0x20,
 }SPI_DATA_ORDER;
 
 typedef enum{ SPI_MODE_0=0X00 ,
	 SPI_SAMPLE_RISING=0x00,
	 SPI_MODE_1=0x04 ,
	 SPI_SETUP_RISING=0x04,
	 SPI_MODE_2=0x08 ,
	 SPI_SAMPLE_FALLING=0x08,
	 SPI_MODE_3= 0x0c,
	 SPI_SETUP_FALLING=0x0c,
	 
 }SPI_MODE;
 typedef enum{ 
	 SPI_SCALE_4=0x00,
	 SPI_SCALE_16=0x01,
	 SPI_SCALE_64=0x02,
	 SPI_SCALE_128=0x03,
	 SPI_SCALE_2=0x100,
	 SPI_SCALE_8=0x101,
	 SPI_SCALE_32=0x102,
 }SPI_PRESCALE;
  typedef enum{
	 SPI_MASTER=0x10,
	 SPI_SLAVE=0x00,
  }SPI_OPMODE;
  
  extern  volatile  SPI_DATA data;
	void SPI_INT(SPI_OPMODE opmode, SPI_PRESCALE prescale,SPI_MODE mode, SPI_INTURUPT intupt,SPI_DATA_ORDER order);
    void SPI_EXCHANGE_PULL(volatile SPI_DATA *data);
	
	void SPI_SEND_INTR(volatile SPI_DATA *data);
	void SPI_RECV_INTR(volatile SPI_DATA *data);


#endif /* SPI_H_ */