#include "fifo.h"

unsigned long fifo[FIFOSIZE];
unsigned long fifoSum;
unsigned char fifoIndex=0;


void clrFIFO( void )
{
  unsigned char i;
  for( i = 0; i < FIFOSIZE; i++ )
  {
    fifo[i] = 0;
  }
  fifoIndex = 0;
  fifoSum = 0;
}

void putFIFO( unsigned long dado )
{
  fifoSum += (dado - fifo[fifoIndex]);
  fifo[fifoIndex] = dado;
  fifoIndex = (++fifoIndex % FIFOSIZE);
}

unsigned long getFIFO( void )
{
  return(fifoSum >> POTN2);
}

