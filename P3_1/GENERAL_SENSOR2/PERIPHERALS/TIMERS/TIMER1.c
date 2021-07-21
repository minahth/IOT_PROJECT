/*
 * TIMER1.c
 *
 * Created: 30/03/2020 11:56:50 م
 *  Author: ideapad510
 */ 
#include "TIMER1.h"
#if 1
volatile static unsigned long long clock_carry_1=0;
#endif
/*extern volatile  unsigned long long clock_carry_1=0;

 ISR(BADISR_vect)
 {
 ;
 
 
 }

 ISR(TIMER1_OVF_vect)
 {
 clock_carry_1++;        happens when  timer1 reach 00 
 
 
 }
 TIMER1_ENABLE_NORMAL_MODE(ENABLE_TIMER1_INT,TIMER_1024);
 dont change TCNT1H_ACCESS or clock_carry_1 */
















void TIMER1_ENABLE_NORMAL_MODE(T1_INT_DATATYPE overflowint,T1_clock clk_pre)
{
	TIMER1_ENABLE(T1_OCA1_DISCONNECT,T1_OCB1_DISCONNECT,T1_NORMAL,clk_pre,0,0,overflowint,DISABLE_TIMER1_INT,DISABLE_TIMER1_INT,DISABLE_TIMER1_INT);
}

unsigned short TIMER1_READ_COUNTER()
{
	unsigned short x= TCNT1L_ACCESS;
	x|=TCNT1H_ACCESS<<8;
	return x;
}

void TIMER1_SET_COUNTER(unsigned short x)
{
	TCNT1H_ACCESS=x>>8;
	TCNT1L_ACCESS=0x00ff&x;
}




void TIMER1_ENABLE_CTC_OCRCA_MODE(T1_INT_DATATYPE COMPER_A_MATCHint ,T1_clock clk_pre,T1_OCA1_DATATYPE oca_type,unsigned long f_ocna)
{
		unsigned short oc;
	switch(clk_pre)
	{case T1_TIMER_1: oc=F_CPU/(2*1*f_ocna)-1;
		break;
		case T1_TIMER_8: oc=F_CPU/(2*8*f_ocna)-1;
		break;
		case T1_TIMER_64: oc=F_CPU/(2*64*f_ocna)-1;
		break;
		case T1_TIMER_256: oc=F_CPU/(2*256*f_ocna)-1;
		break;
		case T1_TIMER_1024: oc=F_CPU/(2*1024*f_ocna)-1;
		break;
		default: oc=0xffff;
		
	}
	
	TIMER1_ENABLE(oca_type,T1_OCB1_DISCONNECT,T1_CTC_OCR1A,clk_pre,oc,0,DISABLE_TIMER1_INT,DISABLE_TIMER1_INT,COMPER_A_MATCHint,DISABLE_TIMER1_INT);
}


void TIMER1_ENABLE_FREQ_DIV_MODE(unsigned char divider)
{
	unsigned short oc;
	oc=divider/2-1;
	TIMER1_ENABLE(T1_OCA1_TOGGLE,T1_OCB1_DISCONNECT,T1_CTC_OCR1A,T1_TIMER_T1_SOURCE_RISING,oc,0,DISABLE_TIMER1_INT,DISABLE_TIMER1_INT,ENABLE_TIMER1_INT,DISABLE_TIMER1_INT);
    gpio_inputconfg(DDR_T1_ADDRESS,PORT_INMOD_T1_ADDRESS,FLOAT_mod,T1_PIN);
}

/*

void TIMER1_FAST_PWM(T1_INT_DATATYPE overflowint,T1_INT_DATATYPE OCA_INT,T1_INT_DATATYPE OCB_INT,T1_INT_DATATYPE INPUTCAP,T1_clock clk,T1_OCA1_DATATYPE oca_type,T1_OCB1_DATATYPE ocb_type,unsigned short oca,unsigned short ocb)
{
	TCCR1A_ACCESS=00;
	TCCR1B_ACCESS=00;
	TCCR1A_ACCESS|=oca_type<<6;
	TCCR1A_ACCESS|=ocb_type<<4;
	TCCR1A_ACCESS|=mode & 0x03;
	TCCR1B_ACCESS|=(mode & 0x0c)<<1;
	TCCR1B_ACCESS|=clk_pre;
	
	
	  OCR1AH_ACCESS=oca>>8;
	  OCR1AL_ACCESS=oca;
	  OCR1BH_ACCESS=ocb>>8;
	  OCR1BL_ACCESS=ocb;
	
	
	
	
	
	
	
	
	
	
	
	
	if (overflowint==ENABLE_TIMER1_INT)
	{
		setbit(TIMSK_ACCESS,2);
	}
	else
	{
		clearbit(TIMSK_ACCESS,2);
	}
	if (OCA_INT==ENABLE_TIMER1_INT)
	{
		setbit(TIMSK_ACCESS,4);
	}
	else
	{
		clearbit(TIMSK_ACCESS,4);
	}
	if (OCB_INT==ENABLE_TIMER1_INT)
	{
		setbit(TIMSK_ACCESS,3);
	}
	else
	{
		clearbit(TIMSK_ACCESS,3);
	}
	if (INPUTCAP==ENABLE_TIMER1_INT)
	{
		setbit(TIMSK_ACCESS,5);
	}
	else
	{
		clearbit(TIMSK_ACCESS,5);
	}
	
	
	
	
	
	
	
}*/

void TIMER1_ENABLE(T1_OCA1_DATATYPE COM1A,
                   T1_OCB1_DATATYPE COM1B,
				   T1_MODE_DATATYPE WGM1, 
				   T1_clock CS1, 
				   unsigned short OCR1A_DATA, 
				   unsigned short OCR1B_DATA, 
				   T1_INT_DATATYPE TOIE1_state, 
				   T1_INT_DATATYPE OCIE1B_state, 
				   T1_INT_DATATYPE OCIE1A_state, 
				   T1_INT_DATATYPE TICIE1_state)
{
	TCCR1A_ACCESS=00;
	TCCR1B_ACCESS=00;
	TCCR1A_ACCESS|=COM1A<<6;
	TCCR1A_ACCESS|=COM1B<<4;
	TCCR1A_ACCESS|=WGM1&0x03;
	TCCR1B_ACCESS|=(WGM1&0x0c)<<1;
    OCR1AH_ACCESS=OCR1A_DATA>>8;
	OCR1AL_ACCESS=OCR1A_DATA;
	OCR1BH_ACCESS=OCR1B_DATA>>8;
	OCR1BL_ACCESS=OCR1B_DATA;
	TIMSK_ACCESS&=0xc3;
	unsigned char x=TOIE1_state|(OCIE1B_state<<1)|(OCIE1A_state<<2)|(TICIE1_state<<3);
	TIMSK_ACCESS|=x<<2;
	TCCR1B_ACCESS|=CS1;
	if (COM1A!=T1_OCA1_DISCONNECT)
	{gpio_outputconfg(DDR_OC1A_ADDRESS,PORT_OUTMOD_OC1A_ADDRESS,OUTPASS,OC1A_PIN);
	}
	if (COM1B!=T1_OCB1_DISCONNECT)
	{gpio_outputconfg(DDR_OC1B_ADDRESS,PORT_OUTMOD_OC1B_ADDRESS,OUTPASS,OC1B_PIN);
	}
	/**/
	TCCR1A_ACCESS|=0x08;
	/**/
}

unsigned short TIMER1_READ_OCR1A()
{
	 
	volatile unsigned short  x1= OCR1AL_ACCESS;
	  x1|=OCR1AH_ACCESS<<8;
	  
	  
	  return x1;
}

void TIMER1_SET_OCR1A(unsigned short x)
{
	 OCR1AH_ACCESS=x>>8;
	 OCR1AL_ACCESS=x;
}

void TIMER1_SET_OCR1B(unsigned short x)
{
	OCR1BH_ACCESS=x>>8;
	OCR1BL_ACCESS=x;
}

unsigned short TIMER1_READ_OCR1B()
{
	  unsigned short x= OCR1BL_ACCESS;
	  x|=OCR1BH_ACCESS<<8;
	  return x;
}

void T1_ENABLE_FOCA()
{
	setbit(TCCR1A_ACCESS,3);
}
void T1_ENABLE_FOCB()
{
	setbit(TCCR1A_ACCESS,2);
}

void T1_DISABLE_FOCA()
{
	clearbit(TCCR1A_ACCESS,3);
}

void T1_DISABLE_FOCB()
{
	clearbit(TCCR1A_ACCESS,2);
	
}

void TIMER1_FAST_PWM(T1_MODE_DATATYPE mode,T1_INT_DATATYPE overflowint,T1_INT_DATATYPE OCA_INT,T1_INT_DATATYPE OCB_INT,T1_INT_DATATYPE INPUTCAP,T1_clock clk,T1_OCA1_DATATYPE oca_type,T1_OCB1_DATATYPE ocb_type,unsigned short oca,unsigned short ocb)
{
	TIMER1_ENABLE(oca_type,ocb_type,mode,clk,oca,ocb,overflowint,OCB_INT,OCA_INT,INPUTCAP);
}

void TIMER1_DISABLE()
{
	TCCR1A_ACCESS=00;
	TCCR1B_ACCESS=00;
	TIMER1_SET_COUNTER(0);
}
