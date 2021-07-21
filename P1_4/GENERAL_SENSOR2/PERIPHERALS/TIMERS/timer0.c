/*
 * timer.c
 *
 * Created: 26/10/2018 10:52:11 ص
 *  Author: ideapad510
 */ 
#include "timer0.h"

 #define round_factor 0.5
 #if 0
      volatile static unsigned long long clock_carry=0;
#endif
 /*extern volatile  unsigned long long clock_carry=0;

 ISR(BADISR_vect)
 {
 ;
 
 
 }

 ISR(TIMER0_OVF_vect)
 {
 clock_carry++;
 
 
 }
 TIMER0_ENABLE(TIMER_1024,OVER_FLOW_INT_ENABLE);
 dont change TCNT0_ACCESS or clock_carry */
  void TIMER0_ENABLE(T0_clock fre,timer0_mode mode,TIMER0_COMPARE_OUTPUT_MOD cmpmode,TIMER0_INT intmode, unsigned char oc)
{     OCRO_ACCESS=oc;
	  TCCR0_ACCESS=0x00;
	  TIMSK_ACCESS&=0xfC;
	  TCCR0_ACCESS|=fre|mode|cmpmode;
	  TIMSK_ACCESS |=intmode;
/*
	  if (intmode!=INT_DISABLE)
	  {global_interupt_enable();
	  }*/
   }
	   
	    void TIMER0_DISABLE()
		{
			  TCCR0_ACCESS=0x00;
		}
		
		
		void TIMER0_CTC_WAVE_GENERATE(float64 frequancy_oc0,float64 T_clock)
		{OCRO_ACCESS= ((T_clock/(2*frequancy_oc0))-1)+round_factor;
			 gpio_outputconfg(PORTB_DDRA_ADDRESS,PORTB_OUTMOD_ADDRESS,OUTPASS,3);
		}

void TIMER0_FREQUANCY_DIVIDER(uint8 scale)
{
	TIMER0_ENABLE(TIMER_T0_SOURCE_RISING,CTC_MOD,NON_INVERT_OC0_TOGGLE,INT_DISABLE, 0);
	OCRO_ACCESS=scale*0.5-1;
	gpio_outputconfg(PORTD_DDRA_ADDRESS,PORTD_OUTMOD_ADDRESS,OUTPASS,4);
	
	
}

void TIMER0_FAST_PWM_OC0(timer0_mode mode,TIMER0_COMPARE_OUTPUT_MOD CMP, float32 clockpersantage)
{
	switch(mode)
	{case PWM_FAST:
	        switch (CMP)
			{ 
				case PWM_FAST_OC0_NONINVERT :OCRO_ACCESS= (clockpersantage/100)*255 +round_factor;
				                             break;
				case PWM_FAST_OC0_INVERT :OCRO_ACCESS= ((100-clockpersantage)/100)*255 +round_factor;
				break;
				    default: return;
			}        
			break;    
	case PWM_PHASE_CORRECT_MOD: break;
	
	default:return;
	}
}

unsigned char TIMER0_READ_COUNTER()
{
		return TCNT0_ACCESS	  ;
}
		  
void TIMER0_SET_COUNTER(unsigned char X)
{
	TCNT0_ACCESS=X ;
}