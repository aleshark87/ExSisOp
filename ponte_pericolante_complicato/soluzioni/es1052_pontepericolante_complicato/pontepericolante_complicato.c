/* file:  ponte_pericolante_complicato.c */

#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif

#include "printerror.h"

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h> 

#include "DBGpthread.h"

#define NUMAUTOORARIO 4
#define NUMAUTOANTIORARIO 4
#define INDICESENSOORARIO 0
#define INDICESENSOANTIORARIO 1

/* dati da proteggere */
int bigliettoDistributore[2];
int numInAttesa[2];
int biglietto[2];
int PonteOccupato=0;
int MezzoPonteSuperato=1;	/* valido solo se PonteOccupato==1
				   quando una auto comincia attraversamento
				   viene messo a 0
				*/
int sensoDiAttraversamentoCorrente=0; /* valido solo se PonteOccupato==1 */

/* variabili per la sincronizzazione */
pthread_mutex_t  mutex;
pthread_mutex_t  mutexDistributori;
pthread_cond_t   condPonteLiberoInUnSenso[2];

void *Auto (void *arg) 
{ 
	char Plabel[128];
	intptr_t indiceAuto;
	int myBiglietto, indiceSenso;
	char senso; /* O orario  A antiorario */
	int faiWait;

	indiceAuto=(intptr_t)arg;
	if( indiceAuto<NUMAUTOORARIO ) 
	{	senso='O'; indiceSenso=INDICESENSOORARIO; }
	else
	{	senso='A'; indiceSenso=INDICESENSOANTIORARIO; }

	sprintf(Plabel,"%c%" PRIiPTR "", senso, indiceAuto);

	while(1) {
		DBGpthread_mutex_lock(&mutexDistributori,Plabel); 
		myBiglietto=bigliettoDistributore[indiceSenso];
		bigliettoDistributore[indiceSenso]++;
		DBGpthread_mutex_unlock(&mutexDistributori,Plabel); 

		printf("auto %s prende biglietto %i \n", Plabel, myBiglietto);
		fflush(stdout);

		DBGpthread_mutex_lock(&mutex,Plabel); 
		numInAttesa[indiceSenso]++;
		do {
			faiWait=0;
			if (	/* non e' il mio turno */
				( myBiglietto>biglietto[indiceSenso] ) ||
				/* oppure c'e' auto ad inizio ponte */
				( PonteOccupato && MezzoPonteSuperato==0 ) ||
				/* oppure c'e' auto contraria oltre meta' ponte */
				( PonteOccupato && MezzoPonteSuperato && 
			  	sensoDiAttraversamentoCorrente!=indiceSenso)
			)	
			{
				faiWait=1;
			}

			if( faiWait==0 ) {
			    if(
				/* c'e' auto favorevole oltre meta' ponte */
				( PonteOccupato && MezzoPonteSuperato && 
			  	sensoDiAttraversamentoCorrente==indiceSenso)
			    ) 
			    {
				printf("%s parto in coda ad altra auto\n",Plabel);fflush(stdout);
				break; /* e' il tuo turno, vai */
			    }
			}
			if( faiWait==0 ) {
			    if(
				/*ponte vuoto,mio turno,ma precedenza di la'*/
				( PonteOccupato==0 )
				&&
				( (numInAttesa[indiceSenso]<numInAttesa[!indiceSenso] )
			  	  ||
				  (
			  	  numInAttesa[indiceSenso]==numInAttesa[!indiceSenso]
			  	  &&
			  	  indiceSenso==INDICESENSOANTIORARIO
				  )
			        )
			   ) {
				faiWait=1;
			     }
			}
			if( faiWait==1 ) {
				DBGpthread_cond_wait(&condPonteLiberoInUnSenso[indiceSenso],&mutex,Plabel);
			} else {
				printf("%s parto con ponte vuoto\n",Plabel);fflush(stdout);
			}
		
		} while(faiWait==1);

		numInAttesa[indiceSenso]--;
		PonteOccupato=1;
		/* valido solo se PonteOccupato==1 */
		sensoDiAttraversamentoCorrente=indiceSenso;
		MezzoPonteSuperato=0;
		biglietto[indiceSenso]++;
		printf("auto %s attraversa ponte biglietto %i \n", Plabel,
			myBiglietto );
		fflush(stdout);

		DBGpthread_mutex_unlock(&mutex,Plabel); 

		sleep(1); /* attraverso la prima meta' del ponte */

		DBGpthread_mutex_lock(&mutex,Plabel); 
		printf("auto %s supera meta\' ponte \n", Plabel );
		fflush(stdout);
		MezzoPonteSuperato=1;
		/* risveglio quelli del mio verso di percorrenza */
		DBGpthread_cond_broadcast(&condPonteLiberoInUnSenso[indiceSenso],Plabel);
		DBGpthread_mutex_unlock(&mutex,Plabel); 

		sleep(1); /*attraverso seconda meta' del ponte */

		DBGpthread_mutex_lock(&mutex,Plabel); 
		if( MezzoPonteSuperato==1 ) { /* non e' entrata altra auto */
			PonteOccupato=0;
			/* risveglio quelli del mio verso di percorrenza */
			DBGpthread_cond_broadcast(&condPonteLiberoInUnSenso[indiceSenso],Plabel);
			/* risveglio quelli dell'altro verso di percorrenza */
			DBGpthread_cond_broadcast(&condPonteLiberoInUnSenso[!indiceSenso],Plabel);
		}
		DBGpthread_mutex_unlock(&mutex,Plabel); 

		printf("auto %s gira intorno\n", Plabel );
		fflush(stdout);

		sleep(10); /* faccio il giro attorno a Villa Inferno */
	}	

	pthread_exit(NULL); 
}


int main ( int argc, char* argv[] ) 
{ 
	pthread_t    th; 
	int  rc;
	intptr_t i;

	rc = pthread_mutex_init(&mutex, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	rc = pthread_mutex_init(&mutexDistributori, NULL); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	for( i=0; i<2; i++ ) {
		numInAttesa[i]=0;
		bigliettoDistributore[i]=0;
		biglietto[i]=0;
		rc = pthread_cond_init(&condPonteLiberoInUnSenso[i], NULL);
		if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");

	}

	for(i=0;i<NUMAUTOORARIO+NUMAUTOANTIORARIO;i++) {
		rc=pthread_create(&th,NULL,Auto,(void*)i); 
		if(rc) PrintERROR_andExit(rc,"pthread_create failed");
	}
	
	pthread_exit(NULL);
	return(0); 
} 
  
  
  
