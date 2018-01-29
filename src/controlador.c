#include "controlador.h"
extern long vprint[8];



long gc, gct;
float Gc, Gct;
long LPA2v( float  u0, float u1 )
{
  float l0, l1;
  l0 = 1.0 - u0;
  l1 = 1.0 - u1;

  Gc  = u0 - l1;
  Gct = (u0 + l1) - 1.0;
  gc = Gct * 1000;
  gct = Gct * 1000;
}

char estado = 1;
long setpointAnt = 0, et=0;
char rampa = 0, partida = 0;
long controlador( long setpoint, long sensor, long max )
{
  long vm; 		// Var manipulada
  float ksp;		// relacao 100-sp / sp-0
  float u0,u1;

  if( setpointAnt != setpoint )
  {
    setpointAnt = setpoint;
    //et = 0;
  }
  u0 = (float)setpoint/(float)max;
  u1 = (float)sensor/(float)max;
  LPA2v(u0,u1);

  ksp = (1.0-u0)/(u0);
  if( u0 < 0.05 )
  {
    vm = 1;
    et = 0;
    partida = 1;
  }
  else  if( u1 < u0*0.8 )
  {
    vm = 1000 - (ksp*sensor);
    rampa = 1;
  }
  else
  {
    vm = 1000 - (ksp*sensor) + et;
    rampa = 0;
    partida = 0;
  }

  if( vm < 1 ) vm = 1;
  if( vm > 1000 ) vm = 1000;

  vprint[0] = setpoint;
  vprint[1] = sensor;
  vprint[2] = vm;
  vprint[3] = gct;
  vprint[4] = et;

  return( vm );
}

long setTempoIncrErro = 10;
long conTempoIncrErro = 0;

void incrementaErro( void )
{
 if( !rampa || partida )
  if( ++conTempoIncrErro >= setTempoIncrErro )
  {
    conTempoIncrErro = 0;
    if( gct > 20 )
    {
      et += 1;
      setTempoIncrErro = 1;
    }
    else if( gct > 0 )
    {
      et += 1;
      setTempoIncrErro = 2;
    }
    else if( gct < -20 )
    {
      et -= 2;
      setTempoIncrErro = 1;
    }
    else if( gct < -1 )
    {
      et -= 1;
      setTempoIncrErro = 2;
    }
  }
}


float raiz2( float num )
{
  char i;
  float recorre = num;
  for( i=0; i<5; ++i )
    recorre = recorre/2.0 + num/(2.0*recorre);
  return( recorre );
}

