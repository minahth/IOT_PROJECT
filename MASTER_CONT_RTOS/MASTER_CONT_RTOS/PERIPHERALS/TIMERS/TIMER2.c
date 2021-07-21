/*
 * TIMER2.c
 *
 * Created: 30/03/2020 11:57:31 م
 *  Author: ideapad510
 */ 
#include "TIMER2.h"
void TIMER2_DISABLE()
{   TCNT2=0;
	TCCR2=00;
}

void TIMER2_ENABLE(WGM2_D MODE,T2_OC2_DATATYPE OC2_TYPE,T2_clock CLK_PRE,unsigned char as2_sync_mode /*1 or 0 default 0*/,T2_INT_DATATYPE out_compare_int,T2_INT_DATATYPE ovr_flow_int ,unsigned char ocr2)
{
	TCCR2_ACCESS=00;
	TCCR2_ACCESS|=MODE;
	TCCR2_ACCESS|=OC2_TYPE<<4;
	TCCR2_ACCESS|=CLK_PRE;
	OCR2_ACCESS=ocr2;
	clearbit(ASSR,3);
	ASSR|=as2_sync_mode<<3;
	TIMSK_ACCESS&=0x3f;
	TIMSK_ACCESS|=out_compare_int<<7;
	TIMSK_ACCESS|=ovr_flow_int<<6;
	if (OC2_TYPE!=T2_OC2_DISCONNECT)
	{gpio_outputconfg(DDR_OC2_ADDRESS,PORT_OUTMOD_OC2_ADDRESS,OUTPASS,OC2_PIN);
	}	
}

unsigned char TIMER2_READ_COUNTER()
{
	return TCNT2_ACCESS;
}

void TIMER2_SET_COUNTER(unsigned char x)
{
	TCNT2_ACCESS=x;
}

unsigned char TIMER2_READ_OCR2()
{
	return OCR2_ACCESS;
}

void TIMER2_SET_OCR2(unsigned char x)
{
	OCR2_ACCESS=x;
}
