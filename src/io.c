#include "io.h"




void PortF_Init(void)	                // PortF: MSB xxxIOOOI LSB
{ 
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020; 	// 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           	// allow time for clock to start
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; 	// 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           	// allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        	// 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   	// 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          	// 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        	// 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          	// enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          	// 7) enable digital I/O on PF4-0
}

void PortE_Init(void)			// PortE: MSB xxxxxxII LSB
{ 
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;     	// 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;           	// allow time for clock to start
  GPIO_PORTE_LOCK_R = GPIO_LOCK_KEY;   	// 2) unlock GPIO Port E
  GPIO_PORTE_CR_R = 0x1F;           	// allow changes to PE4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTE_AMSEL_R = 0x00;        	// 3) disable analog on PF
  GPIO_PORTE_PCTL_R = 0x00000000;   	// 4) PCTL GPIO on PE4-0
  GPIO_PORTE_DIR_R = 0x00;          	// 5) PE7-0 in
  GPIO_PORTE_AFSEL_R = 0x00;        	// 6) disable alt funct on PE7-0
  GPIO_PORTE_PDR_R = 0x03;          	// enable pull-down on PE0 and PE1
  GPIO_PORTE_DEN_R = 0x03;          	// 7) enable digital I/O on PE1-0
}

void PortD_Init(void)	       		// PortD: 
{ 
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000008;     	// 1) activate clock for Port D
  delay = SYSCTL_RCGC2_R;           	// allow time for clock to start
  GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;   	// 2) unlock GPIO Port D
  GPIO_PORTD_CR_R = 0x1F;           	// allow changes to PD4-0
  GPIO_PORTD_AMSEL_R = 0x00;        	// 3) disable analog on PD
  GPIO_PORTD_PCTL_R = 0x00000000;   	// 4) PCTL GPIO on PD4-0
  GPIO_PORTD_DIR_R = 0x08;          	// 5) PD3 out/in One-Wire
  GPIO_PORTD_AFSEL_R = 0x00;        	// 6) disable alt funct on PD7-0
  GPIO_PORTD_PUR_R = 0x08;          	// enable pull-down on PD3
  GPIO_PORTD_DEN_R = 0x08;		// 7) enable digital I/O on PD3
}

void PortC_Init(void)			// PortC: 
{ 
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000004;    	// 1) activate clock for Port C
  delay = SYSCTL_RCGC2_R;          	// allow time for clock to start
  GPIO_PORTC_CR_R = 0x20; 		// allow changes to PC7-4 OBS.: PC3-0 JTAG/SWD pins 
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTC_AMSEL_R = 0x00;        	// 3) disable analog on PC
  GPIO_PORTC_PCTL_R &= 0xFF0FFFFF;   	// 4) PCTL GPIO on PC4-0
  GPIO_PORTC_DIR_R |= 0x20;          	// 5) PC5 pin EN Bluetooth
  GPIO_PORTC_AFSEL_R &= ~0x20;        	// 6) disable alt funct on PC7-0
  GPIO_PORTC_DEN_R |= 0x20;          	// 7) enable digital I/O on PC5
  // Table 10.7 GPIO Pins With Special Consideration
}

void (*ptrSW1)(void);
void (*ptrSW2)(void);

void initSWLEDS( void (*fSW1)(void), void (*fSW2)(void) )
{ 
  volatile unsigned long delay;
  SYSCTL_RCGCGPIO_R	|= SYSCTL_RCGC2_GPIOF;
  SYSCTL_RCGC2_R 	|= SYSCTL_RCGC2_GPIOF;
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R	= GPIO_LOCK_KEY;	// Unlock PF[0]  
  GPIO_PORTF_CR_R	|=  0x1F;		// Allow GPIO port F
  GPIO_PORTF_DIR_R	&= ~0x11;		// Set Input
  GPIO_PORTF_DIR_R	|=  0x0E;		// Set Output
  GPIO_PORTF_AFSEL_R	&= ~0x1F;		// GPIO, Alt function off
  GPIO_PORTF_PCTL_R	&= ~GPIO_PCTL_PF4_M;	// PF4 mask
  GPIO_PORTF_PUR_R	|=  0x11;		// Set Pull-Up 
  GPIO_PORTF_DEN_R	|=  0x1F;		// Digital Enable
  GPIO_PORTF_IM_R	&= ~0x11;		// Interrupt Mask
  GPIO_PORTF_IBE_R 	&=  0x11;		// Interrupt Both Edge
  GPIO_PORTF_IEV_R	|=  0x11; 		// Set Rising Edge
  GPIO_PORTF_IS_R	&= ~0x11;		// Interrupt Sense	
  GPIO_PORTF_IM_R       |=  0x11;		// Interrupt Mask
  GPIO_PORTF_ICR_R	|=  0x11;		// Clear Interrupt Reg

  NVIC_PRI7_R		 = (NVIC_PRI7_R & 0xFF00FFFF)|0x00400000;
  NVIC_EN0_R		 = 0x40000000; 

  ptrSW1 = fSW1;
  ptrSW2 = fSW2;
}

void GPIOPortF_Handler(void)
{ 
  if(GPIO_PORTF_RIS_R&0x01)
  {  // SW2 touch
    GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag0
    ptrSW2();
  }
  if(GPIO_PORTF_RIS_R&0x10)
  {  // SW1 touch
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4
    ptrSW1();
  }
}

