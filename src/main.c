/* ******************************************************************

           Controle de sistema dinamico utilizando LPA2v
 
                    Pós-Graduação Stricto Sensu
    Mestrado Profissional em Automação e Controle de Processos 
 Instituto Federal de Educação, Ciência e Tecnologia de São Paulo
 				IFSP
                          José W. R. Pereira 	
                              2017-10-30

******************************************************************* */

/* ************************** Uso dos Pinos em cada PORT

PA0 : UART0_RX 
PA1 : UART0_TX
PA2 :
PA3 : 
PA4 : 
PA5 : 
PA6 :
PA7 :

PB0 : 
PB1 : 
PB2 :
PB3 : 
PB4 : 
PB5 : 
PB6 : M0PWM0
PB7 : M0PWM1

PC0 : 
PC1 : 
PC2 :
PC3 : 
PC4 : 
PC5 : 
PC6 :
PC7 :

PD0 : 
PD1 : 
PD2 : WT3A - Capture Mode
PD3 :  
PD4 : 
PD5 : 
PD6 : UART2-RX
PD7 : UART2-TX

PE0 : 
PE1 : 
PE2 :
PE3 : 
PE4 : 
PE5 : 
PE6 :
PE7 :

PF0 : SW2
PF1 : LED_R
PF2 : LED_B
PF3 : LED_G
PF4 : SW1

PF5 : 
PF6 :
PF7 :

********************************************************* */

#include "pll.h"
#include "io.h"
#include "uart.h"
#include "systick.h"
#include "fifo.h"
#include "pwm.h"
#include "timer.h"

void vazio(void){ }


void sTempo( void )
{
   SETLED( BLUE );
   putFIFO( readSysTick() );
   clearSysTick();
   CLRLED( BLUE );
}


void main( void )
{
  char tecla;
  unsigned int frequencia, duty; 
  initPLL();
  initSWLEDS( &vazio, &sTempo );
  initUART0_80MHz_115200bps();
  initPWM( 500, 1 );
  initTimer( 800000, &vazio);
  initSysTick( 80000 );
  clrFIFO();
  
  while( 1 )
  {
    if( UART_InCharAvailable() )
      tecla =  UART_InChar();
    else
      tecla = 0;

    switch( tecla )
    {
      case ' ': 
      case '0':
		setPWM(   0 ); break;
      case '1': setPWM( 100 ); break;
      case '2': setPWM( 200 ); break;
      case '3': setPWM( 300 ); break; 
    }


    if( readSysTickB() > 100 )
    {
      SETLED( RED );
      clearSysTickB();
      UART_OutUDec( getFIFO() );
      UART_OutCRLF();
      CLRLED( RED );
    }
  }
}

