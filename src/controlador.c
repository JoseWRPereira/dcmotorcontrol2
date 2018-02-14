#include "controlador.h"
#include "io.h"

extern long vprint[8];


//*******************************************************//
//*******************************************Tabela Delta
#define E24SIZE 24

int e24M[E24SIZE] = {	107,117,128,143,158,173,194,214,
			235,260,291,322,352,383,419,460,
			500,546,602,664,730,802,883,1000};

int  e24[E24SIZE] = {	100,110,120,130,150,160,180,200,
			220,240,270,300,330,360,390,430,
			470,510,560,620,680,750,820,910};

int e24m[E24SIZE] = {  	  0,107,117,128,143,158,173,194,
			214,235,260,291,322,352,383,419,
			460,500,546,602,664,730,802,883};

int delta[E24SIZE] = { 	0,  0,  0,  0,  0,  0,  0,  0,
		  	0,  0,  0,  0,  0,  0,  0,  0,
	 	  	0,  0,  0,  0,  0,  0,  0,  0};

char deltaAlvoMax = E24SIZE-1;
char deltaAlvo = E24SIZE>>1;;
char deltaAlvoMin = 0;

int rdDelta( void )
{
  return( delta[deltaAlvo] );
}

void wrDelta( int d )
{
  delta[deltaAlvo] = d;
}

void buscaDelta( int d )//recebe dado na forma da tabela e24
{ 
 char cont = 0;
 char diff = E24SIZE>>1;
 deltaAlvoMax = E24SIZE-1;
 deltaAlvo = diff;
 deltaAlvoMin = 0;
 
  while( (deltaAlvoMin != deltaAlvoMax) && (++cont<E24SIZE) )
  {
    if( d < e24M[deltaAlvo] )
    {
      deltaAlvoMax = deltaAlvo; 
      deltaAlvo = (deltaAlvoMax+deltaAlvoMin+1)>>1;;
    }
    if( d > e24m[deltaAlvo] )
    {
      deltaAlvoMin = deltaAlvo;
      deltaAlvo = (deltaAlvoMax+deltaAlvoMin+1)>>1;;
    }    
  }
}


//*******************************************************//
//***************************************Filtro Media Gct
#define FMSIZE 8

int fmGct[FMSIZE];
long fmGctAcc = 0;
char fmIndice = 0;

void fmGctAdd( int gct )
{
  fmGctAcc -= fmGct[fmIndice];
  fmGctAcc +- gct;
  fmGct[fmIndice] = gct;
  fmIndice = (fmIndice+1)%FMSIZE;
}

void fmGctRst( void )
{
  for( char i = 0; i<FMSIZE; i++ )
    fmGct[i] = 0;
  fmGctAcc = 0;
  fmIndice = 0;
}

long rdFmGct( void )
{
  return( fmGctAcc / FMSIZE );
}

//*******************************************************//
//********************************************LPAEt timer

long timeLPAEt = 200; 	// 2 segundos
long timeLPAEtrun = 0;

void timerLPAEt( void ) // 10 ms
{
  if( ++timeLPAEtrun > timeLPAEt )
  {
    timeLPAEtrun = 0;

    wrDelta( rdDelta() + rdFmGct() );  // Correção Delta

  }
}

void setTimerLPAEt( long t )
{
  timeLPAEt = t;
}

void rstTimerLPAEt( long t )
{
  timeLPAEt = t;
  timeLPAEtrun = 0;
}



//*******************************************************//
//*******************************************************//




long gc, gct, uer;
float Gc, Gct, uEr;


//struct klp_T
//{
//  float limiarGc;
//  int offset;
//}
 
int klp[10] = {1000,10,20,30,40,50,60,70,80,90};



void initControlador( void )
{
 
/*
  klp[0].limiarGc = -0.9;
  klp[0].offset = 1000;
 
  klp[1].limiarGc = -0.5;
  klp[1].offset = 34;

  klp[2].limiarGc = -0.4;
  klp[2].offset = 14;

  klp[3].limiarGc = 1.0;
  klp[3].offset = 117;
*/   
} 

int offset[8] = {0,0,0,0,0,0,0,0};
char indice = 0;
char contIndice = 0;
int offsetAnt = 0;
long somaOffset = 0;

long setTempo=10, runTempo=0;
void incrementaErro( void )
{
  if( ++runTempo > setTempo )
  {
    runTempo = 0;
    if( gct == offsetAnt )
      contIndice++;
    else
    {
      offsetAnt = gct;
      contIndice = 0;
    }
    if( contIndice > 20 )
    {
      contIndice = 0;
      klp[uer] += gct/2;
    }
  
    somaOffset -= offset[indice];
    somaOffset += offset[indice] = gct;
    indice = (indice+1)%8;
 }
}




long LPA2v( float  u0, float u1 )
{
  float l0, l1;
  l0 = 1.0 - u0;
  l1 = 1.0 - u1;

  Gc  = u0 - l1;
  Gct = (u0 + l1) - 1.0;
  gc = Gc * 1000;
  gct = Gct * 1000;
  uEr = (Gc+1.0)/2.0;
  uer = uEr * 10;

}

char estado = 1;
long setpointAnt = 0, et=0, erro=0;
char rampa = 0, partida = 0;


long controlador( long setpoint, long sensor, long max )
{
  long vm, err;		// Var manipulada
  float ksp;		// relacao 100-sp / sp-0
  float u0,u1;


/////////////// LPAEt : Calculo de Gc e Gct
  u0 = (float)setpoint/(float)max;
  u1 = (float)sensor/(float)max;
  LPA2v(u0,u1);

///////////////
  if( setpoint > 10 )
  {
    if( gct > 50 )
      vm = 1000;
    else if( gct < -50 )
      vm = 0;
//    else if( gct > 20 )
//      vm = setpoint + 2*klp[uer];
//    else if( gct < -20 )
//       vm = setpoint + klp[uer]/2;
    else
    if( uer < 10 )
      vm = setpoint + klp[uer];
    else
      vm = setpoint + klp[9];
  }
  else
    vm = 0;
//  ksp = (1.0-u0)/(u0);
//  if( u1 < u0*0.60 || u1 > u0*1.20 )
//  {
//    vm = max - (ksp*sensor);
//    erro = 0;
//  }
//  else
//  {
//    //vm = max - (ksp*setpoint) + erro ;
//    vm = setpoint + erro; 
//  }

/////////////// Limitador da VarManipulada
  if( vm < 0 ) vm = 0;
  if( vm > 1000 ) vm = 1000;
/////////////// Valores Tx pela serial
  vprint[0] = setpoint;
  vprint[1] = sensor;
  vprint[2] = vm;
  vprint[3] = gct;
  vprint[4] = uEr*1000;
  vprint[5] = somaOffset/8;
  return( vm );
}


