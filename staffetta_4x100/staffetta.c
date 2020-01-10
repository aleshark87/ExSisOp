#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 

int passaggioTestimone = 0;
int indiceAtletaAttuale = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t passaggio = PTHREAD_COND_INITIALIZER;
pthread_cond_t urlo = PTHREAD_COND_INITIALIZER;

void* staffettista(void* arg){
	int indiceAtleta; indiceAtleta = * (int*) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while(indiceAtletaAttuale != indiceAtleta){
			pthread_cond_wait(&passaggio, &mutex);
		}
		if(passaggioTestimone == 1){
			printf("PRENDO IL TESTIMONE. Staffettista[%d]\n", indiceAtleta);
			passaggioTestimone = 0; printf("LASCIAMELO CAZZO!\n"); pthread_cond_signal(&urlo);
		}
		pthread_mutex_unlock(&mutex);
		printf("STAFFETTISTA[%d] INIZIO IL MIO GIRO !! \n", indiceAtleta);
		sleep(3);
		printf("STAFFETTISTA[%d] FINE FRAZIONE !! INIZIO PASSAGGIO \n", indiceAtleta);
		pthread_mutex_lock(&mutex);
		indiceAtletaAttuale++;
		if(indiceAtletaAttuale == 4){
			indiceAtletaAttuale = 0;
		}
		passaggioTestimone = 1; pthread_cond_broadcast(&passaggio);
		while(passaggioTestimone == 1){
			printf("STAFFETTISTA[%d]. Aspetto che %d mi urli e gli lascio il testimone !! \n", indiceAtleta, indiceAtletaAttuale);
			pthread_cond_wait(&urlo, &mutex);
			printf("VAI CAZZO %d FAGLI IL CULO !! \n", indiceAtletaAttuale);
		}
		pthread_mutex_unlock(&mutex);
	}
}

int main(void){
	pthread_t tid; int indice; int *p;
	for(indice = 0; indice < 4; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid, NULL, staffettista, (void*)p);
	}
	pthread_exit(NULL);
}
			
