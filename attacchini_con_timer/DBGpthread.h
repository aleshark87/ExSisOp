/* DBGpthread.h */

#ifndef __DBGPTHREAD_H__
#define __DBGPTHREAD_H__

#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h> 

void DBGpthread_mutex_lock( pthread_mutex_t *AddrMutex );
int DBGpthread_mutex_trylock(pthread_mutex_t *AddrMutex );
void DBGpthread_mutex_unlock( pthread_mutex_t *AddrMutex );
void DBGpthread_cond_wait( pthread_cond_t *AddrCond, pthread_mutex_t *AddrMutex );
void DBGpthread_cond_signal( pthread_cond_t *AddrCond );
void DBGpthread_cond_broadcast( pthread_cond_t *AddrCond );

void DBGpthread_mutex_init( pthread_mutex_t *mutex,
           const pthread_mutexattr_t *attr );
void DBGpthread_mutex_destroy(pthread_mutex_t *mutex );
void DBGpthread_cond_init( pthread_cond_t *cond,
           const pthread_condattr_t *attr );
void DBGpthread_cond_destroy(pthread_cond_t *cond );

void DBGsleep( unsigned int NumSeconds );
void DBGnanosleep( unsigned long int NumNanoSeconds );

#endif  /*  __DBGPTHREAD_H__ */

