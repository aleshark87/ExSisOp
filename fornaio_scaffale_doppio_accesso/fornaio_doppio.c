#define POSTIBANCONE 2
#define NUMCLIENTI 10

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
	while(1){
		pthread_mutex_lock(&mutexDistributore);
		biglietto = bigliettoGlobale;
		printf("sono cliente %d, prendo biglietto %d\n", indice, biglietto);
		bigliettoGlobale++;
		pthread_mutex_unlock(&mutexDistributore);
		pthread_mutex_lock(&mutexTurno);
		printf("bigliettoDisplay %d \n", bigliettoSulDisplay);
		while((bigliettoSulDisplay != biglietto) || (alloScaffale >= POSTIBANCONE)){
			printf("sono cliente %d, e aspetto. \n", indice);
			pthread_cond_wait(&cond, &mutexTurno);
			printf("sono cliente %d, e mi sono risvegliato \n", indice);
		}
		alloScaffale++;
		printf("sono cliente %d e mi servo !, allo scaffale %d\n", indice, alloScaffale);
		sleep(3);
		alloScaffale--;
		bigliettoSulDisplay++;
		pthread_cond_broadcast(&cond);
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
