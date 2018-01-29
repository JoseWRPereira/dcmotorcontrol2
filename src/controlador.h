#ifndef __CONTROLADOR_H__
#define __CONTROLADOR_H__


extern long gc, gct, et, mx;

long controlador( long setpoint, long sensor, long max );
void incrementaErro( void );
#endif

