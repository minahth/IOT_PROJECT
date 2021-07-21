/*
 * TIMER3.h
 *
 * Created: 06-Jul-21 3:13:39 PM
 *  Author: Mina Medhat
 */ 


#ifndef TIMER3_H_
#define TIMER3_H_

#include "microconfigrations.h"


typedef enum{  DISABLE_TIMER3_INT=0,
	           ENABLE_TIMER3_INT,
         }T3_INT_DATATYPE;
	
typedef enum{  T3_NORMAL,
	           T3_PWM_8_BIT,
			   T3_PWM_9_BIT,
			   T3_PWM_10_BIT,
			   T3_CTC_OCR3A,
			   T3_FAST_PWM_8_BIT,
			   T3_FAST_PWM_9_BIT,
			   T3_FAST_PWM_10_BIT,
			   T3_PWM_PHASE_FREQ_CORRECTION_ICR3,
			   T3_PWM_PHASE_FREQ_CORRECTION_OCR3A,
			   T3_PWM_PHASE_CORRECTION_ICR3,
			   T3_PWM_PHASE_CORRECTION_OCR3A,
			   T3_CTC_ICR3,
			   T3_FAST_PWM_ICR3=14,
			   T3_FAST_PWM_OCR3A,  
}T3_MODE_DATATYPE;



typedef enum { T3_TIMER_OFF=0x00,
	T3_TIMER_1=0x01,
	T3_TIMER_8=0x02,
	T3_TIMER_64=0x03,
	T3_TIMER_256=0x04,
	T3_TIMER_1024=0x05,
	T3_TIMER_T3_SOURCE_FALLING=0x06,
	T3_TIMER_T3_SOURCE_RISING=0x07,
	
}T3_clock;


typedef enum{  T3_OCA3_DISCONNECT,
	T3_OCA3_TOGGLE,
	T3_OCA3_CLEAR,
	T3_OCA3_SET,
	
	
	
}T3_OCA3_DATATYPE;

typedef enum{  T3_OCB3_DISCONNECT,
	T3_OCB3_TOGGLE,
	T3_OCB3_CLEAR,
	T3_OCB3_SET,
}T3_OCB3_DATATYPE;


typedef enum{  T3_OCC3_DISCONNECT,
	T3_OCC3_TOGGLE,
	T3_OCC3_CLEAR,
	T3_OCC3_SET,
}T3_OCC3_DATATYPE;
void TIMER3_DISABLE();
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
					T3_INT_DATATYPE TICIE3_state);

void TIMER3_ENABLE_NORMAL_MODE(T3_INT_DATATYPE overflowint ,T3_clock);
void TIMER3_ENABLE_CTC_OCRCA_MODE(T3_INT_DATATYPE COMPER_A_MATCHint ,T3_clock clk,T3_OCA3_DATATYPE oca_type,unsigned long f_ocna);
void TIMER3_ENABLE_FREQ_DIV_MODE(unsigned char divider);
void TIMER3_FAST_PWM(T3_MODE_DATATYPE WGM3,T3_INT_DATATYPE overflowint,T3_INT_DATATYPE OCA_INT,T3_INT_DATATYPE OCB_INT,T3_INT_DATATYPE INPUTCAP,T3_clock clk,T3_OCA3_DATATYPE oca_type,T3_OCB3_DATATYPE ocb_type,unsigned short oca,unsigned short ocb);
unsigned short TIMER3_READ_COUNTER();
void TIMER3_SET_COUNTER(unsigned short x);
unsigned short TIMER3_READ_OCR3A();
void TIMER3_SET_OCR3A(unsigned short x);
unsigned short TIMER3_READ_OCR3B();
void TIMER3_SET_OCR3B(unsigned short x);
unsigned short TIMER3_READ_OCR3C();
void TIMER3_SET_OCR3C(unsigned short x);
void T3_ENABLE_FOCA();
void T3_DISABLE_FOCA();
void T3_ENABLE_FOCB();
void T3_DISABLE_FOCB();



#endif /* TIMER3_H_ */