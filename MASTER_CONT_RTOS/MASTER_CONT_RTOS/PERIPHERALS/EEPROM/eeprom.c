/*
 * relaymod.c
 *
 * Created: 11/09/2018 01:18:55 م
 *  Author: ideapad510
 */ 
#include "eeprom.h"

void writeeeprom(uint16 ADD,uint8 Data)
{
	if (readeeprom(ADD)!=Data)
	{
		while(isbitset(*EEPROM_CONTROL_ADD,1));  /*checking the last write is done*/
		*EEPROM_ADDRESS_HIGH_ADD=ADD>>8;
		*EEPROM_ADDRESS_LOW_ADD=ADD;
		*EEPROM_DATA_ADD=Data;
		setbit(*EEPROM_CONTROL_ADD,2);
		clearbit(*EEPROM_CONTROL_ADD,0);
		setbit(*EEPROM_CONTROL_ADD,1);
		while(isbitset(*EEPROM_CONTROL_ADD,1));
	}
}

uint8 readeeprom(uint16 ADD)
{while(isbitset(*EEPROM_CONTROL_ADD,1));  /*checking the last write is done*/
	*EEPROM_ADDRESS_HIGH_ADD=ADD>>8;
	*EEPROM_ADDRESS_LOW_ADD=ADD;
    clearbit(*EEPROM_CONTROL_ADD,2);
	setbit(*EEPROM_CONTROL_ADD,0);	
	
	return *EEPROM_DATA_ADD;
}
