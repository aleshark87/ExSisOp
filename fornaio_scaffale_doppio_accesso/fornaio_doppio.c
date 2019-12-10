#define POSTIBANCONE 2
#define NUMCLIENTI 3

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int bigliettoGlobale = 0;
int bigliettoSulDisplay = 0;
int clientiAlBancone = 0;
int alloScaffale = 0;
pthread_mutex_t mutexDistributore = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexTurno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t inDue = PTHREAD_COND_INITIALIZER;

void* cliente(void* arg){
	int indice; int biglietto;
	indice = * (int*) arg;
	indice++;
	while(1){
		pthread_mutex_lock(&mutexDistributore);
		biglietto = bigliettoGlobale;
		printf("sono cliente %d, prendo biglietto %d\n", indice, biglietto);
		bigliettoGlobale++;
		pthread_mutex_unlock(&mutexDistributore);
		pthread_mutex_lock(&mutexTurno);
		while(bigliettoSulDisplay != biglietto){
			pthread_cond_wait(&cond, &mutexTurno);
		}
		alloScaffale++;
		/* Se c'è posto per un altro, chiamo la broadcast per vedere
		 * chi è di turno!! Ovviamente, bisogna incrementare anche il
		 * biglietto. Altrimenti non ci sarebbe nessuno di turno */
		if(alloScaffale < POSTIBANCONE){
			printf("**CLIENTE %d**CHIAMO QUALCUN ALTRO \n", indice);
			bigliettoSulDisplay++;
			pthread_cond_broadcast(&cond);
		}
		pthread_mutex_unlock(&mutexTurno);
		printf("sono cliente %d e mi servo !, allo scaffale %d\n", indice, alloScaffale);
		sleep(3);
		pthread_mutex_lock(&mutexTurno);
		/* Questo if è molto importante.  */
		if(alloScaffale == POSTIBANCONE){
			printf("**CLIENTE %d**, PRIMA di increment, bigSulDispl %d \n",indice, bigliettoSulDisplay);
			bigliettoSulDisplay++;
			pthread_cond_broadcast(&cond);
		}
		alloScaffale--;
		pthread_mutex_unlock(&mutexTurno);
	}
}

int main(void){
	pthread_t tid; int i; int *p;
	for(i = 0; i < NUMCLIENTI; i++){
		p = malloc(sizeof(int));
		*p = i;
		pthread_create(&tid, NULL, cliente, (void*)p);
	}
	pthread_exit(NULL);
}
