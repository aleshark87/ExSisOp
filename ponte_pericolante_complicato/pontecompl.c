#define NUMAUTOORARIO 4
#define NUMAUTOANTI 15
#define LATOA 1
#define LATOB 0
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 

pthread_mutex_t mutexDistributore = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condPonteLibero = PTHREAD_COND_INITIALIZER;
int bigliettoGlobaleA = 1; int bigliettoGlobaleB = 1;
int ponteOccupato = 0; int numInAttesaA = 0; int numInAttesaB = 0;
int bigliettoA = 1; int bigliettoB = 1; int metaPonte[2] = {0, 0} ;

void* auto_orario(void * arg){
	int indiceAuto; int myBiglietto; indiceAuto = *(int*) arg;
	while(1){
		pthread_mutex_lock(&mutexDistributore);
		myBiglietto = bigliettoGlobaleB;
		bigliettoGlobaleB++;
		pthread_mutex_unlock(&mutexDistributore);
		pthread_mutex_lock(&mutex); 
		numInAttesaB++;
		while((ponteOccupato == 1) ||
				(myBiglietto > bigliettoB) ||
					(numInAttesaB < numInAttesaA)){
						pthread_cond_wait(&condPonteLibero, &mutex);
		}
		numInAttesaB--; ponteOccupato = 1; bigliettoB++;
		printf("auto B %d attraversa il ponte. \n", indiceAuto);
		pthread_mutex_unlock(&mutex); sleep(1); 
		pthread_mutex_lock(&mutex);
		
		ponteOccupato = 0; pthread_cond_broadcast(&condPonteLibero);
		pthread_mutex_unlock(&mutex);
		printf("auto B %d fa il giro \n", indiceAuto); sleep(6);
	}
}

void* auto_antiorario(void * arg){
	int indiceAuto; int myBiglietto; indiceAuto = *(int*) arg;
	while(1){
		pthread_mutex_lock(&mutexDistributore);
		myBiglietto = bigliettoGlobaleA;
		bigliettoGlobaleA++;
		pthread_mutex_unlock(&mutexDistributore);
		pthread_mutex_lock(&mutex);
		numInAttesaA++;
		if(metaPonte[LATOA] ==  0) { pthread_cond_wait(&metaPonte, &mutex); }
		/* RIVEDERE QUESTE CONDIZIONI */
		if((metaPonte[LATOA] == 1) && 
			(myBiglietto > bigliettoA) &&
				(numInAttesaA > numInAttesaB)){
					printf("auto A %d attraversa il ponte con la metà. \n", indiceAuto);
					numInAttesaA--; ponteOccupato = 1; bigliettoA++;
					pthread_mutex_unlock(&mutex); sleep(1);
					pthread_mutex_lock(&mutex); metaPonte[LATOA] = 1;
					pthread_mutex_unlock(&mutex); sleep(1);
					pthread_mutex_lock(&mutex); 
					metaPonte[LATOA] = 0; ponteOccupato = 0; 
					pthread_cond_broadcast(&condPonteLibero);
					pthread_mutex_unlock(&mutex);
		}
		else {
			/* l'auto aspetta la precedenza e attraversa il ponte */
			while((ponteOccupato == 1) ||
					(myBiglietto > bigliettoA) || 
						(numInAttesaA < numInAttesaB)){
							pthread_cond_wait(&condPonteLibero, &mutex);
			}
			printf("auto A %d attraversa il ponte \n", indiceAuto);
			numInAttesaA--; ponteOccupato = 1; bigliettoA++;
			pthread_mutex_unlock(&mutex); sleep(1);
			pthread_mutex_lock(&mutex); metaPonte[LATOA] = 1;
			pthread_mutex_unlock(&mutex); sleep(1);
			pthread_mutex_lock(&mutex); 
			metaPonte[LATOA] = 0; ponteOccupato = 0; 
			pthread_cond_broadcast(&condPonteLibero);
			pthread_mutex_unlock(&mutex);
			
		}
		printf("auto A %d fa il giro \n", indiceAuto); sleep(6);
	}
	/* pthread_exit(NULL); */
}

int main(void){
	pthread_t tid[NUMAUTOORARIO + NUMAUTOANTI]; int indice; int *p;
	/*for(indice = 0; indice < NUMAUTOORARIO; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid[indice], NULL, auto_orario, (void*)p);
	}*/
	for(indice = 0; indice < NUMAUTOANTI; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid[indice], NULL, auto_antiorario, (void*)p);
	}
	pthread_exit(NULL);
} 
 
