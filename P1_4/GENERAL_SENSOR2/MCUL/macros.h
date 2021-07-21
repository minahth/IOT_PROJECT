/*
 * macros.h
 *
 * Created: 04/09/2018 10:47:09 ص
 *  Author: ideapad510
 */ 


#ifndef MACRO_H_
#define MACRO_H_
#define setbit(reg,num) (reg|=(1<<num))
#define clearbit(reg,num) (reg&=~(1<<num))
#define toggybit(reg,num) (reg^=(1<<num))
#define isbitclear(reg,num) !(reg&(1<<num))
#define isbitset(reg,num) (reg&(1<<num))
#endif /* MACROS_H_ */