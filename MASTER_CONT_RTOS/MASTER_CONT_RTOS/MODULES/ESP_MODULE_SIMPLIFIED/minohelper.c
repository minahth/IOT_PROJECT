/*
 * minohelper.c
 *
 * Created: 9/1/2020 12:42:54 PM
 *  Author: ideapad510
 */ 
#include "minohelper.h"
unsigned char inttostring(unsigned char * the_return,long long thenumber)
{
	unsigned char counter=0;
	unsigned long long the_scaler=10000000000000;/*13 digit*/
	if (thenumber<0)
	{
		the_return[0]='-';
		thenumber=thenumber*(-1);
		counter=1;
	}
	if (thenumber==0)
	{
		the_return[0]='0';
		return 1;
	}
	while (thenumber/the_scaler==0)
	{
		the_scaler=the_scaler/10;
	}
	
	do 
	{
			the_return[counter]=(thenumber/the_scaler)+48;
			counter++;
			thenumber=thenumber%the_scaler;
			if (the_scaler==1)
			{
				break;
			}
			the_scaler=the_scaler/10;
	} while (1);

	
	return counter;
}

unsigned long long STRINGTOINT(unsigned char * the_return,unsigned char size)
{
	unsigned char i=0;
	unsigned long long result=0;
	for (i=0;i<size;i++)
	{
		result*=10;
		result+=(the_return[i]-48);
	}
	return result;
}