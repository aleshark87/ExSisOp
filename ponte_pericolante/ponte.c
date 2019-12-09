#define NMACCHINEORARIO 4
#define NMACCHINEANTIORARIO 4

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int siamoAlPonte[NMACCHINEANTIORARIO + NMACCHINEORARIO];
int bigliettoGlobA = 0;
int bigliettoVisA = 0;
pthread_mutex_t mutexDistributoreA = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
int bigliettoGlobB = 0;
int bigliettoVisB = 0;
int occupato = 0;
int macchineInCodaLatoA = 0;
int macchineInCodaLatoB = 0;
pthread_mutex_t mutexDistributoreB = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexTurno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condB = PTHREAD_COND_INITIALIZER;
pthread_cond_t condOccupato = PTHREAD_COND_INITIALIZER;

void* macchina_antiorario(void* arg){
	while(1){
		int indice; int biglietto;
		indice = * (int*) arg;
		while(siamoAlPonte[indice] == 0){
			sleep(5);
			siamoAlPonte[indice] = 1;
		}
		pthread_mutex_lock(&mutexDistributoreA);
		biglietto = bigliettoGlobA;
		printf("dato biglietto %d , a macchina %d \n", biglietto, indice);
		bigliettoGlobA++;
		pthread_mutex_unlock(&mutexDistributoreA);
		pthread_mutex_lock(&mutexTurno);
		
		while(bigliettoVisA != biglietto){
			pthread_cond_wait(&condA, &mutexTurno);
		}
		while(occupato == 1){
			 pthread_cond_wait(&condOccupato, &mutexTurno); 
		}
		/* l'automobilista attraversa il ponte */
		printf("macchina %d attraversa il ponte, coda: %d \n", indice, macchineInCodaLatoA);
		occupato = 1;
		sleep(3);
		occupato = 0; 
		printf("macchina %d ha attraversato il ponte \n",indice);
		siamoAlPonte[indice] = 0;
		bigliettoVisA++;
		pthread_cond_broadcast(&condA);
		pthread_mutex_unlock(&mutexTurno);
	}
}

void* macchina_orario(void* arg){
	while(1){
		int indice; int biglietto;
		indice = * (int*) arg;
		while(siamoAlPonte[indice] == 0){
			sleep(5);
			siamoAlPonte[indice] = 1;
		}
		pthread_mutex_lock(&mutexDistributoreB);
		biglietto = bigliettoGlobB;
		printf("dato biglietto %d , a macchina %d \n", biglietto, indice);
		bigliettoGlobB++;
		pthread_mutex_unlock(&mutexDistributoreB);
		pthread_mutex_lock(&mutexTurno);
		while(bigliettoVisB != biglietto){
			pthread_cond_wait(&condB, &mutexTurno);
		}
		while(occupato == 1){
			 printf("macchina %d aspetta \n", indice);
			 pthread_cond_wait(&condOccupato, &mutexTurno); 
		}
		printf("macchina %d attraversa il ponte \n", indice);
		occupato = 1;
		sleep(3);
		occupato = 0;
		printf("macchina %d ha attraversato il ponte \n", indice);
		pthread_cond_broadcast(&condOccupato);
		siamoAlPonte[indice] = 0;
		bigliettoVisB++;
		pthread_cond_broadcast(&condB);
		pthread_mutex_unlock(&mutexTurno);
	}
}

int main(void){
	
	int indice; int *p;
	pthread_t tid;
	for(indice = 0; indice < NMACCHINEANTIORARIO + NMACCHINEORARIO; indice++){
		p = malloc(sizeof(int*));
		*p = indice;
		sleep(1);
		if(indice % 2 == 0){
			pthread_create(&tid, NULL, macchina_orario, (void*)p);
		}
		else {
			pthread_create(&tid, NULL, macchina_antiorario, (void*)p);
		}
	}
	pthread_exit(NULL);
	return 0;
}
