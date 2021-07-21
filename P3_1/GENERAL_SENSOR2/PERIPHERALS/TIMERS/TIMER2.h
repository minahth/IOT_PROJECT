/*
 * TIMER2.h
 *
 * Created: 30/03/2020 11:57:14 م
 *  Author: ideapad510
 */ 


#ifndef TIMER2_H_
#define TIMER2_H_
#include "microconfigrations.h"
typedef enum{
	T2_NORMAL,
	T2_PWM_PHASE_CORRECT=0x40,
	T2_CTC=0x08,
	T2_FAST_PWM=0x48,
	}WGM2_D;
typedef enum{  T2_OC2_DISCONNECT,
	T2_OC2_TOGGLE,
	T2_OC2_CLEAR,
	T2_OC2_SET,
}T2_OC2_DATATYPE;
typedef enum { T2_TIMER_OFF=0x00,
	T2_TIMER_1=0x01,
	T2_TIMER_8=0x02,
	T2_TIMER_32,
	T2_TIMER_64,
	T2_TIMER_128,
	T2_TIMER_256,
	T2_TIMER_1024,

	
}T2_clock;
	
typedef enum{  DISABLE_TIMER2_INT=0,
	ENABLE_TIMER2_INT,
}T2_INT_DATATYPE;	
	
	
	void TIMER2_DISABLE();
void TIMER2_ENABLE(WGM2_D MODE,T2_OC2_DATATYPE OC2_TYPE,T2_clock CLK_PRE,unsigned char as2_sync_mode /*1 or 0 default 0*/,T2_INT_DATATYPE out_compare_int,T2_INT_DATATYPE ovr_flow_int ,unsigned char ocr2 );
unsigned char TIMER2_READ_COUNTER();
void TIMER2_SET_COUNTER(unsigned char x);
unsigned char TIMER2_READ_OCR2();
void TIMER2_SET_OCR2(unsigned char x);


#endif /* TIMER2_H_ */