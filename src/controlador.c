#include "controlador.h"


float raiz2( float num )
{
  char i;
  float recorre = num;
  for( i=0; i<5; ++i )
    recorre = recorre/2.0 + num/(2.0*recorre);
  return( recorre );
}


long Gc, Gct;
long diff;
long LPA2v( long  u0, long  l0 )
{
  if( u0 > 1000 ) u0 = 1000;
  if( u0 < 0 )    u0 = 0;
  if( l0 > 1000 ) l0 = 1000;
  if( l0 < 0 )    l0 = 0;

  Gc  = u0 - l0;
  Gct = (u0 + l0) - 1000;
}

long controlador( long setpoint, long sensor, long max )
{ 
  if( setpoint > sensor )
  {
    LPA2v( 1000, ((setpoint-sensor)*1000)/setpoint );
    diff = setpoint-sensor;
    return( (setpoint+diff)*10 ); 
  }
  else
    return( 0 );
}


