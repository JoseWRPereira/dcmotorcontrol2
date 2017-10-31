#ifndef __IO_H__
#define __IO_H__

#include "tm4c123gh6pm.h"

// IOs/PORTs
#define PORTA                GPIO_PORTA_DATA_R
#define PORTB                GPIO_PORTB_DATA_R
#define PORTC                GPIO_PORTC_DATA_R
#define PORTD                GPIO_PORTD_DATA_R
#define PORTE                GPIO_PORTE_DATA_R
#define PORTF                GPIO_PORTF_DATA_R

#define PORTA_BASE           0x40004000
#define PORTB_BASE           0x40005000
#define PORTC_BASE           0x40006000
#define PORTD_BASE           0x40007000
#define PORTE_BASE           0x40024000
#define PORTF_BASE           0x40025000

#define PORTbit0             0x00000004 
#define PORTbit1             0x00000008 
#define PORTbit2             0x00000010 
#define PORTbit3             0x00000020 
#define PORTbit4             0x00000040  
#define PORTbit5             0x00000080 
#define PORTbit6             0x00000100 
#define PORTbit7             0x00000200 


// Switches and LEDs
#define SW1         !(*((volatile unsigned long *)(PORTF_BASE|PORTbit4)))
#define SW2         !(*((volatile unsigned long *)(PORTF_BASE|PORTbit0)))

#define DARK	    0                  // dark     ---    0
#define RED	    0x02               // red      R--    0x02
#define BLUE	    0x04               // blue     --B    0x04
#define GREEN	    0x08               // green    -G-    0x08
#define YELLOW	    (RED|GREEN)        // yellow   RG-    0x0A
#define PINK	    (RED|BLUE)         // pink     R-B    0x06
#define SKYBLUE	    (GREEN|BLUE)       // sky blue -GB    0x0C
#define WHITE	    (RED|GREEN|BLUE)   // white    RGB    0x0E

#define SETLED( n ) (*((volatile unsigned long *)(PORTF_BASE|n<<2))) = n
#define CLRLED( n ) (*((volatile unsigned long *)(PORTF_BASE|n<<2))) = (~n)
#define CPLLED( n ) PORTF ^= n





void PortF_Init(void);       // PortF: MSB xxxIOOOI LSB
void PortE_Init(void);       // PortE: MSB xxxxxxII LSB
void PortD_Init(void);	     // PortD: 
void PortC_Init(void);	     // PortC: 
void initSWLEDS( void (*fSW1)(void), void (*fSW2)(void) );
void GPIOPortF_Handler(void);


#endif

