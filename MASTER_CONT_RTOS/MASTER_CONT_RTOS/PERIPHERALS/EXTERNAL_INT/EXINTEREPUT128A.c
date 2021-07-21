/*
 * EXINTEREPUT128A.c
 *
 * Created: 09/04/2020 08:39:00 م
 *  Author: ideapad510
 */ 
#include "EXINTEREPUT128A.h"

void external_int_enable(INT_ENABLE_NUMBER INT_NUM,INT_POLARTY POLARTY)
{
	global_interupt_disable();
	EIMSK|=INT_NUM;
	switch(INT_NUM)
	{   case INT0_ENABLE:EICRA&=0xfc;
		                 EICRA|=POLARTY;
						 break;
		case INT1_ENABLE:EICRA&=0xf3;
						 EICRA|=POLARTY<<2;
					     break;
		
		case INT2_ENABLE:EICRA&=0xcf;
						 EICRA|=POLARTY<<4;
						 break;
		case INT3_ENABLE:EICRA&=0x3f;
					     EICRA|=POLARTY<<6;
						 break;
		case INT4_ENABLE:EICRB&=0xfc;
						 EICRB|=POLARTY;
						 break;
		case INT5_ENABLE:EICRB&=0xf3;
						 EICRB|=POLARTY<<2;
						 break;
		
		case INT6_ENABLE:EICRB&=0xcf;
						 EICRB|=POLARTY<<4;
						 break;
		case INT7_ENABLE:EICRB&=0x3f;
						 EICRB|=POLARTY<<6;
						 break;
	}
	global_interupt_enable();
}

void external_int_disable(INT_DISABLE_NUMBER INT_NUM)
{
	global_interupt_disable();
	EIMSK&=(~INT_NUM);
	/*global_interupt_enable();*/
}
