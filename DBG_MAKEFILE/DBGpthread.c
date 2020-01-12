/* DBGpthread.c */

/* a causa dell'uso di strerror_r
   dove incluso, compilare con -D_POSIX_C_SOURCE >= 200112L
*/

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
        #define _POSIX_C_SOURCE 200112L
#else
        #if _POSIX_C_SOURCE < 200112L
                #undef  _POSIX_C_SOURCE
                #define _POSIX_C_SOURCE 200112L
        #endif
#endif


#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <pthread.h> 
#include <errno.h>
#include <string.h>	/* per strerror_r  and  memset */
#include "printerror.h"

/* #define DEBUG */

void DBGpthread_mutex_lock(pthread_mutex_t *AddrMutex) 
{
	int rc;	
	rc = pthread_mutex_lock(AddrMutex);
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
}

void DBGpthread_mutex_unlock( pthread_mutex_t *AddrMutex)
{	int rc;
	rc = pthread_mutex_unlock(AddrMutex);
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
}

void DBGpthread_cond_wait(pthread_cond_t *AddrCond, pthread_mutex_t *AddrMutex)
{	int rc;
	rc = pthread_cond_wait(AddrCond,AddrMutex);
	if(rc) PrintERROR_andExit(rc,"pthread_cond_wait failed");
}

void DBGpthread_cond_signal(pthread_cond_t *AddrCond)
{	int rc;	
	rc = pthread_cond_signal(AddrCond);
	if(rc) PrintERROR_andExit(rc,"pthread_cond_signal failed");
}

void DBGpthread_cond_broadcast(pthread_cond_t *AddrCond)
{	int rc;	
	rc = pthread_cond_broadcast(AddrCond);
	if(rc) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");
}


void DBGpthread_mutex_init( pthread_mutex_t *mutex,
           const pthread_mutexattr_t *attr)
{
	int rc;
	rc = pthread_mutex_init( mutex, attr );
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_init failed");
}


void DBGpthread_mutex_destroy(pthread_mutex_t *mutex)
{
	int rc;
	rc = pthread_mutex_destroy( mutex );
	if(rc) PrintERROR_andExit(rc,"pthread_mutex_init failed");
}


void DBGpthread_cond_init( pthread_cond_t *cond,
           const pthread_condattr_t *attr )
{
	int rc;
	rc = pthread_cond_init( cond, attr );
	if(rc) PrintERROR_andExit(rc,"pthread_cond_init failed");
}


void DBGpthread_cond_destroy(pthread_cond_t *cond)
{
	int rc;
	rc = pthread_cond_destroy( cond );
	if(rc) PrintERROR_andExit(rc,"pthread_cond_init failed");
}



void DBGsleep( unsigned int NumSeconds )
{
	sleep(NumSeconds);
}


void DBGnanosleep( unsigned long int NumNanoSeconds)
{
	#define NANOSEC_IN_SEC 1000000000L
	int rc, myerrno;
	struct timespec req, res;
	res.tv_sec=NumNanoSeconds/NANOSEC_IN_SEC;
	res.tv_nsec=NumNanoSeconds%NANOSEC_IN_SEC;
	do {
		req=res;
		rc=nanosleep(&req,&res);
		myerrno=errno;
	} while( (rc<0) && (myerrno==EINTR) );
	
	if( rc<0 ) {
                PrintERROR_andExit(myerrno,"nanosleep failed");
        }
}


