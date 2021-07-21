/*
 * relaymod.h
 *
 * Created: 11/09/2018 01:19:13 م
 *  Author: ideapad510
 */ 


#ifndef EEprom_
#define EEprom_
#include "microconfigrations.h"
#define EEPROM_ADDRESS_HIGH_ADD ((volatile uint8*)0x3F)
#define EEPROM_ADDRESS_LOW_ADD ((volatile uint8*)0x3E)
#define EEPROM_DATA_ADD ((volatile uint8*)0x3D)
#define EEPROM_CONTROL_ADD ((volatile uint8*)0x3C) 

void writeeeprom(uint16 ADD,uint8 Data);
uint8 readeeprom(uint16 ADD);


#endif /* RELAYMOD_H_ */