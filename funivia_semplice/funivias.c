#define NUMTURISTI 4
#define POSTIDISP 2
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t wait = PTHREAD_COND_INITIALIZER;
pthread_cond_t partenza = PTHREAD_COND_INITIALIZER;
pthread_cond_t fineGiro = PTHREAD_COND_INITIALIZER;

int aBordo = 0; int possoSalire = 0; int siamoTutti = 0;

void* turista(void * arg){
	int indiceTur; indiceTur = * (int*) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		/* se posso salire, salgo. Altrimenti, aspetto. */
		while((aBordo >= 2) || (possoSalire == 0)){
			printf("TUR[%d] ASPETTO . \n", indiceTur);
			pthread_cond_wait(&wait, &mutex);
		}
		aBordo++;
		printf("TUR[%d] SONO SALITO . \n", indiceTur);
		if(aBordo >= 2){
			siamoTutti = 1;
			possoSalire = 1;
			pthread_cond_signal(&partenza);
		}
		pthread_cond_wait(&fineGiro, &mutex);
		pthread_mutex_unlock(&mutex);
	}
}

void* funivia(void * arg){
	while(1){
		printf("NUOVO GIRO .. \n");
		pthread_mutex_lock(&mutex);
		possoSalire = 1; 
		pthread_cond_broadcast(&wait);
		/* se non sono saliti tutti aspetto a partire */
		while(siamoTutti == 0){
			pthread_cond_wait(&partenza, &mutex);
		}
		pthread_mutex_unlock(&mutex); 
		printf("SI PUÒ PARTIRE. \n");
		sleep(2); 
		printf("SIAMO IN CIMA AL CAMPANILE. \n");
		sleep(2);
		pthread_mutex_lock(&mutex); siamoTutti = 0; aBordo = 0;
		possoSalire = 0;
		pthread_cond_broadcast(&fineGiro);
		pthread_mutex_unlock(&mutex);
	}
}

int main(void){
	pthread_t tid[NUMTURISTI]; pthread_t tid2; int indice; int *p;
	for(indice = 0; indice < NUMTURISTI; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid[indice], NULL, turista, (void*)p);
	}
	pthread_create(&tid2, NULL, funivia, NULL);
	pthread_exit(NULL);
}
