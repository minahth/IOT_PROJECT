/*
 * NRF24L01.h
 *
 * Created: 20-Jan-21 7:36:13 PM
 *  Author: ideapad510
 */ 


#ifndef NRF24L01_H_
#define NRF24L01_H_
#include "microconfigrations.h"
#include "HARDWARE_CONNECTIONS.h"
#include "NRF24L01_DEFINE.h"
#include "NRF24L01_CONFIG.h"

#define NRF_SPI_CLEAR_PIN() (clearbit(*PORT_OUT_CSN,CSN_PIN))
#define NRF_SPI_SET_PIN()	(setbit(*PORT_OUT_CSN,CSN_PIN))

#define NRF_CLEAR_CE() (clearbit(*PORT_OUT_CE,CE_PIN))
#define NRF_SET_CE()	(setbit(*PORT_OUT_CE,CE_PIN))

#define NRF_IS_IRQ_SET() (isbitset(*PORT_IN_IRQ,IRQ_PIN))
#define NRF_IS_IRQ_CLEAR() (isbitclear(*PORT_IN_IRQ,IRQ_PIN))


/*HELPING FUNCTIONS*/
volatile unsigned char NRF_SPI_EXCHANGE(unsigned char TX_DATA);
void NRF_WRITE_REGISTER(unsigned char REGISTER_NUM,unsigned char DATA);
unsigned char NRF_READ_REGISTER(unsigned char REGISTER_NUM);

/*COMMAND EXCUTER*/
void NRF_R_RX_PAYLOAD_DYNAMIC(unsigned char *DATA,unsigned char LENGTH);
void NRF_W_TX_PAYLOAD_ACK(unsigned char *DATA,unsigned char LENGTH);
void NRF_W_TX_PAYLOAD_NOACK(unsigned char *DATA,unsigned char LENGTH);
void NRF_FLUSH_TX_BUFFER();
void NRF_FLUSH_RX_BUFFER();
void NRF_RETX_PL();
unsigned char NRF_READ_LENGTH_RX_PL();
unsigned char NRF_READ_STATUS_NOW();
unsigned char NRF_READ_RPD();
unsigned char NRF_READ_RX_DR_AND_CLEAR();
unsigned char NRF_READ_TX_DS_AND_CLEAR();
unsigned char NRF_READ_MAX_RT_AND_CLEAR();
void NRF_SET_TX_MODE();
void NRF_SET_RX_MODE();

void NRF_UPDATE_PIPE_RX_ADDRESS(unsigned char NUMBER,volatile unsigned char *ADDRESS,unsigned char LENGTH);
void NRF_UPDATE_TX_ADDRESS(unsigned char *ADDRESS,unsigned char LENGTH);
void NRF_UPDATE_USED_CHANNEL(NRF_RF_CHANNEL_DT channel);
void NRF_UPDATE_RET_PL(NRF_RETRANSMIT_COUNT_DT RET);

/*..ETC*/



/*****************/


void NRF_IO_SETUP();
void NRF_SETUP_USED_CONFIG();
unsigned char NRF_SEND_DATA_WITH_ACK(unsigned char *ADDRESS,unsigned char ADDRESS_LENGTH,unsigned char *DATA,unsigned char DATA_LENGTH);
void NRF_START_RX_MODE();
void NRF_STOP_RX_MODE();
unsigned char NRF_CHECK_RX_PL(unsigned char *DATA,unsigned char *LENGTH);














#endif /* NRF24L01_H_ */