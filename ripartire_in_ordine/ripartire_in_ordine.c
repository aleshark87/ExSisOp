/* file:  CondVarSignal.c 
   Routine che fornisce un synchronization point. 
   E' chiamata da ognuno dei SYNC_MAX pthread, che si fermano 
   finche' tutti gli altri sono arrivati allo stesso punto di esecuzione. 
*/ 

/* simboli già messi nella riga di comando del compilatore 
#define _THREAD_SAFE
#define _REENTRANT
#define _POSIX_C_SOURCE 200112L
*/

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 
#include "printerror.h"

#define SYNC_MAX 5 

pthread_mutex_t  sync_lock; 
pthread_cond_t   sync_cond; 
int  sync_count = 0; 

void SyncPoint(void) 
{ 
	int rc, turn;

	/* blocca l'accesso al counter */ 
	rc = pthread_mutex_lock(&sync_lock); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_lock failed"); /* no EINTR */
	
	turn = sync_count;
	/* incrementa il counter di quelli arrivati*/ 
	sync_count++; 

	/* controlla se deve aspettare o no */ 
	if (sync_count < SYNC_MAX) {
		/* aspetta */ 
		printf("sono thread numero %lu e sto aspettando alla prima wait\n",pthread_self());
		fflush(stdout);
		rc = pthread_cond_wait(&sync_cond, &sync_lock); 
		printf("sono thread numero %lu e ho sorpassato la prima wait\n",pthread_self());
		if( rc ) PrintERROR_andExit(rc, "pthread_cond_wait failed"); /* no EINTR */
		rc = pthread_cond_signal (&sync_cond); /* senza questa signal ne terminano solo 2 */
		if( rc ) PrintERROR_andExit(rc,"pthread_cond_signal failed"); /* no EINTR */
	}
	else    {
		/* tutti hanno raggiunto il punto di barriera */ 
		/*si riparte dal primo*/
		sync_count = 0;
		/*partano tutti !*/
		printf("sono il thread %lu e adesso faccio la prima broadcast !!\n",pthread_self());
		rc = pthread_cond_broadcast (&sync_cond); 
		if( rc ) PrintERROR_andExit(rc,"pthread_cond_signal failed"); /* no EINTR */
	}
	while(sync_count!=turn) {
		printf("sono %lu e aspetto dentro il while..\n",pthread_self());
		rc = pthread_cond_wait(&sync_cond, &sync_lock); 
		if( rc ) PrintERROR_andExit(rc, "pthread_cond_wait failed");
		printf("sono %lu e ho sorpassato la wait nel while :)\n",pthread_self());
	}
	/*incremento sync_count*/	
	sync_count++;
	
	/*e rifaccio andare tutti*/	
	printf("sono il thread %lu e adesso faccio la seconda broadcast !!\n",pthread_self());
	rc = pthread_cond_broadcast (&sync_cond); 
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_broadcast failed");

	printf("Sono %lu e esco \n", pthread_self()); 
	fflush(stdout);

	/* sblocca il mutex */ 
	rc = pthread_mutex_unlock (&sync_lock);  /* senza unlock ne termina solo 1 */ 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_unlock failed"); /* no EINTR */
	return; 
} 

void *Thread (void *arg) 
{ 
	pthread_t  th; 

	th=pthread_self();    /* thread identifier */  
	SyncPoint(); 
	printf("Sono %lu e sono uscito \n", th); 
	pthread_exit(NULL); 
} 

int main () 
{ 
	pthread_t    th[SYNC_MAX]; 
	int  rc; intptr_t i;
	void *ptr; 
	/*si poteva fare una allocazione anche statica ma qua è dinamica*/
	rc = pthread_cond_init(&sync_cond, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed"); /* no EINTR */
	rc = pthread_mutex_init(&sync_lock, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed"); /* no EINTR */
	
	/*vengono creati SYNC_MAX Thread. Ad ognuno viene passata la funzione thread*/
	for(i=0;i<SYNC_MAX;i++) {
		rc = pthread_create(&(th[i]), NULL, Thread, NULL); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed"); /* no EINTR */
	}
	/*aspetto SYNC_MAX Thread*/
	for(i=0;i<SYNC_MAX;i++) {
		rc = pthread_join(th[i], &ptr ); 
		if (rc) PrintERROR_andExit(rc,"pthread_join failed"); /* no EINTR */
	}
	
	/*semplice deallocazione mutex e cond*/
	rc = pthread_mutex_destroy(&sync_lock); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_destroy failed"); /*no EINTR*/
	rc = pthread_cond_destroy(&sync_cond); 
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_destroy failed"); /*no EINTR*/
	pthread_exit (NULL); 
} 
  
  
  
