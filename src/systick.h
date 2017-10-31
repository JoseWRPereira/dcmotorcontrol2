#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "tm4c123gh6pm.h"

void initSysTick( unsigned long baseTempo );
void SysTick_Handler( void );
void clearSysTick( void );
unsigned long readSysTick( void );
void clearSysTickB( void );
unsigned long readSysTickB( void );


#endif
