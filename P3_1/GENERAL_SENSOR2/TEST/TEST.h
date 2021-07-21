/*
 * TEST.h
 *
 * Created: 09-Apr-21 4:48:30 PM
 *  Author: Mina Medhat
 */ 


#ifndef TEST_H_
#define TEST_H_
#include "TEST_HARDWARE.h"
#include "microconfigrations.h"

#define TEST1_SET() (setbit(*PORT_OUT_TEST_1,TEST_1_PIN))
#define TEST2_SET() (setbit(*PORT_OUT_TEST_2,TEST_2_PIN))
#define TEST3_SET() (setbit(*PORT_OUT_TEST_3,TEST_3_PIN))

#define TEST1_CLEAR() (clearbit(*PORT_OUT_TEST_1,TEST_1_PIN))
#define TEST2_CLEAR() (clearbit(*PORT_OUT_TEST_2,TEST_2_PIN))
#define TEST3_CLEAR() (clearbit(*PORT_OUT_TEST_3,TEST_3_PIN))

#define TEST1_TOGGLE() (toggybit(*PORT_OUT_TEST_1,TEST_1_PIN))
#define TEST2_TOGGLE() (toggybit(*PORT_OUT_TEST_2,TEST_2_PIN))
#define TEST3_TOGGLE() (toggybit(*PORT_OUT_TEST_3,TEST_3_PIN))

void TEST_IO_SETUP();
void TEST_SETUP();
void TEST_UART_SEND_VALUE(long long X);


#endif /* TEST_H_ */