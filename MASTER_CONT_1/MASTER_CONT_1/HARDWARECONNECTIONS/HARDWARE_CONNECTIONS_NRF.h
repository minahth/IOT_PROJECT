/*
 * HARDWARE_CONNECTIONS_NRF.h
 *
 * Created: 30-Mar-21 6:24:53 PM
 *  Author: Mina Medhat
 */ 


#ifndef HARDWARE_CONNECTIONS_NRF_H_
#define HARDWARE_CONNECTIONS_NRF_H_

/*CE PIN FOR TX RX MODES*/
#define PORT_DDR_CE PORTB_DDRA_ADDRESS
#define PORT_OUTMOD_CE PORTB_OUTMOD_ADDRESS
#define PORT_OUT_CE PORTB_OUT_ADDRESS
#define CE_PIN 4
/*CSN FOR SPI*/
#define PORT_DDR_CSN PORTE_DDRA_ADDRESS
#define PORT_OUTMOD_CSN PORTE_OUTMOD_ADDRESS
#define PORT_OUT_CSN PORTE_OUT_ADDRESS
#define CSN_PIN 7
/*IRQ INTERRUPT*/
#define PORT_DDR_IRQ PORTD_DDRA_ADDRESS
#define PORT_INMOD_IRQ PORTD_INMOD_ADDRESS
#define PORT_IN_IRQ PORTD_IN_ADDRESS
#define IRQ_PIN 4


#endif /* HARDWARE_CONNECTIONS_NRF_H_ */