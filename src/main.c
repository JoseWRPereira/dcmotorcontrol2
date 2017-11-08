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
#include "controlador.h"

void vazio(void){ }

long addTcont;
void addTempo( void )
{
  if( ++addTcont > 100 )
  {
    SETLED( BLUE );
    putFIFO( readSysTick() );
    addTcont = 0;;
    CLRLED( BLUE );
  } 
}


void sTempo( void )
{
   SETLED( BLUE );
   putFIFO( readSysTick() );
   addTcont = 0;
   clearSysTick();
   CLRLED( BLUE );
}

void printFIFO( void )
{
  SETLED( RED );
  UART_OutUDec( getFIFO() );
  UART_OutCRLF();
  CLRLED( RED );
} 

void main( void )
{
  char tecla;
  char fifoOut = 0;
  long controle;
  long sp;
//  unsigned int frequencia, duty; 
  initPLL();
  initSWLEDS( &vazio, &sTempo );
  initUART0_80MHz_115200bps();
  initPWM( 500, 1 );
//  initTimer( 800000, &printSerial);
  initTimer(800000, &addTempo);
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
		sp = 1; break;
      case '1': sp = 10 ; 
		fifoOut = 1;   break;
      case '2': sp = 20; 
		fifoOut = 1;   break;
      case '3': sp = 25; 
		fifoOut = 1;   break;
      case '4': sp = 30; 
		fifoOut = 1;   break;
      case 'p': fifoOut = 0;   break;
    }


    controle = controlador( sp, 1000/getFIFO(), 80 );
    setPWM( controle );

    if( readSysTickB() > 100 && fifoOut )
    {
      clearSysTickB();
      //printFIFO();  

      SETLED( GREEN );
      UART_OutUDec( 1000/getFIFO() ); 
      UART_OutChar( ' ' );
      UART_OutUDec( controle );
      UART_OutChar( ' ' );
      if( Gc & 0x80000000  )
      {
        UART_OutChar( '-' );
        UART_OutUDec( (~Gc)+1 );
      }
      else
        UART_OutUDec( Gc );
      UART_OutChar( ' ' );
      if( Gct & 0x80000000  )
      {
        UART_OutChar( '-' );
        UART_OutUDec( (~Gct)+1 );
      }
      else
        UART_OutUDec( Gct );
      UART_OutCRLF();
      CLRLED( GREEN );
    
    }
  }
}

