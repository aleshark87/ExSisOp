/* file:  SyncPoint_a_coppie_CORRETTO.c
*/ 

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 
#include "printerror.h"
#include "DBGpthread.h"

#define SYNC_MAX 5

pthread_mutex_t  mutex; 
pthread_cond_t   cond_sync, cond_exit; 
int  sync_count = 0; 
#define INDICEA 0
#define INDICEB 1
intptr_t V[2]={-1,-1};

void esco_come_primo_di_coppia(intptr_t indice, int indiceTipo, int indiceAltroTipo, char *Tlabel )
{
	char debugmsg[1024];
	/* io sono il primo di coppia ad uscire */ 
	V[indiceTipo]=indice;
	/* avviso che sto attendendo secondo di coppia */
	sprintf(debugmsg, "%s_primodicoppia", Tlabel);
	DBGpthread_cond_signal (&cond_sync, debugmsg ); 
	/* attendo arrivo secondo di coppia */
	DBGpthread_cond_wait(&cond_exit, &mutex, debugmsg );
	/* ora posso leggere chi e' il secondo di coppia */
	printf("sono %" PRIiPTR " esco assieme a %" PRIiPTR "\n", indice, V[indiceAltroTipo] );
	fflush(stdout);
	/* dico al secondo di coppia che ora possiamo uscire */
	DBGpthread_cond_signal(&cond_exit, debugmsg );
}

void esco_come_secondo_di_coppia(intptr_t indice, int indiceTipo, int indiceAltroTipo, char *Tlabel )
{
	char debugmsg[1024];

	V[indiceTipo]=indice;
	/* ora posso leggere chi e' il primo di coppia */
	printf("sono %" PRIiPTR " esco assieme a %" PRIiPTR "\n", indice, V[indiceAltroTipo] );
	fflush(stdout);
	/* avviso che sono arrivato come secondo di coppia */
	sprintf(debugmsg, "%s_secondodicoppia", Tlabel);
	DBGpthread_cond_signal (&cond_exit, debugmsg ); 
	/* attendo conferma da primo di coppia che lui abbia letto mio indice */
	DBGpthread_cond_wait(&cond_exit, &mutex, debugmsg );
	/* prima di uscire resetto gli indici degli uscenti */
	V[INDICEA]=-1;
	V[INDICEB]=-1;
}


void SyncPoint(int indice, char tipo, int indiceTipo, int indiceAltroTipo)
{
	char Tlabel[128], debugmsg[1024];

	if( tipo=='A' ) {
		sprintf(Tlabel,"A%d",indice);
	} else {
		sprintf(Tlabel,"B%d",indice);
	}

	/* blocca l'accesso al counter */
	sprintf(debugmsg, "%s_inizio", Tlabel);
	DBGpthread_mutex_lock(&mutex, debugmsg );
 
	/* incrementa il counter di quelli arrivati */ 
	sync_count++;

	/* controlla se deve aspettare o no */
	if (sync_count != SYNC_MAX+1) {
		/* aspetta di uscire */
		while (1) {
			sprintf(debugmsg, "%s_attesatutti", Tlabel);
			DBGpthread_cond_wait(&cond_sync, &mutex, debugmsg);
			/* guardo se sono il primo della coppia e se sono del giusto tipo */
			if( V[indiceTipo]==-1) { /* sono del tipo giusto */

				if( V[indiceAltroTipo]==-1 ) { /* sono il primo di coppia */
					esco_come_primo_di_coppia(indice,indiceTipo,indiceAltroTipo,Tlabel );
				} else { /* sono il secondo di coppia ad uscire */
					esco_come_secondo_di_coppia(indice,indiceTipo,indiceAltroTipo,Tlabel );
					/* dico ad un altro in coda che puo' uscire */
					DBGpthread_cond_signal (&cond_sync, debugmsg ); 
				}
				break; /* esco dal while */
			}
			else { /* NON  sono il tipo giusto, sveglio un altro */
				sprintf(debugmsg, "%s_sonotiposbagliato", Tlabel);
				DBGpthread_cond_signal (&cond_sync, debugmsg ); 
			}
		} /* fine while */
	}
	else {
		/* I PRIMI SYNC_MAX+1 hanno raggiunto il punto di barriera
		   IO SONO IL PRIMO DELLA PRIMA COPPIA AD USCIRE 
		*/
		esco_come_primo_di_coppia(indice,indiceTipo,indiceAltroTipo,Tlabel );
	}

	sprintf(debugmsg, "%s_uscita", Tlabel);
	DBGpthread_mutex_unlock (&mutex, debugmsg);
	return; 
} 

void *Thread (void *arg) 
{ 
	intptr_t indice; char tipo; int indiceTipo, indiceAltroTipo;
	indice=(intptr_t)arg;
	if( indice < SYNC_MAX ) {
		tipo='A';
		indiceTipo=INDICEA;
		indiceAltroTipo=INDICEB;
	} else {
		tipo='B';
		indiceTipo=INDICEB;
		indiceAltroTipo=INDICEA;
	}

	SyncPoint(indice,tipo,indiceTipo,indiceAltroTipo); 
	pthread_exit(NULL); 
}

int main () 
{ 
	pthread_t    th[2*SYNC_MAX]; 
	int  rc; intptr_t i;
	void *ptr; 

	rc = pthread_cond_init(&cond_sync, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&cond_exit, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&mutex, NULL);
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	/* i thread con indice <SYNC_MAS sono di tipo A, gli altri di tipo B */
	for(i=0;i<2*SYNC_MAX;i++) {
		rc = pthread_create(&(th[i]), NULL, Thread, (void*)i); 
		if (rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	for(i=0;i<2*SYNC_MAX;i++) {
		rc = pthread_join(th[i], &ptr );
		if (rc) PrintERROR_andExit(rc,"pthread_join failed");
	}

	rc = pthread_mutex_destroy(&mutex);
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_destroy failed");
	rc = pthread_cond_destroy(&cond_sync);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	rc = pthread_cond_destroy(&cond_exit);
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_destroy failed");
	pthread_exit (NULL);
}

