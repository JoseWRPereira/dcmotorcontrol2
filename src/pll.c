#include "tm4c123gh6pm.h" 
#include "pll.h"

// bus frequency is 400MHz/(SYSDIV2+1) = 400MHz/(4+1) = 80 MHz

void initPLL( void )
{
  SYSCTL_RCC2_R |=  SYSCTL_RCC2_USERCC2;	// Use RCC2 
  SYSCTL_RCC2_R |=  SYSCTL_RCC2_BYPASS2;	// SYSCLK = MOSC/SYSDIV2 
  SYSCTL_RCC_R  &= ~SYSCTL_RCC_XTAL_M;		// Clear Crystal Value
  SYSCTL_RCC_R  |=  SYSCTL_RCC_XTAL_16MHZ;	// Crystal Value 16MHz
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;	// Clear OSC Source
  SYSCTL_RCC2_R |=  SYSCTL_RCC2_OSCSRC2_MO;	// MOSC
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;		// PLL: Operating normally
  SYSCTL_RCC2_R |=  SYSCTL_RCC2_DIV400;		// Divide PLL 400MHz
  SYSCTL_RCC2_R = ( SYSCTL_RCC2_R & 
		    ~(SYSCTL_RCC2_SYSDIV2_M|SYSCTL_RCC2_SYSDIV2LSB) ) | 
	 	  (SYSDIV2<<(SYSCTL_RCC2_SYSDIV2_S-1));// see pll.h

       // wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS)==0)
	;

  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;	// SYSCLK = PLL/SYSDIV2
}


void busSpeedPLL( void )		// MainOSC 16MHz
{
  SYSCTL_RCC_R = 0x00000000;
  SYSCTL_RCC_R =  (unsigned long)
  (
	SYSCTL_RCC_XTAL_16MHZ 		|	// 16 MHz
	//SYSCTL_RCC_MOSCDIS 		|     	// Main Oscillator Disable
	SYSCTL_RCC_OSCSRC_MAIN 		| 	// MOSC
	SYSCTL_RCC_PWRDN 		|       // PLL Power Down
	SYSCTL_RCC_BYPASS 		|      	// PLL Bypass
	//SYSCTL_RCC_USESYSDIV 		|    	// Enable System Clock Divider
	(0x15<<SYSCTL_RCC_SYSDIV_S)  		// System Divisor: n << Bit Number : 1 <= n <= 16
  );
}


