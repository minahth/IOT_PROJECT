/*
 * microconfigrations.h
 *
 * Created: 04/09/2018 11:01:52 ص
 *  Author: ideapad510
 */ 


#ifndef MICROCONFIGRATIONS_H_
#define MICROCONFIGRATIONS_H_

#define F_CPU (16000000u)
#include <util/delay.h>
#include <avr/interrupt.h>
#include "standerdtypes.h"
#include "macros.h"

/*#include "WATCH_DOG.h"*/

/********************************ATMEGA 128*************************/
#define global_interupt_enable() (setbit(MCU_STATE_REG_ACCESS,7))
#define global_interupt_disable() (clearbit(MCU_STATE_REG_ACCESS,7))
/**************THE I/O REGISTERS***************/
/*PORT  A*/

#define PORTA_DDRA_ADDRESS         ((volatile uint8*)0x3A)
#define PORTA_OUT_ADDRESS          ((volatile uint8*)0x3B)
#define PORTA_INMOD_ADDRESS        ((volatile uint8*)0x3B)
#define PORTA_IN_ADDRESS           ((volatile uint8*)0x39)
#define PORTA_OUTMOD_ADDRESS       ((volatile uint8*)0x39)


#define PORTA_DDRA_Access          (*PORTA_DDRA_ADDRESS  )
#define PORTA_OUT_Access           (*PORTA_OUT_ADDRESS   )
#define PORTA_INMOD_Access         (*PORTA_INMOD_ADDRESS )
#define PORTA_IN_Access            (*PORTA_IN_ADDRESS    )
#define PORTA_OUTMOD_Access        (*PORTA_OUTMOD_ADDRESS)

/*PORT  B*/
#define PORTB_DDRA_ADDRESS         ((volatile uint8*)0x37)
#define PORTB_OUT_ADDRESS          ((volatile uint8*)0x38)
#define PORTB_INMOD_ADDRESS        ((volatile uint8*)0x38)
#define PORTB_IN_ADDRESS           ((volatile uint8*)0x36)
#define PORTB_OUTMOD_ADDRESS       ((volatile uint8*)0x36)


#define PORTB_DDRA_Access          (*PORTB_DDRA_ADDRESS  )
#define PORTB_OUT_Access           (*PORTB_OUT_ADDRESS   )
#define PORTB_INMOD_Access         (*PORTB_INMOD_ADDRESS )
#define PORTB_IN_Access            (*PORTB_IN_ADDRESS    )
#define PORTB_OUTMOD_Access        (*PORTB_OUTMOD_ADDRESS)

/*PORT  C*/
#define PORTC_DDRA_ADDRESS         ((volatile uint8*)0x34)
#define PORTC_OUT_ADDRESS          ((volatile uint8*)0x35)
#define PORTC_INMOD_ADDRESS        ((volatile uint8*)0x35)
#define PORTC_IN_ADDRESS           ((volatile uint8*)0x33)
#define PORTC_OUTMOD_ADDRESS       ((volatile uint8*)0x33)


#define PORTC_DDRA_Access          (*PORTC_DDRA_ADDRESS  )
#define PORTC_OUT_Access           (*PORTC_OUT_ADDRESS   )
#define PORTC_INMOD_Access         (*PORTC_INMOD_ADDRESS )
#define PORTC_IN_Access            (*PORTC_IN_ADDRESS    )
#define PORTC_OUTMOD_Access        (*PORTC_OUTMOD_ADDRESS)

/*PORT  D*/
#define PORTD_DDRA_ADDRESS         ((volatile uint8*)0x31)
#define PORTD_OUT_ADDRESS          ((volatile uint8*)0x32)
#define PORTD_INMOD_ADDRESS        ((volatile uint8*)0x32)
#define PORTD_IN_ADDRESS           ((volatile uint8*)0x30)
#define PORTD_OUTMOD_ADDRESS       ((volatile uint8*)0x30)


#define PORTD_DDRA_Access          (*PORTD_DDRA_ADDRESS  )
#define PORTD_OUT_Access           (*PORTD_OUT_ADDRESS   )
#define PORTD_INMOD_Access         (*PORTD_INMOD_ADDRESS )
#define PORTD_IN_Access            (*PORTD_IN_ADDRESS    )
#define PORTD_OUTMOD_Access        (*PORTD_OUTMOD_ADDRESS)

/*PORT G 0->5 ONLY*/

#define PORTG_DDRA_ADDRESS         ((volatile uint8*)0x64)
#define PORTG_OUT_ADDRESS          ((volatile uint8*)0x65)
#define PORTG_INMOD_ADDRESS        ((volatile uint8*)0x65)
#define PORTG_IN_ADDRESS           ((volatile uint8*)0x63)
#define PORTG_OUTMOD_ADDRESS       ((volatile uint8*)0x63)


#define PORTG_DDRA_Access          (*PORTG_DDRA_ADDRESS  )
#define PORTG_OUT_Access           (*PORTG_OUT_ADDRESS   )
#define PORTG_INMOD_Access         (*PORTG_INMOD_ADDRESS )
#define PORTG_IN_Access            (*PORTG_IN_ADDRESS    )
#define PORTG_OUTMOD_Access        (*PORTG_OUTMOD_ADDRESS)

/*PORT F */

#define PORTF_DDRA_ADDRESS         ((volatile uint8*)0x61)
#define PORTF_OUT_ADDRESS          ((volatile uint8*)0x62)
#define PORTF_INMOD_ADDRESS        ((volatile uint8*)0x62)
#define PORTF_IN_ADDRESS           ((volatile uint8*)0x20)
#define PORTF_OUTMOD_ADDRESS       ((volatile uint8*)0x20)


#define PORTF_DDRA_Access          (*PORTF_DDRA_ADDRESS  )
#define PORTF_OUT_Access           (*PORTF_OUT_ADDRESS   )
#define PORTF_INMOD_Access         (*PORTF_INMOD_ADDRESS )
#define PORTF_IN_Access            (*PORTF_IN_ADDRESS    )
#define PORTF_OUTMOD_Access        (*PORTF_OUTMOD_ADDRESS)


/*PORT E */

#define PORTE_DDRA_ADDRESS         ((volatile uint8*)0x22)
#define PORTE_OUT_ADDRESS          ((volatile uint8*)0x23)
#define PORTE_INMOD_ADDRESS        ((volatile uint8*)0x23)
#define PORTE_IN_ADDRESS           ((volatile uint8*)0x21)
#define PORTE_OUTMOD_ADDRESS       ((volatile uint8*)0x21)


#define PORTE_DDRA_Access          (*PORTE_DDRA_ADDRESS  )
#define PORTE_OUT_Access           (*PORTE_OUT_ADDRESS   )
#define PORTE_INMOD_Access         (*PORTE_INMOD_ADDRESS )
#define PORTE_IN_Access            (*PORTE_IN_ADDRESS    )
#define PORTE_OUTMOD_Access        (*PORTE_OUTMOD_ADDRESS)








/****************************************************************/
/*TIMER0*/

#define TCCR0_ADDRESS ((volatile uint8 *)(0x53))
#define TCNT0_ADDRESS ((volatile uint8 *)(0x52))
#define OCRO_ADDRESS ((volatile uint8 *)(0x5c)) 


#define TCCR0_ACCESS (*TCCR0_ADDRESS)    /*timer0 control register*/
#define TCNT0_ACCESS (*TCNT0_ADDRESS)    /*register of count      */
#define OCRO_ACCESS  (*OCRO_ADDRESS)      /*compare register       */

#define DDR_OC0_ADDRESS PORTB_DDRA_ADDRESS
#define PORT_OUT_OC0_ADDRESS PORTB_OUT_ADDRESS
#define PORT_OUTMOD_OC0_ADDRESS PORTB_OUTMOD_ADDRESS
#define OC0_PIN 4




/*timer 1*/
#define TCCR1A_ADDRESS ((volatile uint8 *)(0x4f))
#define TCCR1A_ACCESS (*TCCR1A_ADDRESS) 

#define TCCR1B_ADDRESS ((volatile uint8 *)(0x4e))
#define TCCR1B_ACCESS (*TCCR1B_ADDRESS)

#define TCNT1H_ADDRESS ((volatile uint8 *)(0x4D))
#define TCNT1H_ACCESS (*TCNT1H_ADDRESS)

#define TCNT1L_ADDRESS ((volatile uint8 *)(0x4C))
#define TCNT1L_ACCESS (*TCNT1L_ADDRESS)

#define OCR1AH_ADDRESS ((volatile uint8 *)(0x4B))
#define OCR1AH_ACCESS (*OCR1AH_ADDRESS)

#define OCR1AL_ADDRESS ((volatile uint8 *)(0x4A))
#define OCR1AL_ACCESS (*OCR1AL_ADDRESS)

#define OCR1BH_ADDRESS ((volatile uint8 *)(0x49))
#define OCR1BH_ACCESS (*OCR1BH_ADDRESS)

#define OCR1BL_ADDRESS ((volatile uint8 *)(0x48))
#define OCR1BL_ACCESS (*OCR1BL_ADDRESS)


#define OCR1CH_ADDRESS ((volatile uint8 *)(0x79))
#define OCR1CH_ACCESS (*OCR1CH_ADDRESS)

#define OCR1CL_ADDRESS ((volatile uint8 *)(0x78))
#define OCR1CL_ACCESS (*OCR1CL_ADDRESS)

#define ICR1H_ADDRESS ((volatile uint8 *)(0x47))
#define ICR1H_ACCESS (*ICR1H_ADDRESS)

#define ICR1L_ADDRESS ((volatile uint8 *)(0x46))
#define ICR1L_ACCESS (*ICR1L_ADDRESS)


#define DDR_OC1A_ADDRESS PORTB_DDRA_ADDRESS
#define PORT_OUT_OC1A_ADDRESS PORTB_OUT_ADDRESS
#define PORT_OUTMOD_OC1A_ADDRESS PORTB_OUTMOD_ADDRESS
#define OC1A_PIN 5

#define DDR_OC1B_ADDRESS PORTB_DDRA_ADDRESS
#define PORT_OUT_OC1B_ADDRESS PORTB_OUT_ADDRESS
#define PORT_OUTMOD_OC1B_ADDRESS PORTB_OUTMOD_ADDRESS
#define OC1B_PIN 6

#define DDR_OC1C_ADDRESS PORTB_DDRA_ADDRESS
#define PORT_OUT_OC1C_ADDRESS PORTB_OUT_ADDRESS
#define PORT_OUTMOD_OC1C_ADDRESS PORTB_OUTMOD_ADDRESS
#define OC1C_PIN 7

#define DDR_T1_ADDRESS PORTD_DDRA_ADDRESS
#define PORT_IN_T1_ADDRESS PORTD_IN_ADDRESS
#define PORT_OUT_T1_ADDRESS PORTD_OUT_ADDRESS
#define PORT_OUTMOD_T1_ADDRESS PORTD_OUTMOD_ADDRESS
#define PORT_INMOD_T1_ADDRESS PORTD_INMOD_ADDRESS
#define T1_PIN 6
/*

#define DDR_ICP1_ADDRESS PORTD_DDRA_ADDRESS
#define PORT_IN_ICP1_ADDRESS PORTD_IN_ADDRESS
#define PORT_INMOD_ICP1_ADDRESS PORTD_INMOD_ADDRESS
#define ICP1_PIN 6*/




/*timer 2*/

#define TCCR2_ADDRESS ((volatile uint8 *)(0x45))
#define TCCR2_ACCESS (*TCCR2_ADDRESS)

#define TCNT2_ADDRESS ((volatile uint8 *)(0x44))
#define TCNT2_ACCESS (*TCNT2_ADDRESS)


#define OCR2_ADDRESS ((volatile uint8 *)(0x43))
#define OCR2_ACCESS (*OCR2_ADDRESS)

#define DDR_OC2_ADDRESS PORTD_DDRA_ADDRESS
#define PORT_OUT_OC2_ADDRESS PORTD_OUT_ADDRESS
#define PORT_OUTMOD_OC2_ADDRESS PORTD_OUTMOD_ADDRESS
#define OC2_PIN 7
/*timer3*/
#define DDR_OC3A_ADDRESS PORTE_DDRA_ADDRESS
#define DDR_OC3B_ADDRESS PORTE_DDRA_ADDRESS
#define DDR_OC3C_ADDRESS PORTE_DDRA_ADDRESS
#define PORT_OUTMOD_OC3A_ADDRESS PORTE_OUTMOD_ADDRESS
#define PORT_OUTMOD_OC3B_ADDRESS PORTE_OUTMOD_ADDRESS
#define PORT_OUTMOD_OC3C_ADDRESS PORTE_OUTMOD_ADDRESS
#define OC3A_PIN 3
#define OC3B_PIN 4
#define OC3C_PIN 5




/*spi*/
#define SPCR_ADDRESS ((volatile uint8 *)0x2d)
#define SPSR_ADDRESS ((volatile uint8 *)0x2e)
#define SPDR_ADDRESS ((volatile uint8 *)0x2F)

#define SPCR_ACCESS   (*SPCR_ADDRESS)
#define SPSR_ACCESS   (*SPSR_ADDRESS)
#define SPDR_ACCESS   (*SPDR_ADDRESS)


#define DDR_MISO_ADDRESS PORTB_DDRA_ADDRESS
#define PORT_OUT_MISO_ADDRESS PORTB_OUT_ADDRESS
#define PORT_IN_MISO_ADDRESS PORTB_IN_ADDRESS
#define PORT_INMOD_MISO_ADDRESS PORTB_INMOD_ADDRESS
#define PORT_OUTMOD_MISO_ADDRESS PORTB_OUTMOD_ADDRESS
#define MISO_PIN 3

#define DDR_MOSI_ADDRESS PORTB_DDRA_ADDRESS
#define PORT_OUT_MOSI_ADDRESS PORTB_OUT_ADDRESS
#define PORT_IN_MOSI_ADDRESS PORTB_IN_ADDRESS
#define PORT_INMOD_MOSI_ADDRESS PORTB_INMOD_ADDRESS
#define PORT_OUTMOD_MOSI_ADDRESS PORTB_OUTMOD_ADDRESS
#define MOSI_PIN 2

#define DDR_SCK_ADDRESS PORTB_DDRA_ADDRESS
#define PORT_OUT_SCK_ADDRESS PORTB_OUT_ADDRESS
#define PORT_IN_SCK_ADDRESS PORTB_IN_ADDRESS
#define PORT_INMOD_SCK_ADDRESS PORTB_INMOD_ADDRESS
#define PORT_OUTMOD_SCK_ADDRESS PORTB_OUTMOD_ADDRESS
#define SCK_PIN 1

#define DDR_SS_ADDRESS PORTB_DDRA_ADDRESS
#define PORT_OUT_SS_ADDRESS PORTB_OUT_ADDRESS
#define PORT_IN_SS_ADDRESS PORTB_IN_ADDRESS
#define PORT_INMOD_SS_ADDRESS PORTB_INMOD_ADDRESS
#define PORT_OUTMOD_SS_ADDRESS PORTB_OUTMOD_ADDRESS
#define SS_PIN 0







































/**********************************/
/* GENERAL REGISTERS */
#define  SFIOR_ADDRESS                          ((volatile uint8 *)(0x40)       )
 #define  SFIOR_ACCESS                          (*SFIOR_ADDRESS                 )
#define  MCU_CONTROL_REG_ADDRESS				((volatile uint8 *)(0x55  )     )
 #define  MCU_CONTROL_REG_ACCESS  				(*MCU_CONTROL_REG_ADDRESS       )
#define  MCU_CONTROL_STATE_REG_ADDRESS			((volatile uint8 *)(0x54)       )
 #define  MCU_CONTROL_STATE_REG_ACCESS		    (*MCU_CONTROL_STATE_REG_ADDRESS	)
#define  MCU_STATE_REG_ADDRESS					((volatile uint8 *)(0x5F  )     )/*SREG*/
 #define  MCU_STATE_REG_ACCESS					(*MCU_STATE_REG_ADDRESS         )
#define  TIMSK_ADDRESS							((volatile uint8 *)(0x57 )      )
 #define  TIMSK_ACCESS							(*TIMSK_ADDRESS                 )
#define  TIFR_ADDRESS							((volatile uint8 *)(0x56  )     )
 #define  TIFR_ACCESS							(*TIFR_ADDRESS                  ) 

  /* ENUMS */
typedef enum {
	           input        =0,
			   output       =1,
			   
			  }PIN_MOD;
			 
typedef enum {
	          FLOAT_mod     =0,
			  PULLUP_mod    =1,
			  
			  }INPUT_MOD;
			  
typedef enum {
	          LOW           =0,
	          HIGH          =1,
			  }IO_SIGNAL;
	
	
typedef enum {
	          OUTPASS       =0,
	          OUTBLOCK      =1,
			   }OUTPUTMOD;
	
	
	typedef enum {
		FALSE       =0,
		TRUE      =1,
	}STATE;
	
	/*FUNCTIONS PROTOTYPE*/
	
	void gpio_inputconfg(volatile uint8 * DiR_address,volatile uint8 * inputmodreg_address,INPUT_MOD mod,uint8 pin);
	void gpio_outputconfg(volatile uint8 * DiR_address,volatile uint8 * outputmodreg_address,OUTPUTMOD mod,uint8 pin);
	void DIGwrite(volatile uint8 * PORT_address,uint8 pin, IO_SIGNAL signal);
	IO_SIGNAL DIGread(volatile uint8 * PIN_address,uint8 pin);
	void I_O_PORTS_PULLUP_ENABLE();
	void I_O_PORTS_PULLUP_DISABLE();
	
#endif /* MICROCONFIGRATIONS_H_ */