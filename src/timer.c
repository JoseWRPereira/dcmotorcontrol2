#include "timer.h"


void (*ptrTimer)(void);


void IntT1A_Handler( void )
{
  TIMER1_ICR_R = TIMER_ICR_TATOCINT; 
}

unsigned long readT1A( void )
{
  return( TIMER1_TAV_R );
}

void resetT1A( void )
{
  TIMER1_TAV_R = 1;
}

void IntT1B_Handler( void )
{
  TIMER1_ICR_R = TIMER_ICR_TBTOCINT; 
}

void IntT2A_Handler( void )
{
  TIMER2_ICR_R = TIMER_ICR_TATOCINT; 
  // 10 ms -> 100Hz
  ptrTimer();
} 

void IntT2B_Handler( void )
{
  TIMER2_ICR_R = TIMER_ICR_TBTOCINT;
}


void initTimer( unsigned long tAquis, void (*fTimer)(void) )
{
  SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
  // 1: Ensure the timer is disable
  TIMER1_CTL_R &= ((~TIMER_CTL_TAEN) & (~TIMER_CTL_TBEN));
  // 2: Write the GPTMCFG = 0
  TIMER1_CFG_R = TIMER_CFG_32_BIT_TIMER;
  // 3: Cfg TnMR: OneShot/Periodic
  TIMER1_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;
  // 4: Optionally config.
  TIMER1_SYNC_R |= TIMER_SYNC_SYNCT1_TA;  
  // 5: Load the start value into the INTERVAL LOAD REG
  TIMER1_TAILR_R = 0xFFFFFFFF;//time-2;
  // 6: If interrupt are required, mask interrupt
  TIMER1_IMR_R = TIMER_IMR_TATOIM | TIMER_IMR_TBTOIM;
  // 7: Enable timer and start counting
  TIMER1_CTL_R |= TIMER_CTL_TAEN;
  // 8: Aguarda interrupcao ou polling

  NVIC_PRI5_R	= (NVIC_PRI5_R & 0xFFFF00FF)|0x00004000;
  NVIC_EN0_R	= 0x00200000; // Timer1A



  SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R2;
  // 1: Ensure the timer is disable
  TIMER2_CTL_R &= ((~TIMER_CTL_TAEN) & (~TIMER_CTL_TBEN));
  // 2: Write the GPTMCFG = 0
  TIMER2_CFG_R = TIMER_CFG_32_BIT_TIMER;
  // 3: Cfg TnMR: OneShot/Periodic
  TIMER2_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;
  // 4: Optionally config.
  TIMER2_SYNC_R |= TIMER_SYNC_SYNCT2_TA;  
  // 5: Load the start value into the INTERVAL LOAD REG
  TIMER2_TAILR_R = tAquis-2;
  // 6: If interrupt are required, mask interrupt
  TIMER2_IMR_R = TIMER_IMR_TATOIM | TIMER_IMR_TBTOIM;
  // 7: Enable timer and start counting
  TIMER2_CTL_R |= TIMER_CTL_TAEN;
  // 8: Aguarda interrupcao ou polling

  NVIC_PRI5_R	= (NVIC_PRI5_R & 0x00FFFFFF)|0x40000000;
  NVIC_EN0_R	= 0x00800000; // Timer2A

  ptrTimer = fTimer;
}



