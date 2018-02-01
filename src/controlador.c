#include "controlador.h"
#include "io.h"

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
long setpointAnt = 0, et=0, erro=0;
char rampa = 0, partida = 0;

long controlador( long setpoint, long sensor, long max )
{
  long vm, err;		// Var manipulada
  float ksp;		// relacao 100-sp / sp-0
  float u0,u1;

  u0 = (float)setpoint/(float)max;
  u1 = (float)sensor/(float)max;
  LPA2v(u0,u1);

  ksp = (1.0-u0)/(u0);
  if( u1 < u0*0.80 || u1 > u0*1.20 )
  {
    vm = max - (ksp*sensor);
    erro = 0;
  }
  else
  {
    //vm = max - (ksp*setpoint) + erro ;
    vm = setpoint + erro; 
  }

  if( vm < 1 ) vm = 1;
  if( vm > 1000 ) vm = 1000;

  vprint[0] = setpoint;
  vprint[1] = sensor;
  vprint[2] = vm;
  vprint[3] = gct;
  vprint[4] = erro;

  return( vm );
}
long setTempo=20, runTempo=0;
void incrementaErro( void )
{
  if( ++runTempo > setTempo )
  {
    runTempo = 0;
    if( gct > 0 )
      ++erro;
    if( gct < 0 )
      --erro;
  }
}


