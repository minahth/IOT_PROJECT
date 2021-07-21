/*
 * RANDOM_NUM_GEN.c
 *
 * Created: 31-Mar-21 10:38:30 PM
 *  Author: Mina Medhat
 */ 
#include "RANDOM_NUM_GEN.h"
volatile unsigned long RANDOM_NUMBER=1; 
unsigned short GENNERATE_RANDOM_NUMBER()
{
	   RANDOM_NUMBER=(RANDOM_NUMBER+0x44444*((unsigned short)TIMER0_READ_COUNTER()+1))%(0xffff);
	   return (unsigned short)RANDOM_NUMBER;
}
