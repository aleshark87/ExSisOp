/* file:  es4_ripartire_in_ordine.c 
   Routine che fornisce un synchronization point. 
   E' chiamata da ognuno dei SYNC_MAX pthread, che si fermano 
   finche' tutti gli altri sono arrivati allo stesso punto di esecuzione. 

   Poi devono ripartire ed uscire nello stesso ordine
   in cui si sono fermati.
*/ 

#define _THREAD_SAFE

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <pthread.h> 
#include "printerror.h"

#define SYNC_MAX 5 

pthread_mutex_t  sync_lock; 
pthread_cond_t   sync_cond; 
int  sync_count = 0; 

void SyncPoint(void) 
{ 
	int rc;
	int indiceArrivo;

		pthread_t  th; 
		th=pthread_self();    /* thread identifier */ 

	/* blocca l'accesso al counter */ 
	rc = pthread_mutex_lock(&sync_lock); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed");
  
	indiceArrivo=sync_count;

	/* incrementa il counter di quelli arrivati*/ 
	sync_count++; 

	/* controlla se deve aspettare o no */ 
	if (sync_count < SYNC_MAX) {
		/* aspetta */ 

		printf("Sono %lu e mi sono fermato \n", th); 
		fflush(stdout);

		rc = pthread_cond_wait(&sync_cond, &sync_lock); 
		if( rc ) PrintERROR_andExit(rc, "pthread_cond_wait failed");
		rc = pthread_cond_signal (&sync_cond); /* senza questa signal ne terminano solo 2 */
		if( rc ) PrintERROR_andExit(rc,"pthread_cond_signal failed");
	}
	else    {
		printf("Sono %lu e mi sono fermato \n", th); 
		fflush(stdout);

		/* tutti hanno raggiunto il punto di barriera */ 

		/* nessuno e' ancora ripartito */
		sync_count=0;

		rc = pthread_cond_broadcast (&sync_cond); 
		if( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");
	}

	while(sync_count!=indiceArrivo) {
		rc = pthread_cond_wait(&sync_cond, &sync_lock); 
		if( rc ) PrintERROR_andExit(rc, "pthread_cond_wait failed");
	}

	/* incrementa il counter di quelli partiti*/ 
	sync_count++; 
	rc = pthread_cond_broadcast (&sync_cond); 
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");

		printf("Sono %lu e esco \n", th); 
		fflush(stdout);

	/* sblocca il mutex */ 
	rc = pthread_mutex_unlock (&sync_lock);
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed");
	return; 
} 

void *Thread (void *arg) 
{ 
	SyncPoint(); 
	pthread_exit(NULL); 
} 

int main () 
{ 
	pthread_t    th[SYNC_MAX]; 
	int  rc, i;
	void *ptr; 

	rc = pthread_cond_init(&sync_cond, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&sync_lock, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	for(i=0;i<SYNC_MAX;i++) {
		rc = pthread_create(&(th[i]), NULL, Thread, NULL); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	for(i=0;i<SYNC_MAX;i++) {
		rc = pthread_join(th[i], &ptr ); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	rc = pthread_mutex_destroy(&sync_lock); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&sync_cond); 
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	pthread_exit (NULL); 
} 
  
  
