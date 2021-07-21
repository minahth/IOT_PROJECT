/*
 * TIMER3.c
 *
 * Created: 06-Jul-21 3:13:31 PM
 *  Author: Mina Medhat
 */ 
#include "TIMER3.h"
#if 0
volatile static unsigned long long clock_carry_3=0;
#endif
/*extern volatile  unsigned long long clock_carry_3=0;

 ISR(BADISR_vect)
 {
 ;
 
 
 }

 ISR(TIMER3_OVF_vect)
 {
 clock_carry_1++;        happens when  timer3 reach 00 
 
 
 }
 TIMER3_ENABLE_NORMAL_MODE(ENABLE_TIMER3_INT,TIMER_1024);
 dont change TCNT3H or clock_carry_3 */















/*

void TIMER3_ENABLE_NORMAL_MODE(T3_INT_DATATYPE overflowint,T3_clock clk_pre)
{
	TIMER3_ENABLE(T3_OCA3_DISCONNECT,T3_OCB3_DISCONNECT,T3_NORMAL,clk_pre,0,0,overflowint,DISABLE_TIMER3_INT,DISABLE_TIMER3_INT,DISABLE_TIMER3_INT);
}*/

unsigned short TIMER3_READ_COUNTER()
{
	volatile unsigned short x= TCNT3L;
	x|=TCNT3H<<8;
	return x;
}

void TIMER3_SET_COUNTER(unsigned short x)
{
	TCNT3H=x>>8;
	TCNT3L=0x00ff&x;
}


void TIMER3_DISABLE()
{    TIMER3_SET_COUNTER(0);
	TCCR3A=00;
	TCCR3B=00;
}

void TIMER3_ENABLE(T3_OCA3_DATATYPE COM3A,
                   T3_OCB3_DATATYPE COM3B,
				   T3_OCC3_DATATYPE COM3C,
				   T3_MODE_DATATYPE WGM3, 
				   T3_clock CS3, 
				   unsigned short OCR3A_DATA, 
				   unsigned short OCR3B_DATA,
				    unsigned short OCR3C_DATA,
				   T3_INT_DATATYPE TOIE3_state,
				   T3_INT_DATATYPE OCIE3C_state,  
				   T3_INT_DATATYPE OCIE3B_state, 
				   T3_INT_DATATYPE OCIE3A_state, 
				   
				   T3_INT_DATATYPE TICIE3_state)
{
	TCCR3A=00;
	TCCR3B=00;
	TCCR3A|=COM3A<<6;
	TCCR3A|=COM3B<<4;
	TCCR3A|=COM3C<<2;
	TCCR3A|=WGM3&0x03;
	TCCR3B|=(WGM3&0x0c)<<1;
    OCR3AH=OCR3A_DATA>>8;
	OCR3AL=OCR3A_DATA;
	OCR3BH=OCR3B_DATA>>8;
	OCR3BL=OCR3B_DATA;
	OCR3CH=OCR3C_DATA>>8;
	OCR3CL=OCR3C_DATA;
	
	unsigned char x=(OCIE3C_state<<1)|(TOIE3_state<<2)|(OCIE3B_state<<3)|(OCIE3A_state<<4)|(TICIE3_state<<5);
	ETIMSK&=0b00000001;
	ETIMSK|=x;
	TCCR3B|=CS3;
	if (COM3A!=T3_OCA3_DISCONNECT)
	{gpio_outputconfg(DDR_OC3A_ADDRESS,PORT_OUTMOD_OC3A_ADDRESS,OUTPASS,OC3A_PIN);
	}
	if (COM3B!=T3_OCB3_DISCONNECT)
	{gpio_outputconfg(DDR_OC3B_ADDRESS,PORT_OUTMOD_OC3B_ADDRESS,OUTPASS,OC3B_PIN);
	}
	if (COM3C!=T3_OCC3_DISCONNECT)
	{gpio_outputconfg(DDR_OC3C_ADDRESS,PORT_OUTMOD_OC3C_ADDRESS,OUTPASS,OC3C_PIN);
	}
	
}

unsigned short TIMER3_READ_OCR3A()
{
	 
	volatile unsigned short  x3= OCR3AL;
	  x3|=OCR3AH<<8;
	  return x3;
}

void TIMER3_SET_OCR3A(unsigned short x)
{
	 OCR3AH=x>>8;
	 OCR3AL=x;
}

void TIMER3_SET_OCR3B(unsigned short x)
{
	OCR3BH=x>>8;
	OCR3BL=x;
}




unsigned short TIMER3_READ_OCR3B()
{
	  unsigned short x= OCR3BL;
	  x|=OCR3BH<<8;
	  return x;
}

void TIMER3_SET_OCR3C(unsigned short x)
{
	OCR3CH=x>>8;
	OCR3CL=x;
}




unsigned short TIMER3_READ_OCR3C()
{
	unsigned short x= OCR3CL;
	x|=OCR3CH<<8;
	return x;
}
