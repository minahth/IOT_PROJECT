/*
 * adcmodule.h
 *
 * Created: 19/10/2018 11:08:04 ص
 *  Author: ideapad510
 */ 


#ifndef ADCMODULE_H_
#define ADCMODULE_H_
#include "microconfigrations.h"
/*REGISTERS*/ 
#define ADCL_ADDRESS            ((volatile uint8 *)(0x24))
#define ADCL_ACCESS             (*ADCL_ADDRESS           )
#define ADCH_ADDRESS            ((volatile uint8 *)(0x25))
#define ADCH_ACCESS             (*ADCH_ADDRESS           )
#define ADMUX_ADDRESS           ((volatile uint8*)(0x27) )
#define ADC_CTRL_STATUS_ADDRESS ((volatile uint8 *)(0x26))
#define ADMUX_ACCESS            (*ADMUX_ADDRESS          )
#define ADC_CTRL_STATUS_ACCESS  (*ADC_CTRL_STATUS_ADDRESS)


#define ADC_10_BIT              ((ADCL)|(ADCH<<8))
#define ADC_8_BIT                ADCH



typedef enum {_8_BIT,
	          _10_BIT,
              }ADC_RESOLUTION;
typedef enum { ADC_AREF=0x00,
	           ADC_AVCC= 0x40,
			   ADC_2_56v=0xc0,
	          }ADC_VOLT_REF;
			  
typedef enum {left=0x20,
	          right=0x00
			        }ADC_ADJUST;
					
typedef enum {
	                    ADC0,
						ADC1,
						ADC2,
						ADC3,
						ADC4,
						ADC5,
						ADC6,
						ADC7,
						DEF_PADC0_NADC0_200=0xA,
						DEF_PADC1_NADC0_200,
						DEF_PADC2_NADC2_10,
						DEF_PADC3_NADC2_10,
						DEF_PADC2_NADC2_200,
						DEF_PADC3_NADC2_200,
						DEF_PADC0_NADC1_1,
						
						/*CONTINUE DATASHEET PAGE 273K;*/
						VOLT_BAND_GAP =0X1E,
						VOLT_GROUND=0X1F
						 }ADC_CHANNEL;
					/*END OF REGISTER ADMUX*/
					
					
						

               /*ADITE BIT*/						
						typedef enum{ /*law bit adite b zero yeb2a 5alas law b 1 ha7ot l byte l tanya ll sfior*/
							   SINGLE_MODE=0X00,
							   AUTO_TRIGGER_FREE_RUNNING=0x0020,
							   AUTO_TRIGGER_ANALOG_COMAPRATOR=0x2020,
							   AUTO_TRIGGER_EXT_INT_REQ=0x4020,
							   AUTO_TRIGGER_TIMER0_COMPARE_MATCH=0x6020,
                               AUTO_TRIGGER_TIMER0_COMPARE_OVER_FLOW=0x8020,
                               AUTO_TRIGGER_TIMER1_COMPARE_MATCH_B=0xA020,
                               AUTO_TRIGGER_TIMER1_COMPARE_OVER_FLOW=0xC020,
                               AUTO_TRIGGER_TIMER1_CAPTURE_EVENT=0xE020,
						         }ADC_MODE;
				/*ADIE*/
						typedef enum {
							      ADC_INTERRUPT_DISABLE=0X00,
								  ADC_INTERRUPT_ENABLE=0X08
							          }ADC_INTERRUPT_MODE;
									
									typedef enum
									{ ADC_DIVISION_2=0x01,
										 ADC_DIVISION_4,
										 ADC_DIVISION_8,
										 ADC_DIVISION_16,
										 ADC_DIVISION_32,
										 ADC_DIVISION_64,
										 ADC_DIVISION_128,
										 }ADC_PRESCALE;  
						
						
				void ADC_INIT (ADC_PRESCALE P,ADC_VOLT_REF VF,ADC_MODE MODE ,ADC_INTERRUPT_MODE INT_STATE,ADC_ADJUST ADC_DATA_ORDER);
				void ADC_DISABLE();
				uint16 ADC_READ(ADC_CHANNEL channel,ADC_RESOLUTION resolution);		
				void ADC_START_READ_INT(ADC_CHANNEL channel);			
						
#endif /* ADCMODULE_H_ */