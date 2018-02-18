#include "controlador.h"
#include "io.h"

extern long vprint[8];

///*******************************************************//
//********************************************************//

int limiarGit; // grau de inconsistencia Gct > 0
int limiarGid; // grau de indefinição Gct < 0
int limiarZonaMorta;
void setLimiarGIT( int git )
{
  limiarGit = git;
}

void setLimiarGID( int gid )
{
  limiarGid = gid;
}

void setLimiarZonaMorta( int zm )
{
  limiarZonaMorta = zm;
}




void fmGctRst( void );
void initControlador( void )
{
  fmGctRst();
  setLimiarGIT(  100 );
  setLimiarGID( -50 );
  setLimiarZonaMorta( 100 );
  setTimerLPAEt(50);
  rstTimerLPAEt(300);
} 






//*******************************************************//
//*******************************************Tabela Delta
#define E24SIZE 24

int e24M[E24SIZE] = {	105,115,125,140,155,170,190,210,
			230,254,284,314,334,374,409,449,
			489,534,589,649,713,783,863,1000};

int e24[E24SIZE] = {	100,110,120,130,150,160,180,200,
			220,240,270,300,330,360,390,430,
			470,510,560,620,680,750,820,910};

int e24m[E24SIZE] ={  	  0,105,115,125,140,155,170,190,
			210,230,254,284,314,334,374,409,
			449,489,534,589,649,713,783,863};


long delta[E24SIZE] ={ 	0,  0,  0,  0,  0,  0,  0,  0,
		  	0,  0,  0,  0,  0,  0,  0,  0,
	 	  	0,  0,  0,  0,  0,  0,  0,  0};

long deltaAlvoMax = E24SIZE-1;
long deltaAlvo = E24SIZE>>1;;
long deltaAlvoMin = 0;

long rdDelta( void )
{
  return( delta[deltaAlvo] );
}

void wrDelta( long d )
{
  delta[deltaAlvo] = d;
}

void buscaDelta( int d )//recebe dado na forma da tabela e24
{ 
 char cont = 0;
 deltaAlvoMax = E24SIZE-1;
 deltaAlvo = E24SIZE>>1;;
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

void fmGctAdd( long gct )
{
  fmGctAcc -= fmGct[fmIndice];
  fmGctAcc += gct;
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

long timeLPAEt = 300; 		// 3 segundos
long timeLPAEtAtualiza = 100; 	// 1 segundos
long timeLPAEtrun = 0;
void timerLPAEt( void ) 	// 10 ms
{
  long aux;
  if( ++timeLPAEtrun > timeLPAEt )
  {
    timeLPAEtrun = 0;
    timeLPAEt = timeLPAEtAtualiza;
    aux = rdFmGct();
    if( aux < 100 )
      wrDelta( rdDelta() + (rdFmGct()+1)/2 );// Correção Delta
  }
}
////////////////////////////////////////
// Parametro:
// 	t: tempo de atualização da variável delta
//	Escala: 1 -> 10 ms
////////////////////////////////////////
void setTimerLPAEt( long t )
{
  timeLPAEtAtualiza = t;
}


////////////////////////////////////////
// Permite alterar o tempo de atualização do primeiro ciclo, 
// possibilitando uma atualização apenas após a acomodação
// do sinal.
// Parametro:
// 	t: tempo de acomodação do sinal. 
// 	Escala: 1 -> 10 ms
////////////////////////////////////////
void rstTimerLPAEt( long t )
{
  timeLPAEt = t;
  timeLPAEtrun = 0;
}



//*******************************************************//
//**************************************************LPAEt

long gc, gct, uer;
////////////////////////////////////////////////////////////
//
// Parâmetros:
// u0 : grau de evidencia favorável    [0..1000]
// u1 : grau de evidencia desfavorável [0..1000]
// Retorno: 
//   Através das variáveis gc e gct
//     gc  : grau de certeza 		[0..1000]
//     gct : grau de contradição	[0..1000]
//
////////////////////////////////////////////////////////////
void LPAEt( long u0, long u1 )
{
  long l0, l1;
  l0  = 1000-u0;
  l1  = 1000-u1;
  gc  = u0 - l1;
  gct = (u0 + l1 ) - 1000;
  uer = (u0 + u1)/2;
}



//*******************************************************//
//********************************************Controlador
// Parâmetros:				[rps]
//   	setpoint: valor de referência;	99.9 -> 999
//   	sensor: var controlada; 	99.9 -> 999
//	max: limite máximo da planta; 	99.9 -> 999
// Retorna:
// 	Valor do PWM: [0..100] -> [0..1000]
long setpointAnt = 0; 
long controlador( long setpoint, long sensor, long max )
{
  long vm; // Variavel manipulada

/////////////// LPAEt : Calculo de Gc e Gct
  // Para não trabalhar com ponto flutuante,
  // é adotado o ponto da milhar como virgula da unidade,
  // por isso a multiplicação por 1000.
  // Os 500 somados servem para arredondar o resultado.
  long u0 = ((setpoint*1000)+500)/max;
  long u1 = ((sensor  *1000)+500)/max;
  LPAEt(u0, u1);

/////////////// Add elemento ao filtro do Gct
  fmGctAdd( (int)gct );
/////////////// Busca Delta
  if( setpoint != setpointAnt )
  {
    setTimerLPAEt(50);
    rstTimerLPAEt(300);
    buscaDelta( (int)u0 );
    setpointAnt = setpoint;
  }



/////////////// Estados da LPAEt
  if( uer >= limiarZonaMorta )
  {
    if( gct > limiarGit )	// região Ligar
      vm = 1000;
    else if( gct < limiarGid )	// região Desligar
      vm = 0;
    else			// região Ativa
    {
     vm = u0 + rdDelta();
    }
  }
  else
    vm = 0;
/////////////// Limitador da VarManipulada
  if( vm < 0 ) vm = 0;
  if( vm > 1000 ) vm = 1000;
/////////////// Valores Tx pela serial
  vprint[0] = setpoint;
  vprint[1] = sensor;
  vprint[2] = vm;
  vprint[3] = gct;
  vprint[4] = delta[deltaAlvo];
  vprint[5] = rdFmGct();
  return( vm );
}


