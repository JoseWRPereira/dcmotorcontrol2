#ifndef __FIFO_H__
#define __FIFO_H__

#define POTN2		3		// max 6: 2^6 = 64
#define NAQUIS	 	(0x01<< POTN2)	
#define FIFOSIZE	(NAQUIS)

void clrFIFO( void );
void putFIFO( unsigned long dado );
unsigned long getFIFO( void );

#endif

