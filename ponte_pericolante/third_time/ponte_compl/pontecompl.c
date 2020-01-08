#define LATOA 0
#define LATOB 1
#define NUMORARIO 4
#define NUMANTI 4
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutexDistributore = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t latoA = PTHREAD_COND_INITIALIZER;
pthread_cond_t latoB = PTHREAD_COND_INITIALIZER;

int occupato = 0; int bigliettoGlobaleA = 1; int codaA = 0;
int codaB = 0; int bigliettoGlobaleB = 1;

void* auto_antiorarioA(void* arg){
	int indiceAutoA; int myBiglietto; int metaPonte = 0;
	indiceAutoA = * (int*) arg; 
	while(1){
		pthread_mutex_lock(&mutexDistributore);
		myBiglietto = bigliettoGlobaleA;
		bigliettoGlobaleA++;
		pthread_mutex_unlock(&mutexDistributore);
		pthread_mutex_lock(&mutex);
		codaA++;
		/* a questo punto bisogna decidere chi passa. */
		while(!( ((occupato == 0) || (metaPonte == 1)) && 
				(codaA >= codaB) && (myBiglietto < bigliettoGlobaleA) )){
			if(metaPonte == 1){
				 
				 break;
			}
			pthread_cond_wait(&latoA, &mutex);
		}
		occupato = 1;
		pthread_mutex_unlock(&mutex);
		printf("auto %d A PASSA SUL PONTE, CODA A = %d, .... CODA B = %d \n", indiceAutoA, codaA, codaB);
		sleep(4); pthread_mutex_lock(&mutex); metaPonte = 1;  pthread_cond_broadcast(&latoA);
		pthread_mutex_unlock(&mutex);
		 sleep(4); pthread_mutex_lock(&mutex); metaPonte = 0; pthread_mutex_unlock(&mutex);
		pthread_mutex_lock(&mutex); occupato = 0; codaA--; 
		pthread_cond_broadcast(&latoA); pthread_cond_broadcast(&latoB);
		pthread_mutex_unlock(&mutex);
		printf("auto %d A fa il giro \n", indiceAutoA);
		sleep(6);
	}
}

void* auto_orarioB(void* arg){
	int indiceAutoB; int myBiglietto;
	indiceAutoB = * (int*) arg;
	while(1){
		pthread_mutex_lock(&mutexDistributore);
		myBiglietto = bigliettoGlobaleB;
		bigliettoGlobaleB++;
		pthread_mutex_unlock(&mutexDistributore);
		codaB++;
		/*a questo punto vorrebbero tutti passare, ma passa solo se rispetta 
		 * le condizioni */
		 while((occupato == 1) || (codaB < codaA) ||
				(myBiglietto > bigliettoGlobaleB)){
			/*printf("auto %d B ha il biglietto %d \n", indiceAutoB, myBiglietto);*/
			pthread_cond_wait(&latoB, &mutex);
		}
		occupato = 1;
		pthread_mutex_unlock(&mutex);
		printf("auto %d B PASSA SUL PONTE, CODA B = %d, ... CODA A = %d \n", indiceAutoB, codaB, codaA);
		sleep(4);
		pthread_mutex_lock(&mutex); occupato = 0; codaB--;
		pthread_cond_broadcast(&latoB); pthread_cond_broadcast(&latoA);
		pthread_mutex_unlock(&mutex);
		printf("auto %d B fa il giro \n", indiceAutoB);
		sleep(6);
	}
}

int main(void){
	int indice; int *p;
	pthread_t tidB[NUMORARIO]; pthread_t tidA[NUMANTI];
	for(indice = 1; indice < (NUMORARIO + 1); indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tidB[indice - 1], NULL, auto_orarioB, (void*)p);
	}
	for(indice = 1; indice < (NUMANTI + 1); indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tidA[indice - 1], NULL, auto_antiorarioA, (void*)p);
	}
	pthread_exit(NULL);
}

 
