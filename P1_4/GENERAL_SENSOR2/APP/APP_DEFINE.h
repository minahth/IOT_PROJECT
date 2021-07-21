/*
 * APP_DEFINE.h
 *
 * Created: 25-Apr-21 1:58:45 PM
 *  Author: Mina Medhat
 */ 


#ifndef APP_DEFINE_H_
#define APP_DEFINE_H_

#define LDR_ADC_PIN ADC0
#define HEAT_ADC_PIN ADC2

typedef enum{
	HEAT_SENSOR,
	LDR_SENSOR,
}ADC_APP_SELECTOR_DT;

#endif /* APP_DEFINE_H_ */