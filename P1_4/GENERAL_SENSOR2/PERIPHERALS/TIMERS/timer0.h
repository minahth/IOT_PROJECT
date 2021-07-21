/*
 * timer0.h
 *
 * Created: 26/10/2018 10:52:34 ص
 *  Author: ideapad510
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_
#include "microconfigrations.h"




 /* WGM01 , WGM00 ,BITS USED TO DETERMINED THE MODE OF TIMER0*/
 typedef enum{ NORMAL_MOD=0x00,
	           PWM_PHASE_CORRECT_MOD=0x40,
	           CTC_MOD=0x08,
			   PWM_FAST=0x48,
	          }timer0_mode;
typedef enum { OC0_DISCONNECTED           =0x00,
	           NON_INVERT_OC0_TOGGLE      =0x10,
			   NON_INVERT_OC0_CLEAR       =0x20,
			   NON_INVERT_OCO_SET         =0x30,
	           PWM_FAST_OC0_NONINVERT     =0x20,
	           PWM_FAST_OC0_INVERT        =0x30,
	           PWM_OC0_CLR_UP     =0x20,
	           PWM_OC0_SET_UP        =0x30,
	          }TIMER0_COMPARE_OUTPUT_MOD;

typedef enum { TIMER_OFF=0x00,
	           TIMER_1=0x01,
			   TIMER_8=0x02,
			   TIMER_64=0x03,
			   TIMER_256=0x04,
			   TIMER_1024=0x05,
			   TIMER_T0_SOURCE_FALLING=0x06,
			   TIMER_T0_SOURCE_RISING=0x07,
	
	          }T0_clock;
			  
			  /*OCIE0 , TOIE0 INTIMSK*/
			  typedef enum {INT_DISABLE=0,
				            OVER_FLOW_INT_ENABLE=1,
							OUT_CMP_INT_ENABLE=2,
							OUT_OVER_CTC_INT_ENABLE=3
           				    }TIMER0_INT;
			  
			   
			   void TIMER0_ENABLE(T0_clock fre,timer0_mode mode,TIMER0_COMPARE_OUTPUT_MOD cmpmode,TIMER0_INT intmode, unsigned char oc);
			   void TIMER0_DISABLE();
			    void TIMER0_CTC_WAVE_GENERATE(float64 frequancy_oc0,float64 T_clock);
			  void TIMER0_FREQUANCY_DIVIDER (uint8 scale);
			  void TIMER0_FAST_PWM_OC0(timer0_mode mode,TIMER0_COMPARE_OUTPUT_MOD CMP, float32 clockpersantage);
			  unsigned char TIMER0_READ_COUNTER();
			 void TIMER0_SET_COUNTER(unsigned char X);
			  
#endif /* TIMER0_H_ */