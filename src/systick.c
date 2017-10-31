#include "systick.h"

unsigned long stRunA = 0;
unsigned long stRunB = 0;
void initSysTick( unsigned long baseTempo )
{
  stRunA = stRunB = 0;
  NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_INTEN|NVIC_ST_CTRL_ENABLE);
  NVIC_ST_RELOAD_R = baseTempo-1;
  NVIC_ST_CURRENT_R = baseTempo;
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & ~NVIC_SYS_PRI3_TICK_M)|		\
			(0x2<<NVIC_SYS_PRI3_TICK_S); // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R|=(NVIC_ST_CTRL_CLK_SRC|NVIC_ST_CTRL_INTEN|NVIC_ST_CTRL_ENABLE);
}

void SysTick_Handler( void )
{
  ++stRunA;
  ++stRunB;
}

void clearSysTick( void )
{
  stRunA = 0;
}
unsigned long readSysTick( void )
{
  return( stRunA );
}

void clearSysTickB( void )
{
  stRunB = 0;
}
unsigned long readSysTickB( void )
{
  return( stRunB );
}




