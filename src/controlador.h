#ifndef __CONTROLADOR_H__
#define __CONTROLADOR_H__


extern long gc, gct, et, mx;

void initControlador( void );
long controlador( long setpoint, long sensor, long max );
void timerLPAEt( void );

#endif

