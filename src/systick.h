#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "tm4c123gh6pm.h"

void initSysTick( unsigned long baseTempo );
unsigned long sysTickRun( unsigned long runTime );
void SysTick_Handler( void );

#endif
