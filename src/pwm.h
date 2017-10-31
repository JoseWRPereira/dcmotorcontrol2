#ifndef __PWM_H__
#define __PWM_H__

#include "tm4c123gh6pm.h"
#include "pll.h"

void setPWM( unsigned int dutyc );
void strPWM( void );
void stpPWM( void );
void IntPWM0_Handler( void );
                        //freq={200..50000}  dutyc={1 .. 999}
void initPWM( unsigned int freq, unsigned int dutyc );



#endif

